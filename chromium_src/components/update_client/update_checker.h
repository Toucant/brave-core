/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_COMPONENTS_UPDATE_CLIENT_UPDATE_CHECKER_H_
#define BRAVE_CHROMIUM_SRC_COMPONENTS_UPDATE_CLIENT_UPDATE_CHECKER_H_

// Repeat all inclueds from update_checker.h to make sure our `Create` below
// doesn't replace a declaration in an included file.

#include <memory>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/containers/flat_map.h"
#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "base/optional.h"
#include "components/update_client/component.h"
#include "components/update_client/protocol_parser.h"
#include "url/gurl.h"

#define Create    \
  Create_ChromiumImpl( \
      scoped_refptr<Configurator> config, \
      PersistedData* persistent); \
  static std::unique_ptr<UpdateChecker> Create

#include "../../../../components/update_client/update_checker.h"

#undef Create

#endif  // BRAVE_CHROMIUM_SRC_COMPONENTS_UPDATE_CLIENT_UPDATE_CHECKER_H_
