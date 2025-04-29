#ifndef ENGINE_H
#define ENGINE_H

#include "Hal.h"
#include "App.h"

namespace vgs
{

class Engine
{
public:
  Engine(Hal* hal, App* (*initialApp)(), App* (*menuApp)());
  void tick();

private:
  Hal* m_hal;
  App* m_app;
  App* m_menuApp;
};

} //namespace vgs

#endif
