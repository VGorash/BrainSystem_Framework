#ifndef HAL_H
#define HAL_H

namespace vgs
{

enum class HalSound
{
  Start,
  Press,
  Falstart,
  Tick,
  Signal,
  End,
  None
};

enum class GameState
{
  Idle,
  Countdown,
  Press,
  Falstart
};

enum class GameMode
{
  Normal,
  Falstart,
  Sequence
};

struct GameDisplayInfo
{
  const char* name = "";
  GameMode mode = GameMode::Normal;
  GameState state = GameState::Idle;
  int gameTime = -1;
  int pressTime = -1;
  int player = -1;
  const char* customStr = "";
  int customInt = 0;
};

struct CustomDisplayInfo
{
  int type = -1;
  const void* data = nullptr;
};

struct ButtonState
{
  bool start = false;
  bool stop = false;
  bool enter = false;
  bool menu = false;
  int custom = -1;
  int player = -1;
};

class IHal
{
public:
  //main functions
  virtual void init() = 0;
  virtual void tick() = 0;

  //buttons
  virtual ButtonState getButtonState() = 0;

  //signals (LEDs)
  virtual void correctPressSignal(int player) = 0;
  virtual void falstartPressSignal(int player) = 0;
  virtual void pendingPressSignal(int player) = 0;
  virtual void gameStartSignal() = 0;
  virtual void clearSignals() = 0;

  //sound
  virtual void sound(HalSound soundType) = 0;
  virtual void sound(unsigned int frequency, unsigned int duration) = 0;

  //display
  virtual void updateDisplay(const GameDisplayInfo& info) = 0;
  virtual void updateDisplay(const CustomDisplayInfo& info) = 0;

  //time
  virtual unsigned long getTimeMillis() = 0;
};

} // namespace vgs

#endif
