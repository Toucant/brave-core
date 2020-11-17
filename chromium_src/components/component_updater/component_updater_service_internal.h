/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_COMPONENTS_COMPONENT_UPDATER_COMPONENT_UPDATER_SERVICE_INTERNAL_H_
#define BRAVE_CHROMIUM_SRC_COMPONENTS_COMPONENT_UPDATER_COMPONENT_UPDATER_SERVICE_INTERNAL_H_

#include "../../../../components/component_updater/component_updater_service_internal.h"

namespace component_updater {

class BraveCrxUpdateService : public CrxUpdateService {
 public:
  BraveCrxUpdateService(scoped_refptr<Configurator> config,
                        std::unique_ptr<UpdateScheduler> scheduler,
                        scoped_refptr<UpdateClient> update_client);
 private:
  bool CheckForUpdates(
      UpdateScheduler::OnFinishedCallback on_finished) override;
};

}  // namespace component_updater

#endif  // BRAVE_CHROMIUM_SRC_COMPONENTS_COMPONENT_UPDATER_COMPONENT_UPDATER_SERVICE_INTERNAL_H_
