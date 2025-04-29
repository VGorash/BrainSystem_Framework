#include "BrainRingGame.h"

#define SIGNAL_TIME 10
#define TICKS_TIME 5

using namespace vgs;

BrainRingGame::BrainRingGame(const GameConfig& config) : JeopardyGame(config)
{    
    // do nothing
}

void BrainRingGame::init(Hal* hal)
{
    if (m_config.time.primary < 0 || m_config.time.secondary < 0)
    {
        m_config.time = getDefaultTime();
    }
}

void BrainRingGame::processPress(Hal* hal, GameDisplayInfo& info)
{
  ButtonState buttonState = hal->getButtonState();

  if(buttonState.start)
  {
    hal->ledsOff();
    start(hal, info);
    return;
  }

  Game::processPress(hal, info);
}

void BrainRingGame::processCountdown(Hal* hal, GameDisplayInfo& info)
{
  if(m_gameTimer.tick(hal))
  {
    m_secondsLeft--;
    m_displayDirty = true;

    if(m_secondsLeft == SIGNAL_TIME)
    {
      hal->sound(HalSound::Signal);
    }

    if(m_secondsLeft <= TICKS_TIME)
    {
      hal->sound(HalSound::Tick);
    }
  }
  info.gameTime = m_secondsLeft;

  if(m_secondsLeft <= 0)
  {
    hal->sound(HalSound::End);
    reset(hal, info);
    m_delayTimer.start(hal);
    return;
  }

  Game::processCountdown(hal, info);
}

void BrainRingGame::start(Hal* hal, GameDisplayInfo& info)
{
  m_secondsLeft = m_state == GameState::Press ? m_config.time.secondary : m_config.time.primary;

  info.gameTime = m_secondsLeft;
  m_gameTimer.start(hal);

  Game::start(hal, info);
}

const char* BrainRingGame::getName()
{
  return "БРЕЙН-РИНГ";
}

static GameTime BrainRingGame::getDefaultTime()
{
  GameTime time;
  time.primary = 60; // default game time
  time.secondary = 20; // default wrong answer time

  return time;
}
