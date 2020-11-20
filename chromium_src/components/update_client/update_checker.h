#ifndef BRAVE_CHROMIUM_SRC_COMPONENTS_UPDATE_CLIENT_UPDATE_CHECKER_H_
#define BRAVE_CHROMIUM_SRC_COMPONENTS_UPDATE_CLIENT_UPDATE_CHECKER_H_

#define Create    \
  Create_ChromiumImpl( \
      scoped_refptr<Configurator> config, \
      PersistedData* persistent); \
  static std::unique_ptr<UpdateChecker> Create

#include "../../../../components/update_client/update_checker.h"

#undef Create

#endif  // BRAVE_CHROMIUM_SRC_COMPONENTS_UPDATE_CLIENT_UPDATE_CHECKER_H_