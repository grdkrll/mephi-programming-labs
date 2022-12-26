#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

template <typename F, typename S> struct TPair {
  F First;
  S Second;

  TPair(F a, S b) {
    First = a;
    Second = b;
  }
};

template <typename F, typename S> class TZipper {
public:
  class TZipIterator {
    using TFirstIterator = typename std::vector<F>::iterator;
    using TSecondIterator = typename std::vector<S>::iterator;

  public:
    TZipIterator()
        : firstIterator(TFirstIterator()), firstIteratorEnd(TFirstIterator()),
          secondIterator(TSecondIterator()) {}
    TZipIterator(TFirstIterator firstIter, TFirstIterator firstEnd,
                 TSecondIterator secondIter) {
      firstIterator = firstIter;
      firstIteratorEnd = firstEnd;
      secondIterator = secondIter;
    }

    virtual TPair<F, S> operator*() {
      return TPair<F, S>(*firstIterator, *secondIterator);
    }

    bool operator!=(const TZipIterator &it) {
      return firstIterator != it.firstIterator ||
             secondIterator != it.secondIterator;
    }

    virtual TZipIterator &operator++() {
      ++firstIterator;
      ++secondIterator;
      return *this;
    }

  private:
    TFirstIterator firstIterator;
    TFirstIterator firstIteratorEnd;
    TSecondIterator secondIterator;
  };

  TZipper() : First_(std::vector<F>()), Second_(std::vector<S>()) {}

  TZipper(std::vector<F> &first, std::vector<S> &second)
      : First_(first), Second_(second) {}

  TZipIterator begin() {
    return TZipIterator(First_.begin(), First_.end(), Second_.begin());
  }
  TZipIterator end() {
    return TZipIterator(First_.end(), First_.end(), Second_.end());
  }

  TZipIterator Insert(TZipIterator it, TZipper<F, S> x) {
    auto it1 = First_.insert(it.firstIterator, x.First_);
    auto it2 = Second_.insert(it.secondIterator, x.Second_);
    return TZipIterator(it1, First_.end(), it2);
  }

private:
  std::vector<F> &First_;
  std::vector<S> &Second_;
};

template <typename F, typename S>
class InsertIterator : public TZipper<F, S>::TZipIterator {
public:
  InsertIterator(TZipper<F, S> &x, typename TZipper<F, S>::TZipIterator it)
      : zip(x), iter(std::move(it)) {}
  InsertIterator &operator=(const TPair<F, S> &value) {
    iter = zip->Insert(iter, value);
    ++iter;
    return *this;
  }

  TPair<F, S> operator*() override { return *iter; }

  InsertIterator &operator++() override { return *this; }

  InsertIterator operator++(int) { return *this; }

private:
  TZipper<F, S> &zip;
  typename TZipper<F, S>::TZipIterator iter;
};

int main() {
  std::vector<int> a = {1, 2, 3};
  std::vector<std::string> b = {"abc", "def", "zxc"};
  TZipper z(a, b);

  for (auto it : z) {
    std::cout << it.First << " " << it.Second << std::endl;
  }
  // 1 abc
  // 2 def
  // 3 zxc
  return 0;
}
