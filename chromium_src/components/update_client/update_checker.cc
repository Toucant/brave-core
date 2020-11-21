/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// Repeat all inclueds from update_checker.cc to make sure our `Create` below
// doesn't replace a declaration in an included file.

#include "components/update_client/update_checker.h"

#include <stddef.h>

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/bind.h"
#include "base/location.h"
#include "base/logging.h"
#include "base/macros.h"
#include "base/strings/stringprintf.h"
#include "base/task/post_task.h"
#include "base/task/thread_pool.h"
#include "base/threading/thread_checker.h"
#include "base/threading/thread_task_runner_handle.h"
#include "build/build_config.h"
#include "components/update_client/component.h"
#include "components/update_client/configurator.h"
#include "components/update_client/persisted_data.h"
#include "components/update_client/protocol_definition.h"
#include "components/update_client/protocol_handler.h"
#include "components/update_client/protocol_serializer.h"
#include "components/update_client/request_sender.h"
#include "components/update_client/task_traits.h"
#include "components/update_client/update_client.h"
#include "components/update_client/updater_state.h"
#include "components/update_client/utils.h"
#include "url/gurl.h"

#define Create Create_ChromiumImpl
#include "../../../../components/update_client/update_checker.cc"
#undef Create

namespace update_client {

namespace {

// SequentialUpdateChecker delegates to UpdateChecker to perform a separate
// update request for each component, instead of one request for all components.
// We do for the following reason:
// Google's ToS do not allow distributing all components. In particular, the
// Widevine plugin must be fetched from Google servers. Brave's update server
// for components handles this as follows: When an update for a Google
// component is requested, the server responds with a HTTP redirect to
// Google's server. The problem is that this only works for update requests
// for single components. But Chromium's default implementation sends a list of
// components in one request, which in Brave's case is a mix of Google and Brave
// components. To solve this, we overwrite Chromium's implementation to perform
// separate update requests instead.
class SequentialUpdateChecker : public UpdateChecker {
 public:
  SequentialUpdateChecker(scoped_refptr<Configurator> config,
                          PersistedData* metadata);
  ~SequentialUpdateChecker() override;

  // Overrides for UpdateChecker.
  void CheckForUpdates(
      const std::string& session_id,
      const std::vector<std::string>& ids_checked,
      const IdToComponentPtrMap& components,
      const base::flat_map<std::string, std::string>& additional_attributes,
      bool enabled_component_updates,
      UpdateCheckCallback update_check_callback) override;

 private:
  void Check(size_t id_index);
  void CheckNext(
      const base::Optional<ProtocolParser::Results>& results,
      ErrorCategory error_category,
      int error,
      int retry_after_sec);

  base::ThreadChecker thread_checker_;

  const scoped_refptr<Configurator> config_;
  PersistedData* metadata_ = nullptr;

  std::string session_id_;
  std::vector<std::string> ids_checked_;
  IdToComponentPtrMap components_;
  base::flat_map<std::string, std::string> additional_attributes_;
  bool enabled_component_updates_;
  UpdateCheckCallback update_check_callback_;

  size_t curr_id_ = 0;

  DISALLOW_COPY_AND_ASSIGN(SequentialUpdateChecker);
};

SequentialUpdateChecker::SequentialUpdateChecker(
    scoped_refptr<Configurator> config,
    PersistedData* metadata)
    : config_(config), metadata_(metadata) {}

SequentialUpdateChecker::~SequentialUpdateChecker() {
    DCHECK(thread_checker_.CalledOnValidThread());
}

void SequentialUpdateChecker::CheckForUpdates(
    const std::string& session_id,
    const std::vector<std::string>& ids_checked,
    const IdToComponentPtrMap& components,
    const base::flat_map<std::string, std::string>& additional_attributes,
    bool enabled_component_updates,
    UpdateCheckCallback update_check_callback) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(!ids_checked.empty());

  session_id_ = session_id;
  ids_checked_ = ids_checked;
  components_ = std::cref(components);
  additional_attributes_ = additional_attributes;
  enabled_component_updates_ = enabled_component_updates;
  update_check_callback_ = std::move(update_check_callback);

  Check(0);
}

void SequentialUpdateChecker::Check(size_t id_index) {
  std::string id = ids_checked_[id_index];
  VLOG(3) << "Checking for an update to component " << id;
  std::vector<std::string> id_vector = {id};
  Create_ChromiumImpl(config_, metadata_)->CheckForUpdates(
      session_id_, id_vector, components_, additional_attributes_,
      enabled_component_updates_,
      base::BindOnce(&SequentialUpdateChecker::CheckNext,
                     base::Unretained(this)));
}

void SequentialUpdateChecker::CheckNext(
    const base::Optional<ProtocolParser::Results>& results,
    ErrorCategory error_category,
    int error,
    int retry_after_sec) {
  DCHECK(thread_checker_.CalledOnValidThread());

  ++curr_id_;

  bool done = error || curr_id_ >= ids_checked_.size();

  if (done)
    base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE,
      base::BindOnce(std::move(update_check_callback_),
                     results, error_category, error, retry_after_sec));
  else
    Check(curr_id_);
}

}  // namespace

std::unique_ptr<UpdateChecker> UpdateChecker::Create(
    scoped_refptr<Configurator> config,
    PersistedData* persistent) {
  return std::make_unique<SequentialUpdateChecker>(config, persistent);
}

}  // namespace update_client
