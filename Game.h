#ifndef GAME_H
#define GAME_H

#include "Core/App.h"

#include "Timer.h"
#include "Queue.h"

namespace vgs
{

struct GameTime
{
  int primary = -1; // main mode duration
  int secondary = -1; // second mode duration
};

struct GameConfig
{
  const char* displayed_name; // to reduce memory usage games can skip name saving. This pointer should be always available
  GameMode mode;
  GameTime time;
};

class Game : public IApp
{
public:
  Game(const GameConfig& config);

  void init(IHal& hal) override;
  void tick(IHal& hal) override;

  AppChangeType appChangeNeeded() override;
  IApp* createCustomApp() override;

protected:
  virtual void processIdle(IHal& hal, GameDisplayInfo& info);
  virtual void processCountdown(IHal& hal, GameDisplayInfo& info);
  virtual void processPress(IHal& hal, GameDisplayInfo& info);
  virtual void processFalstart(IHal& hal, GameDisplayInfo& info);

  virtual void start(IHal& hal, GameDisplayInfo& info);
  virtual void reset(IHal& hal, GameDisplayInfo& info);
  virtual void press(IHal& hal, GameDisplayInfo& info, int player);
  virtual void falstart(IHal& hal, GameDisplayInfo& info, int player);
  virtual void sequencePress(IHal& hal, int player);

  virtual void updateSequencePending(IHal& hal);

protected:
  GameState m_state = GameState::Idle;
  bool m_displayDirty = true;

  int m_currentPlayer = -1;

  GameConfig m_config;

  Timer m_delayTimer;
  Queue<int> m_queue;
  
  unsigned long m_startTime;

  AppChangeType m_changeNeeded = AppChangeType::None;
};

}

#endif
