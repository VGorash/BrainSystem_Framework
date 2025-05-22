#ifndef QUESTIONS_GAME_H
#define QUESTIONS_GAME_H

#include "Game.h"
#include "Timer.h"

namespace vgs
{

enum class QuestionsGameState
{
  Idle,
  Countdown,
  LastCountdown,
  Pause
};

class QuestionsGame : public IApp
{
public:
  QuestionsGame(const GameConfig& config);

  void init(IHal& hal) override;
  void tick(IHal& hal) override;

  AppChangeType appChangeNeeded() override;
  IApp* createCustomApp() override;

private:
  void processIdle(IHal& hal, GameDisplayInfo& info);
  void processCountdown(IHal& hal, GameDisplayInfo& info);
  void processLastCountdown(IHal& hal, GameDisplayInfo& info);
  void processPause(IHal& hal, GameDisplayInfo& info);

  void start(IHal& hal, GameDisplayInfo& info);
  void restart(IHal& hal, GameDisplayInfo& info);
  void pause(IHal& hal, GameDisplayInfo& info);
  void reset(IHal& hal, GameDisplayInfo& info);
  void lastCountdown(IHal& hal, GameDisplayInfo& info);

  void exit(IHal& hal);

  static GameState toGameState(QuestionsGameState state);

private:
  GameConfig m_config;

  QuestionsGameState m_state = QuestionsGameState::Idle;
  bool m_displayDirty = true;

  Timer m_gameTimer;
  int m_secondsLeft = 0;
  int m_triesLeft = 1;

  int m_tries = 1;

  AppChangeType m_changeNeeded = AppChangeType::None;
};

} // namespace vgs

#endif