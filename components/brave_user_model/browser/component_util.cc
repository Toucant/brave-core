/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_user_model/browser/component_util.h"

#include "brave/components/brave_user_model/browser/components.h"

namespace brave_user_model {

base::Optional<ComponentInfo> GetComponentInfo(
    const std::string& id) {
  const auto iter = components.find(id);
  if (iter == components.end()) {
    return base::nullopt;
  }

  return iter->second;
}

}  // namespace brave_user_model
