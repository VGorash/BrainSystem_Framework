#include "QuestionsGame.h"

using namespace vgs;

QuestionsGame::QuestionsGame(const GameConfig& config) : m_config(config)
{
  m_gameTimer.setPeriodMode(true);
}

void QuestionsGame::init(IHal& hal)
{
  if (m_config.time.primary < 0 || m_config.time.secondary < 0)
  {
    m_config.time.primary = 60;
    m_config.time.secondary = 10;
  }
}

void QuestionsGame::tick(IHal& hal)
{
  if(hal.getButtonState().menu)
  {
    exit(hal);
    return;
  }

  GameDisplayInfo info;

  switch(m_state)
  {
    case QuestionsGameState::Idle:
      processIdle(hal, info);
      break;
    case QuestionsGameState::Countdown:
      processCountdown(hal, info);
      break;
    case QuestionsGameState::LastCountdown:
      processLastCountdown(hal, info);
      break;
    case QuestionsGameState::Pause:
      processPause(hal, info);
      break;
  }

  if(m_displayDirty)
  {
    info.name = m_config.displayed_name;
    info.mode = m_config.mode;
    info.state = toGameState(m_state);
    info.customInt = m_tries; // send inner mode information via custom field

    hal.updateDisplay(info);
    m_displayDirty = false;
  }
}

AppChangeType QuestionsGame::appChangeNeeded()
{
  return m_changeNeeded;
}

IApp* QuestionsGame::createCustomApp()
{
  return nullptr;
}

void QuestionsGame::processIdle(IHal& hal, GameDisplayInfo& info)
{
  ButtonState buttonState = hal.getButtonState();

  if(buttonState.enter)
  {
    m_tries++;
    m_displayDirty = true;
  }

  if(buttonState.start)
  {
    start(hal, info);
    return;
  }

  if(buttonState.stop)
  {
    reset(hal, info);
    return;
  }
}

void QuestionsGame::processCountdown(IHal& hal, GameDisplayInfo& info)
{
  ButtonState buttonState = hal.getButtonState();

  if(buttonState.stop)
  {
    reset(hal, info);
    return;
  }

  if(m_gameTimer.tick(hal))
  {
    m_secondsLeft--;
    m_displayDirty = true;

    if(m_secondsLeft == 10 && m_triesLeft == 0)
    {
      hal.sound(HalSound::Signal);
    }

    if(m_secondsLeft <= 0)
    {
      if(m_triesLeft == 0)
      {
        lastCountdown(hal, info);
        return;
      }
      else
      {
        pause(hal, info);
        hal.sound(HalSound::End);
        return;
      }
    }
  }

  info.gameTime = m_secondsLeft;
}

void QuestionsGame::processLastCountdown(IHal& hal, GameDisplayInfo& info)
{
  ButtonState buttonState = hal.getButtonState();

  if(buttonState.stop)
  {
    reset(hal, info);
    return;
  }

  if(m_gameTimer.tick(hal))
  {
    m_secondsLeft--;
    m_displayDirty = true;

    if(m_secondsLeft <= 0)
    {
      reset(hal, info);
      hal.sound(HalSound::End);
      return;
    }
  }

  info.gameTime = m_secondsLeft;
}

void QuestionsGame::processPause(IHal& hal, GameDisplayInfo& info)
{
  ButtonState buttonState = hal.getButtonState();

  if(buttonState.start)
  {
    restart(hal, info);
    return;
  }

  if(buttonState.stop)
  {
    reset(hal, info);
    return;
  }
}

void QuestionsGame::exit(IHal& hal)
{
  hal.sound(HalSound::None);
  hal.ledsOff();
  m_changeNeeded = AppChangeType::Menu;
}

void QuestionsGame::start(IHal& hal, GameDisplayInfo& info)
{
  m_triesLeft = m_tries;
  restart(hal, info);
}

void QuestionsGame::restart(IHal& hal, GameDisplayInfo& info)
{
  m_triesLeft--;
  m_secondsLeft = m_config.time.primary / m_tries;

  info.gameTime = m_secondsLeft;
  m_gameTimer.start(hal);

  m_state = QuestionsGameState::Countdown;
  m_displayDirty = true;

  hal.signalLedOn();
  hal.sound(HalSound::Start);
}

void QuestionsGame::pause(IHal& hal, GameDisplayInfo& info)
{
  m_gameTimer.stop();
  m_state = QuestionsGameState::Pause;
  m_displayDirty = true;

  info.gameTime =  m_config.time.primary / m_tries;

  hal.ledsOff();
  hal.sound(HalSound::None);
}

void QuestionsGame::reset(IHal& hal, GameDisplayInfo& info)
{
  pause(hal, info);
  info.gameTime = -1;
  m_state = QuestionsGameState::Idle;
  m_tries = 1;
}

void QuestionsGame::lastCountdown(IHal& hal, GameDisplayInfo& info)
{
  m_secondsLeft = m_config.time.secondary;

  info.gameTime = m_secondsLeft;
  m_gameTimer.start(hal);

  m_state = QuestionsGameState::LastCountdown;
  m_displayDirty = true;

  hal.sound(HalSound::Signal);
}

GameState QuestionsGame::toGameState(QuestionsGameState state)
{
  switch(state)
  {
    case QuestionsGameState::Idle:
    case QuestionsGameState::Pause:
      return GameState::Idle;

    case QuestionsGameState::Countdown:
    case QuestionsGameState::LastCountdown:
      return GameState::Countdown;
  }
}
