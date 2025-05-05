#ifndef APP_H
#define APP_H

#include "Hal.h"

namespace vgs
{

enum class AppChangeType
{
  None,
  Menu,
  Custom
};

class IApp
{
public:
  virtual void init(IHal& hal) = 0;
  virtual void tick(IHal& hal) = 0;

  virtual AppChangeType appChangeNeeded() = 0;
  virtual IApp* createCustomApp() = 0;
};

} // namespace vgs

#endif
