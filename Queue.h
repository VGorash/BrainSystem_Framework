#ifndef GAME_QUEUE_H
#define GAME_QUEUE_H

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

    if (m_first == nullptr)
    {
      m_first = item;
    }

    else
    {
      addItem(item, m_first);
    }

    m_size++;
  }

  bool getFirst(T& outValue)
  {
    if (m_first != nullptr)
    {
      outValue = m_first->value;
      return true;
    }

    return false;
  }

  void toArray(T* outArray)
  {
    int i = 0;

    Item<T>* temp = m_first;

    while(temp != nullptr)
    {
      outArray[i++] = temp->value;
      temp = temp->next;
    }
  }

  bool check(const T& value)
  {
    return findItem(value, m_first);
  }

  bool dequeue(T& outValue)
  {
    if (!getFirst(outValue))
    {
      return false;
    }

    Item<T>* temp = m_first;
    m_first = m_first->next;
    delete temp;
    m_size--;
    return true;
  }

  void clear()
  {
    if (m_first != nullptr)
    {
      deleteItem(m_first);
    }

    m_first = nullptr;
    m_size = 0;
  }

  int size()
  {
    return m_size;
  }

private:
  void deleteItem(Item<T>* item)
  {
    if (item->next != nullptr)
    {
      deleteItem(item->next);
    }

    delete item;
  }

  void addItem(Item<T>* item, Item<T>* parent)
  {
    if (parent->next == nullptr)
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
    if (item == nullptr)
    {
      return false;
    }

    if (item->value == value)
    {
      return true;
    }

    return findItem(value, item->next);
  }

private:
  Item<T>* m_first = nullptr;
  int m_size = 0;
};

}

#endif
