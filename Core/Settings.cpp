#include "Settings.h"

using namespace vgs;

SettingsItemBase::SettingsItemBase(const char* name) : m_name(name)
{
  
}

const char* SettingsItemBase::getName() const
{
  return m_name;
}

ISettingsItem* SettingsItemBase::getPrevious() const
{
  return m_previous;
}

ISettingsItem* SettingsItemBase::getNext() const
{
  return m_next;
}

void SettingsItemBase::setPrevious(ISettingsItem* item)
{
  m_previous = item;
}

void SettingsItemBase::setNext(ISettingsItem* item)
{
  m_next = item;
}

ListSettingsItem::ListSettingsItem(const char* name, int numValues, const char* const* values) : SettingsItemBase(name), m_counter(0), m_numValues(numValues), m_values(values)
{
  
}

int ListSettingsItem::getValueInt() const
{
  return m_counter;
}

const char* ListSettingsItem::getValueStr() const
{
  return m_values[m_counter];
}

void ListSettingsItem::increment()
{
  m_counter++;

  if (m_counter >= m_numValues)
  {
    m_counter = 0;
  }
}

void ListSettingsItem::decrement()
{
  m_counter--;

  if (m_counter < 0)
  {
    m_counter = m_numValues - 1;
  }
}

bool ListSettingsItem::setValueInt(int value)
{
  if(value < 0 || value >= m_numValues)
  {
    return false;
  }

  m_counter = value;

  return true;
}

Settings::~Settings()
{
  ISettingsItem* item = m_firstItem;
  while (item)
  {
    ISettingsItem* temp = item->getNext();
    delete item;
    item = temp;
  }
}

void Settings::addItem(ISettingsItem* item)
{
  m_numItems++;
    
  if(m_numItems == 1)
  {
    m_currentItem = item;
    m_firstItem = item;
  }
  else
  {
    m_lastItem->setNext(item);
    item->setPrevious(m_lastItem);
  }

  m_lastItem = item;
}

void Settings::moveNext()
{
  if (m_numItems == 0)
  {
    return;
  }
  if (m_currentItem->getNext())
  {
    m_currentItem = m_currentItem->getNext();
    m_currentItemIndex++;
  }
  else
  {
    m_currentItem = m_firstItem;
    m_currentItemIndex = 0;
  }
}

void Settings::movePrevious()
{
  if (m_numItems == 0)
  {
      return;
  }
  if (m_currentItem->getPrevious())
  {
    m_currentItem = m_currentItem->getPrevious();
    m_currentItemIndex--;
  }
  else
  {
    m_currentItem = m_lastItem;
    m_currentItemIndex = m_numItems - 1;
  }
}

ISettingsItem& Settings::getCurrentItem() const
{
  return *m_currentItem;
}

int Settings::getCurrentItemIndex() const
{
  return m_currentItemIndex;
}

int Settings::size() const
{
  return m_numItems;
}

void Settings::dumpData(int* data) const
{
  ISettingsItem* item = m_firstItem;

  for (int i = 0; i < m_numItems; i++)
  {
    data[i] = item->getValueInt();
    item = item->getNext();
  }
}

void Settings::loadData(const int* data)
{
  ISettingsItem* item = m_firstItem;

  for (int i = 0; i < m_numItems; i++)
  {
    item->setValueInt(data[i]);
    item = item->getNext();
  }
}
