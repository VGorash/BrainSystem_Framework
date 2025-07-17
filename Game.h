#ifndef GAME_H
#define GAME_H

#include "Core/App.h"

#include "Timer.h"

namespace vgs
{
  
template<typename T>
struct Item
{
  Item(const T& v)
  {
    value = v;
  }
	
  T value;
  Item<T>* next = nullptr;
};

template<typename T>
class Queue
{  
public:

  Queue()
  {
    
  }
  
  void enqueue(const T& value)
  {
    Item<T>* item = new Item<T>(value);
    
    if(m_first == nullptr)
    {
      m_first = item;
    }
    
    else
    {
      addItem(item, m_first);
    }
  }
  
  bool getFirst(T& outValue)
  {
    if(m_first != nullptr)
    {
      outValue = m_first->value;
	    return true;
    }
    
    return false;
  }
  
  bool check(const T& value)
  {
    return findItem(value, m_first);
  }
  
  bool dequeue(T& outValue)
  {
    if(!getFirst(outValue))
    {
      return false;
    }
    
    Item<T>* temp = m_first;
    m_first = m_first->next;
    delete temp;
    return true;
  }
  
  void clear()
  {
    if(m_first != nullptr)
    {
    	deleteItem(m_first);
    }
    
    m_first = nullptr;
  }
  
private:
  void deleteItem(Item<T>* item)
  {
    if(item->next != nullptr)
    {
      deleteItem(item->next);
    }
	
    delete item;
  }
  
  void addItem(Item<T>* item, Item<T>* parent)
  {
    if(parent->next == nullptr)
    {
      parent->next = item;
    }
    else
    {
      addItem(item, parent->next);
    }
  }
  
  bool findItem(const T& value, Item<T>* item)
  {
    if(item == nullptr)
    {
      return false;
    }
    
    if(item->value == value)
    {
      return true;
    }
    
    return findItem(value, item->next);
  }
  
private:
  Item<T>* m_first = nullptr;
};

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
  virtual void sequencePress(int player);

protected:
  GameState m_state = GameState::Idle;
  bool m_displayDirty = true;

  GameConfig m_config;

  Timer m_delayTimer;
  Queue<int> m_queue;
  
  unsigned long m_startTime;

  AppChangeType m_changeNeeded = AppChangeType::None;
};

}

#endif
