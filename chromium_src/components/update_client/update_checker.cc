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

#include <deque>

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
  void CheckNext();
  void UpdateResultAvailable(
      const base::Optional<ProtocolParser::Results>& results,
      ErrorCategory error_category,
      int error,
      int retry_after_sec);

  base::ThreadChecker thread_checker_;

  const scoped_refptr<Configurator> config_;
  PersistedData* metadata_ = nullptr;

  // Store the parameters to CheckForUpdates(...), so we can pass them multiple
  // times to the original UpdateChecker implementation.
  std::string session_id_;
  // Needs to be a pointer because the values in IdToComponentPtrMap are of
  // type std::unique_ptr, which we can't copy. Furthermore, it is okay to keep
  // this pointer because IdToComponentPtrMap resides in in UpdateContext, which
  // outlives this class.
  const IdToComponentPtrMap* components_;
  base::flat_map<std::string, std::string> additional_attributes_;
  bool enabled_component_updates_;
  UpdateCheckCallback update_check_callback_;

  std::deque<std::string> remaining_ids_;

  // The currently running update_checker_. We keep a smart pointer to it to
  // keep it alive while this particular sequential update check takes place.
  std::unique_ptr<UpdateChecker> update_checker_;
  // Aggregates results from all sequential update requests.
  ProtocolParser::Results results_;

  DISALLOW_COPY_AND_ASSIGN(SequentialUpdateChecker);
};

SequentialUpdateChecker::SequentialUpdateChecker(
    scoped_refptr<Configurator> config,
    PersistedData* metadata)
    : config_(config), metadata_(metadata) {
      VLOG(3) << "SequentialUpdateChecker";
    }

SequentialUpdateChecker::~SequentialUpdateChecker() {
    VLOG(3) << "> ~SequentialUpdateChecker";
    DCHECK(thread_checker_.CalledOnValidThread());
    VLOG(3) << "< ~SequentialUpdateChecker";
}

void SequentialUpdateChecker::CheckForUpdates(
    const std::string& session_id,
    const std::vector<std::string>& ids_checked,
    const IdToComponentPtrMap& components,
    const base::flat_map<std::string, std::string>& additional_attributes,
    bool enabled_component_updates,
    UpdateCheckCallback update_check_callback) {
  VLOG(3) << "> CheckForUpdates";

  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(!ids_checked.empty());

  for (const auto& app_id : ids_checked) {
    VLOG(3) << ">  * " << app_id;
    remaining_ids_.push_back(app_id);
  }

  session_id_ = session_id;
  components_ = &components;
  additional_attributes_ = additional_attributes;
  enabled_component_updates_ = enabled_component_updates;
  update_check_callback_ = std::move(update_check_callback);

  VLOG(3) << "< CheckForUpdates";
}

void SequentialUpdateChecker::CheckNext() {
  VLOG(3) << "> CheckNext()";
  DCHECK(!remaining_ids_.empty());
  std::string id = remaining_ids_.pop_front();
  std::vector<std::string> id_vector = {id};

  update_checker_ = Create_ChromiumImpl(config_, metadata_);
  update_checker_->CheckForUpdates(
      session_id_, id_vector, *components_, additional_attributes_,
      enabled_component_updates_,
      base::BindOnce(&SequentialUpdateChecker::UpdateResultAvailable,
                     base::Unretained(this)));
  VLOG(3) << "< CheckNext()";
}

void SequentialUpdateChecker::UpdateResultAvailable(
    const base::Optional<ProtocolParser::Results>& results,
    ErrorCategory error_category,
    int error,
    int retry_after_sec) {
  VLOG(3) << "< UpdateResultAvailable(" << error << ")";
  DCHECK(thread_checker_.CalledOnValidThread());

  if (!error) {
    DCHECK(results);
    for (const auto& result : results->list)
      results_.list.push_back(result);
  }

  bool done = error || remaining_ids_.empty();

  if (done)
    base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE,
      base::BindOnce(std::move(update_check_callback_),
                     error ? base::nullopt :
                     base::make_optional<ProtocolParser::Results>(results_),
                     error_category, error, retry_after_sec));
  else
    CheckNext();
  VLOG(3) << "> UpdateResultAvailable(" << error << ")";
}

}  // namespace

std::unique_ptr<UpdateChecker> UpdateChecker::Create(
    scoped_refptr<Configurator> config,
    PersistedData* persistent) {
  VLOG(3) << "Create";
  return std::make_unique<SequentialUpdateChecker>(config, persistent);
}

}  // namespace update_client
