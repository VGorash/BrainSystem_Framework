#ifndef BRAIN_RING_GAME_H
#define BRAIN_RING_GAME_H

#include "JeopardyGame.h"

namespace vgs
{

class BrainRingGame : public JeopardyGame
{
public:
  BrainRingGame(const GameConfig& config);

  void init(IHal& hal) override;

  static GameTime getDefaultTime();

protected:
  void processPress(IHal& hal, GameDisplayInfo& info) override;
  void processCountdown(IHal& hal, GameDisplayInfo& info) override;

  void start(IHal& hal, GameDisplayInfo& info) override;  
};

} // namespace vgs

#endif
