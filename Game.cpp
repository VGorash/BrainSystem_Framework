#include "Game.h"

using namespace vgs;

Game::Game(const GameConfig& config) : m_config(config)
{
  // do nothing
}

void Game::init(IHal& hal)
{
  // do nothing
}

void Game::tick(IHal& hal)
{
  m_delayTimer.tick(hal);

  if(m_delayTimer.isStarted())
  {
    return;
  }

  if(hal.getButtonState().menu)
  {
    hal.sound(HalSound::None);
    hal.ledsOff();
    m_changeNeeded = AppChangeType::Menu;
    return;
  }

  GameDisplayInfo info;

  switch(m_state)
  {
    case GameState::Idle:
      processIdle(hal, info);
      break;
    case GameState::Countdown:
      processCountdown(hal, info);
      break;
    case GameState::Press:
      processPress(hal, info);
      break;
    case GameState::Falstart:
      processFalstart(hal, info);
      break;
  }

  if(m_displayDirty)
  {
    info.name = m_config.displayed_name;
    info.mode = m_config.mode;
    info.state = m_state;

    hal.updateDisplay(info);
    m_displayDirty = false;
  }
}

void Game::processIdle(IHal& hal, GameDisplayInfo& info)
{
  ButtonState buttonState = hal.getButtonState();

  if(buttonState.player >= 0)
  {
    if(m_config.mode == GameMode::Falstart)
    {
      falstart(hal, info, buttonState.player);
    }
    else
    {
      press(hal, info, buttonState.player);
    }
    return;
  }
  if(buttonState.start)
  {
    start(hal, info);
    return;
  }
}

void Game::processCountdown(IHal& hal, GameDisplayInfo& info)
{
  ButtonState buttonState = hal.getButtonState();
  
  if(buttonState.player >= 0)
  {
    info.pressTime = hal.getTimeMillis() - m_startTime;
    press(hal, info, buttonState.player);
    return;
  }
  if(buttonState.stop)
  {
    reset(hal, info);
    return;
  }
}

void Game::processPress(IHal& hal, GameDisplayInfo& info)
{
  ButtonState buttonState = hal.getButtonState();

  if(buttonState.stop)
  {
    reset(hal, info);
    return;
  }
}

void Game::processFalstart(IHal& hal, GameDisplayInfo& info)
{
  processPress(hal, info);
}

void Game::start(IHal& hal, GameDisplayInfo& info)
{
  m_state = GameState::Countdown;
  m_displayDirty = true;
  m_startTime = hal.getTimeMillis();
  hal.signalLedOn();
  hal.sound(HalSound::Start);
}

void Game::reset(IHal& hal, GameDisplayInfo& info)
{
  m_state = GameState::Idle;
  m_displayDirty = true;
  hal.ledsOff();
  hal.sound(HalSound::None);
}

void Game::press(IHal& hal, GameDisplayInfo& info, int player)
{
  reset(hal, info);

  m_state = GameState::Press;
  hal.playerLedOn(player);
  hal.sound(HalSound::Press);
  info.player = player;
  m_delayTimer.start(hal);
}

void Game::falstart(IHal& hal, GameDisplayInfo& info, int player)
{
  reset(hal, info);

  m_state = GameState::Falstart;
  hal.playerLedBlink(player);
  hal.sound(HalSound::Falstart);
  info.player = player;
  m_delayTimer.start(hal);
}

AppChangeType Game::appChangeNeeded()
{
  return m_changeNeeded;
}

IApp* Game::createCustomApp()
{
  return nullptr;
}
