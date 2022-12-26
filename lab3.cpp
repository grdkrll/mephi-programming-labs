#include <iostream>

/*
 * Intrusive pointer -- разновидность умных указателей, аналогично
 * std::shared_ptr позволяющий совместное владение указателем с подсчетом
 * количества ссылок и автоматическим освобождением памяти. Разница с shared_ptr
 * состоит в том, что intrusive pointer хранит счетчик ссылок в самом объекте,
 * на который ссылается.
 *
 *
 * В данном задании требуется реализовать шаблонный класс IntrusivePtr, который
 * обладает следующими свойствами:
 *
 * - Хранит внутри указатель на объект (т.н. "сырой" указатель)
 * - Ведет подсчет ссылок на хранимый объект, при этом счетчик хранится в самом
 * хранимом объекте (для этого хранимые объекты должны наследоваться от базового
 * класса IntrusivePtrCounter, код которого уже дан в задании)
 * - Владеет хранимым указателем и должен освобождать память, когда счетчик
 * достигнет нуля
 * - Позволяет создавать свои копии, увеличивая при создании каждой копии
 * счетчик ссылок в объекте
 * - Предоставляет доступ к содержимому как посредством оператора *, так и ->
 * - Позволяет принимать для хранений "сырой" указатель при конструировании
 * объекта IntrusivePtr
 * - Позволяет принимать для хранений "сырой" указатель в процессе жизни объекта
 * IntrusivePtr через оператор =
 * - Позволяет уменьшать счетчик ссылок и сбрасывать хранимый "сырой" указатель
 * в nullptr в процессе жизни объекта IntrusivePtr с помощью метода Reset()
 * - Позволяет получить хранимый "сырой" указатель с помощью метода Get()
 * - Допускает приведение к типу bool для удобных проверок на содержание
 * указателя nullptr
 * - Доступ к хранимому "сырому" указателю должен работать в том числе для const
 * объектов IntrusivePtr
 */

class IntrusivePtrCounter {
private:
  size_t counter_ = 0;

  void AddRef() { ++counter_; }
  size_t ReleaseRef() { return --counter_; }
  size_t GetRefCount() { return counter_; }
  template <class T> friend class IntrusivePtr;
};

template <class T> class IntrusivePtr {
  using value_type = T;
  using pointer = T *;
  using reference = T &;

public:
  IntrusivePtr(const IntrusivePtr &other) : Data_(other.Data_) {
    Data_->AddRef();
  }

  IntrusivePtr &operator=(const IntrusivePtr &other) {
    Data_ = other.Data_;
    return *this;
  }

  IntrusivePtr() : Data_(nullptr) {}

  IntrusivePtr(const pointer ptr) : Data_(ptr) {}

  IntrusivePtr &operator=(const pointer ptr) {
    delete Data_;
    Data_ = ptr;
    return *this;
  }

  value_type operator*() const { return *Data_; }

  pointer operator->() { return Data_; }

  void Reset() {
    Data_->ReleaseRef();
    Data_ = nullptr;
  }

  pointer Get() const { return Data_; }

  operator bool() const { return Data_ != nullptr; }

  ~IntrusivePtr() {
    if (Data_ != nullptr) {
      Data_->ReleaseRef();
      if (Data_->GetRefCount() == 0) {
        delete Data_;
      }
    } else {
      delete Data_;
    }
  }

private:
  pointer Data_;
};

// Код, помогающий в отладке:

class Foo : public IntrusivePtrCounter {
public:
  Foo(int value) : value_(value) {}
  int Bar() { return 100500; }

private:
  int value_;
  friend std::ostream &operator<<(std::ostream &, const Foo &);
};

std::ostream &operator<<(std::ostream &stream, const Foo &foo) {
  return stream << "Foo(" << foo.value_ << ')';
}

int main() {
  IntrusivePtr<Foo> ptr1;
  IntrusivePtr<Foo> ptr2;

  ptr1 = new Foo(1);
  std::cout << "*ptr1 = " << *ptr1 << std::endl;

  ptr1 = new Foo(2);
  std::cout << "*ptr1 = " << *ptr1 << std::endl;

  ptr2 = std::move(ptr1);
  std::cout << "ptr1 = " << ptr1.Get() << std::endl;
  std::cout << "*ptr2 = " << *ptr2 << std::endl;

  ptr1 = IntrusivePtr<Foo>(new Foo(11));
  std::cout << "*ptr1 = " << *ptr1 << std::endl;

  ptr1 = new Foo(12);
  std::cout << "*ptr1 = " << *ptr1 << std::endl;

  ptr1.Reset();
  std::cout << "ptr1 = " << ptr1.Get() << std::endl;

  if (!ptr1) {
    std::cout << "ptr1 is null: " << !ptr1 << std::endl;
  } else {
    return 1;
  }

  IntrusivePtr<Foo> ptr3 = new Foo(3);
  std::cout << "ptr3->Bar() = " << ptr3->Bar() << std::endl;

  const IntrusivePtr<Foo> ptr4 = new Foo(42);
  std::cout << "ptr4 is null: " << !ptr4 << std::endl;
  std::cout << "*ptr4 = " << *ptr4 << std::endl;

  return 0;
}
