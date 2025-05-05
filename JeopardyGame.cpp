#include "JeopardyGame.h"

using namespace vgs;

JeopardyGame::JeopardyGame(const GameConfig& config) : Game(config)
{
  m_gameTimer.setPeriodMode(true);
}


void JeopardyGame::init(IHal& hal)
{
  if (m_config.time.primary < 0)
  {
    m_config.time = getDefaultTime();
  }
}


void JeopardyGame::processCountdown(IHal& hal, GameDisplayInfo& info)
{
  if(m_gameTimer.tick(hal))
  {
    m_secondsLeft--;
    m_displayDirty = true;
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

void JeopardyGame::start(IHal& hal, GameDisplayInfo& info)
{
  m_secondsLeft = m_config.time.primary;
  info.gameTime = m_secondsLeft;
  m_gameTimer.start(hal);

  Game::start(hal, info);
}

void JeopardyGame::reset(IHal& hal, GameDisplayInfo& info)
{
  m_gameTimer.stop();

  Game::reset(hal, info);
}

void JeopardyGame::press(IHal& hal, GameDisplayInfo& info, int player)
{
  m_gameTimer.stop();

  Game::press(hal, info, player);
}

GameTime JeopardyGame::getDefaultTime()
{
  GameTime time;
  time.primary = 7; // default game time

  return time;
}
