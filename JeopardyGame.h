#ifndef JEOPARDY_GAME_H
#define JEOPARDY_GAME_H

#include "Game.h"

namespace vgs
{

class JeopardyGame : public Game
{
public:
  JeopardyGame(const GameConfig& config);

  void init(IHal& hal) override;

  static GameTime getDefaultTime();

protected:
  void processCountdown(IHal& hal, GameDisplayInfo& info) override;

  void start(IHal& hal, GameDisplayInfo& info) override;
  void reset(IHal& hal, GameDisplayInfo& info) override;
  void press(IHal& hal, GameDisplayInfo& info, int player) override;

protected:
  Timer m_gameTimer;
  int m_secondsLeft = 0;
};

} //namespace vgs

#endif
