/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#define CheckForUpdates CheckForUpdates_ChromiumImpl
#include "../../../../components/component_updater/component_updater_service.cc"
#undef CheckForUpdates

namespace component_updater {

bool CrxUpdateService::CheckForUpdates(
  UpdateScheduler::OnFinishedCallback on_finished) {
  /* We overwrite CrxUpdateService::CheckForUpdates for the following reason:
   * Google's ToS do not allow distributing all components. In particular, the
   * Widevine plugin must be fetched from Google servers. Brave's update server
   * for components handles this as follows: When an update for a Google
   * component is requested, the server responds with a HTTP redirect to
   * Google's server. However: The Omaha update protocol - and the default
   * implementation in CrxUpdateService - allows update checks for several
   * components at once. In Brave, this list of components is a mix of Brave's
   * own and Google's. To handle this, the server would need to issue a redirect
   * for a subset of components for which an update was requested. But the
   * update protocol does not allow this. To work around this, we here switch
   * the default implementation from requesting a batch of updates to issuing
   * multiple sequential update requests.
   */
  DCHECK(thread_checker_.CalledOnValidThread());

  VLOG(1) << "CheckForUpdates: Brave override called.";

  UMA_HISTOGRAM_ENUMERATION("ComponentUpdater.Calls", UPDATE_TYPE_AUTOMATIC,
                            UPDATE_TYPE_COUNT);

  if (components_order_.empty()) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(FROM_HERE,
                                                  std::move(on_finished));
    return true;
  }

  Callback on_finished_callback = base::BindOnce(
      [](UpdateScheduler::OnFinishedCallback on_finished,
         update_client::Error error) { std::move(on_finished).Run(); },
      std::move(on_finished));

  auto last = components_order_.end() - 1;

  for (auto i = components_order_.begin(); i != components_order_.end(); ++i) {
    DCHECK(components_.find(*i) != components_.end());
    update_client_->Update(
        std::vector<std::string>({*i}),
        base::BindOnce(&CrxUpdateService::GetCrxComponents,
                       base::Unretained(this)),
        {}, false,
        base::BindOnce(
            &CrxUpdateService::OnUpdateComplete, base::Unretained(this),
            i == last ? std::move(on_finished_callback) : Callback(),
            base::TimeTicks::Now()));
  }

  return true;
}

}  // namespace component_updater
