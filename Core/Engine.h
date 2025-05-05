#ifndef ENGINE_H
#define ENGINE_H

#include "Hal.h"
#include "App.h"

namespace vgs
{

class Engine
{
public:
  Engine(IHal* hal, IApp* initialApp, IApp* menuApp);
  ~Engine();

  void tick();
  void init();

private:
  IHal* m_hal = nullptr;
  IApp* m_app = nullptr;
  IApp* m_menuApp = nullptr;
};

} //namespace vgs

#endif
