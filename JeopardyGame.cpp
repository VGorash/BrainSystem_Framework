#include "JeopardyGame.h"

using namespace vgs;

JeopardyGame::JeopardyGame(const GameConfig& config) : Game(config)
{
  m_gameTimer.setPeriodMode(true);
}


void JeopardyGame::init(Hal* hal)
{
    if (m_config.time.primary < 0)
    {
        m_config.time = getDefaultTime();
    }
}


void JeopardyGame::processCountdown(Hal* hal, GameDisplayInfo& info)
{
  if(m_gameTimer.tick(hal))
  {
    m_secondsLeft--;
    m_displayDirty = true;
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

void JeopardyGame::start(Hal* hal, GameDisplayInfo& info)
{
  m_secondsLeft = m_config.time.primary;
  info.gameTime = m_secondsLeft;
  m_gameTimer.start(hal);

  Game::start(hal, info);
}

void JeopardyGame::reset(Hal* hal, GameDisplayInfo& info)
{
  m_gameTimer.stop();

  Game::reset(hal, info);
}

void JeopardyGame::press(Hal* hal, GameDisplayInfo& info, int player)
{
  m_gameTimer.stop();

  Game::press(hal, info, player);
}

const char* JeopardyGame::getName()
{
  return "СВОЯ ИГРА";
}

GameTime JeopardyGame::getDefaultTime()
{
  GameTime time;
  time.primary = 7; // default game time

  return time;
}
