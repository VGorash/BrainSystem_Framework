#include "BrainRingGame.h"

#define SIGNAL_TIME 10
#define TICKS_TIME 5

using namespace vgs;

BrainRingGame::BrainRingGame(const GameConfig& config) : JeopardyGame(config)
{    
  // do nothing
}

void BrainRingGame::init(IHal& hal)
{
  m_name = "БРЕЙН-РИНГ";

  if (m_config.time.primary < 0 || m_config.time.secondary < 0)
  {
    m_config.time = getDefaultTime();
  }
}

void BrainRingGame::processPress(IHal& hal, GameDisplayInfo& info)
{
  ButtonState buttonState = hal.getButtonState();

  if(buttonState.start)
  {
    reset(hal, info);
    start(hal, info);
    return;
  }

  Game::processPress(hal, info);
}

void BrainRingGame::processCountdown(IHal& hal, GameDisplayInfo& info)
{
  if(m_gameTimer.tick(hal))
  {
    m_secondsLeft--;
    m_displayDirty = true;

    if(m_secondsLeft == SIGNAL_TIME)
    {
      hal.sound(HalSound::Signal);
    }

    if(m_secondsLeft <= TICKS_TIME)
    {
      hal.sound(HalSound::Tick);
    }
  }
  info.gameTime = m_secondsLeft;

  if(m_secondsLeft <= 0)
  {
    reset(hal, info);
    hal.sound(HalSound::End);
    m_delayTimer.start(hal);
    return;
  }

  Game::processCountdown(hal, info);
}

void BrainRingGame::start(IHal& hal, GameDisplayInfo& info)
{
  m_secondsLeft = m_state == GameState::Press ? m_config.time.secondary : m_config.time.primary;

  info.gameTime = m_secondsLeft;
  m_gameTimer.start(hal);

  Game::start(hal, info);
}

GameTime BrainRingGame::getDefaultTime()
{
  GameTime time;
  time.primary = 60; // default game time
  time.secondary = 20; // default wrong answer time

  return time;
}
