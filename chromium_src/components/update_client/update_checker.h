#ifndef BRAVE_CHROMIUM_SRC_COMPONENTS_UPDATE_CLIENT_UPDATE_CHECKER_H_
#define BRAVE_CHROMIUM_SRC_COMPONENTS_UPDATE_CLIENT_UPDATE_CHECKER_H_

// 'Create' - defined below - is unfortunately a very common identifier.
// Include those header files mentioned in update_checker.h which also end up
// defining it. This way, the header files won't get their Create replaced.
#include "base/callback.h"
#include "components/update_client/component.h"

#define Create    \
  Create_ChromiumImpl( \
      scoped_refptr<Configurator> config, \
      PersistedData* persistent); \
  static std::unique_ptr<UpdateChecker> Create

#include "../../../../components/update_client/update_checker.h"

#undef Create

#endif  // BRAVE_CHROMIUM_SRC_COMPONENTS_UPDATE_CLIENT_UPDATE_CHECKER_H_