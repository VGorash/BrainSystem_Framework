#ifndef SETTINGS_H
#define SETTINGS_H

namespace vgs
{

class ISettingsItem
{
friend class Settings;

public:
  virtual const char* getName() const = 0;
  virtual int getCounter() const = 0;
  virtual const char* getValue() const = 0;

  virtual void increment() = 0;
  virtual void decrement() = 0;

  virtual bool set(int value) = 0;

protected:
  virtual ISettingsItem* getPrevious() const = 0;
  virtual ISettingsItem* getNext() const = 0;

  virtual void setPrevious(ISettingsItem* item) = 0;
  virtual void setNext(ISettingsItem* item) = 0;
};

class SettingsItemBase : public ISettingsItem
{
public:
  // IMPORTANT: to reduce memory usage it doesn't copy name. Name pointer should be always available!
  SettingsItemBase(const char* name);

  const char* getName() const override;

protected:
  ISettingsItem* getPrevious() const override;
  ISettingsItem* getNext() const override;

  void setPrevious(ISettingsItem* item) override;
  void setNext(ISettingsItem* item) override;

protected:
  const char* m_name;

  ISettingsItem* m_previous = nullptr;
  ISettingsItem* m_next = nullptr;
};

class ListSettingsItem : public SettingsItemBase
{
public:
  // IMPORTANT: to reduce memory usage it doesn't copy values. Values pointer and string pointers inside it should be always available!
  ListSettingsItem(const char* name, int numValues, const char* const* values);

  int getCounter() const override;
  const char* getValue() const override;

  void increment() override;
  void decrement() override;

  bool set(int value) override;

protected:
  int m_numValues;
  const char* const* m_values;
  int m_counter;
};

class Settings
{
public:

  ~Settings();

  void addItem(ISettingsItem* item);

  void next();
  void previous();

  ISettingsItem* getItem(int index) const;
  ISettingsItem* getCurrentItem() const;
  int getCurrentItemIndex() const;

  int size() const;

  void dumpData(int* data) const;
  void loadData(const int* data);

private:
  int m_numItems = 0;
  int m_currentItemIndex = 0; 

  ISettingsItem* m_firstItem = nullptr;
  ISettingsItem* m_lastItem = nullptr;
  ISettingsItem* m_currentItem = nullptr;

};

} //namespace vgs

#endif
