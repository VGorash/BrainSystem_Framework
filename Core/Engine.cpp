#include "Engine.h"

using namespace vgs;

Engine::Engine(IHal* hal, IApp* initialApp, IApp* menuApp): m_hal(hal), m_menuApp(menuApp)
{
  m_app = initialApp ? initialApp : menuApp;
}

Engine::~Engine()
{
  if(m_app != m_menuApp && m_menuApp)
  {
    delete m_menuApp;
  }

  delete m_app;
  delete m_hal;
}

void Engine::init()
{
  m_hal->init();

  m_app->init(*m_hal);

  if (m_app != m_menuApp && m_menuApp)
  {
    m_menuApp->init(*m_hal);
  }
}

void Engine::tick()
{
  m_hal->tick();  
  m_app->tick(*m_hal);

  switch(m_app->appChangeNeeded())
  {
    case AppChangeType::Custom:
    {
      IApp* temp = m_app;
      m_app = m_app->createCustomApp();
      m_app->init(*m_hal);

      if(temp != m_menuApp)
      {
        delete temp;
      }
    }
    break;

    case AppChangeType::Menu:
    {
      if(!m_menuApp)
      {
        break;
      }

      IApp* temp = m_app;
      m_app = m_menuApp;

      if(temp != m_menuApp)
      {
        delete temp;
      }
    }
    break;
  }
};
