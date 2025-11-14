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
  ButtonState buttonState = hal.getButtonState();
  
  if(buttonState.menu)
  {
    hal.sound(HalSound::None);
    hal.clearSignals();
    m_changeNeeded = AppChangeType::Menu;
    return;
  }
  
  if(m_config.mode == GameMode::Sequence && buttonState.player >= 0)
  {
    sequencePress(hal, buttonState.player);
  }
  
  m_delayTimer.tick(hal);

  if(m_delayTimer.isStarted())
  {
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
  
  if(m_config.mode == GameMode::Sequence)
  {
    m_queue.dequeue(buttonState.player);
  }

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
  
  if(m_config.mode == GameMode::Sequence)
  {
    m_queue.dequeue(buttonState.player);
  }
  
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
  m_currentPlayer = -1;
  hal.gameStartSignal();
  hal.sound(HalSound::Start);
}

void Game::reset(IHal& hal, GameDisplayInfo& info)
{
  m_state = GameState::Idle;
  m_displayDirty = true;
  hal.clearSignals();
  hal.sound(HalSound::None);
  m_currentPlayer = -1;
}

void Game::press(IHal& hal, GameDisplayInfo& info, int player)
{
  reset(hal, info);

  m_state = GameState::Press;
  hal.correctPressSignal(player);
  hal.sound(HalSound::Press);
  info.player = player;
  m_currentPlayer = player;
  m_delayTimer.start(hal);
}

void Game::falstart(IHal& hal, GameDisplayInfo& info, int player)
{
  reset(hal, info);

  m_state = GameState::Falstart;
  hal.falstartPressSignal(player);
  hal.sound(HalSound::Falstart);
  info.player = player;
  m_delayTimer.start(hal);
}

void Game::sequencePress(IHal& hal, int player)
{
  if(!m_queue.check(player) && m_currentPlayer != player)
  {
    m_queue.enqueue(player);
    updateSequencePending(hal);
  }
}

void Game::updateSequencePending(IHal& hal)
{
    hal.clearSignals();

    hal.correctPressSignal(m_currentPlayer);

    int numPendingPlayers = m_queue.size();
    int* pendingPlayers = new int[numPendingPlayers];
    m_queue.toArray(pendingPlayers);

    for (int i = 0; i < numPendingPlayers; i++)
    {
      hal.pendingPressSignal(pendingPlayers[i]);
    }

    if (m_state == GameState::Countdown)
    {
      hal.gameStartSignal();
    }

    if (m_currentPlayer != -1) // secondary press
    {
      hal.sound(HalSound::Tick);
    }

    delete[] pendingPlayers;
}

AppChangeType Game::appChangeNeeded()
{
  return m_changeNeeded;
}

IApp* Game::createCustomApp()
{
  return nullptr;
}
