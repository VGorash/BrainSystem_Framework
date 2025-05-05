#ifndef SETTINGS_H
#define SETTINGS_H

namespace vgs
{

class ISettingsItem
{
friend class Settings;

public:
  virtual const char* getName() const = 0;
  virtual int getValueInt() const = 0;
  virtual const char* getValueStr() const = 0;

  virtual void increment() = 0;
  virtual void decrement() = 0;

  virtual bool setValueInt(int value) = 0;

  virtual ISettingsItem* getPrevious() const = 0;
  virtual ISettingsItem* getNext() const = 0;

protected:
  virtual void setPrevious(ISettingsItem* item) = 0;
  virtual void setNext(ISettingsItem* item) = 0;
};

class ISettings
{
public:

  virtual void addItem(ISettingsItem* item) = 0;

  virtual void moveNext() = 0;
  virtual void movePrevious() = 0;

  virtual ISettingsItem* getCurrentItem() const = 0;
  virtual int getCurrentItemIndex() const = 0;

  virtual int size() const = 0;

  virtual void dumpData(int* data) const = 0;
  virtual void loadData(const int* data) = 0;
};

class SettingsItemBase : public ISettingsItem
{
public:
  // IMPORTANT: to reduce memory usage it doesn't copy name. Name pointer should be always available!
  SettingsItemBase(const char* name);

  const char* getName() const override;

  ISettingsItem* getPrevious() const override;
  ISettingsItem* getNext() const override;

protected:
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

  int getValueInt() const override;
  const char* getValueStr() const override;

  void increment() override;
  void decrement() override;

  bool setValueInt(int value) override;

protected:
  int m_numValues;
  const char* const* m_values;
  int m_counter;
};

class Settings : public ISettings
{
public:

  ~Settings();

  void addItem(ISettingsItem* item) override;

  void moveNext() override;
  void movePrevious() override;

  ISettingsItem* getCurrentItem() const override;
  int getCurrentItemIndex() const override;

  int size() const override;

  void dumpData(int* data) const override;
  void loadData(const int* data) override;

protected:
  int m_numItems = 0;
  int m_currentItemIndex = 0; 

  ISettingsItem* m_firstItem = nullptr;
  ISettingsItem* m_lastItem = nullptr;
  ISettingsItem* m_currentItem = nullptr;

};

} //namespace vgs

#endif
