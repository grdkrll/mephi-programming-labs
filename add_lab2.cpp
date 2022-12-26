#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

/*
 * Напишите шаблонный класс MapperIterator, который представляет собой итератор,
 * ссылающийся на другой итератор, и позволяющий получить результат применения
 * какой-то заданной функции к значению, на которое ссылается внутренний
 * итератор. Такой итератор MapperIterator может использоваться для реализации
 * операции map (отображение значений из одного списка в новый список с
 * применением какой-то функции для генерации значений нового списка из значений
 * старого) Также напишите вспомогательную функцию Map, которая для данной
 * функции f и для данного диапазона (begin -- end) или для данного контейнера
 * возвращает объект, содержащий методы begin и end, и по которому можно
 * итерироваться с помощью range for цикла, получая значение, преобразованные с
 * помощью функции f. Еще напишите вспомогательную функцию MakeMapperIterator,
 * которая позволяет получить экземпляр MapperIterator'а не указывая значения
 * параметров шаблона
 */

template <typename IterType, typename ResultType, typename ParamType>
class MapperIterator {
public:
  using Function = typename std::function<ResultType(ParamType)>;

  MapperIterator(IterType it, Function f)
      : it_(std::move(it)), f(std::move(f)) {}

  ResultType operator*() { return f(*it_); }

  ResultType *operator->() { return new ResultType(f(*it_)); }

  MapperIterator &operator++() {
    ++it_;
    return *this;
  }

  bool operator!=(MapperIterator<IterType, ResultType, ParamType> rhs) {
    return this->it_ != rhs.it_;
  }

private:
  IterType it_;
  Function f;
};

template <typename IterType, typename ResultType, typename ParamType>
MapperIterator<IterType, ResultType, ParamType>
MakeMapperIterator(IterType iter, std::function<ResultType(ParamType)> f) {
  return MapperIterator<IterType, ResultType, ParamType>(iter, f);
}

template <typename IterType, typename ResultType, typename ParamType>
class MapCls {
public:
  using MapIter = MapperIterator<IterType, ResultType, ParamType>;

  MapCls(IterType begin, IterType end, std::function<ResultType(ParamType)> f)
      : begin_(MakeMapperIterator<IterType, ResultType, ParamType>(begin, f)),
        end_(MakeMapperIterator<IterType, ResultType, ParamType>(end, f)) {}

  MapIter begin() { return begin_; }

  MapIter end() { return end_; }

private:
  MapIter begin_;
  MapIter end_;
};

template <typename IterType, typename ResultType, typename ParamType>
MapCls<IterType, ResultType, ParamType>
Map(IterType begin, IterType end, std::function<ResultType(ParamType)> f) {
  return MapCls<IterType, ResultType, ParamType>(begin, end, f);
}

template <typename IterType, typename ResultType, typename ParamType>
MapCls<MapperIterator<IterType, ResultType, ParamType>, ResultType, ParamType>
Map(MapCls<IterType, ResultType, ParamType> mp,
    std::function<ResultType(ParamType)> f) {
  return MapCls<MapperIterator<IterType, ResultType, ParamType>, ResultType,
                ParamType>(mp.begin(), mp.end(), f);
}

class Foo {
public:
  explicit Foo(const int &number) : number_(number) {}

  void foo() const { std::cout << number_ << " from Foo. "; }

private:
  const int &number_;
};

int main() {
  const std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8};

  std::cout << "Numbers:\n";
  for (const int &number : numbers) {
    std::cout << number << " ";
  }
  std::cout << std::endl;

  {
    std::cout << "Doubled:\n";
    std::function<int(const int &)> f = [](const int &x) { return x * 2; };
    for (const int &number : Map(numbers.begin(), numbers.end(), f)) {
      std::cout << number << " ";
    }
    std::cout << std::endl;
  }

  {
    std::cout << "Powers of 2:\n";
    std::function<int(const int &)> f = [](const int &x) {
      return std::pow(2, x);
    };
    for (auto it = MakeMapperIterator(numbers.begin(), f);
         it != MakeMapperIterator(numbers.end(), f); ++it) {
      std::cout << *it << " ";
    }
    std::cout << std::endl;
  }

  {
    std::cout << "Foofied:\n";
    std::function<Foo(const int &)> f = [](const int &x) { return Foo(x); };
    for (auto it = MakeMapperIterator(numbers.begin(), f);
         it != MakeMapperIterator(numbers.end(), f); ++it) {
      it->foo();
    }
    std::cout << std::endl;
  }

  {
    std::cout << "Double mapped:\n";
    std::function<int(const int &)> f = [](const int &x) { return x * 2; };
    std::function<int(const int &)> g = [](const int &x) { return x + 15; };
    for (const int &number :
         Map(Map(Map(numbers.begin(), numbers.end(), f), g), g)) {
      std::cout << number << " ";
    }
    std::cout << std::endl;
  }

  {
    std::cout << "Sum of doubled:\n";
    std::function<int(const int &)> f = [](const int &x) { return x * 2; };
    std::cout << std::accumulate(MakeMapperIterator(numbers.begin(), f),
                                 MakeMapperIterator(numbers.end(), f), 0)
              << std::endl;
  }
}
