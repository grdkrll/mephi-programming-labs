#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class TNumber {
public:
  TNumber(std::string data) : SData_(data), IData_(0) {
    try {
      std::stol(SData_);
      if (SData_.empty()) {
        SData_ = "0";
      }
    } catch (std::invalid_argument &e) {
      SData_ = "0";
    }
  }
  TNumber(long int data) : SData_(""), IData_(data) {}

  bool operator==(TNumber &rhs) const {
    long int lN = (SData_.empty() ? IData_ : std::stol(SData_));
    long int rN = (rhs.SData_.empty() ? rhs.IData_ : std::stol(rhs.SData_));
    return lN == rN;
  }

  bool operator<(TNumber &rhs) const {
    long int lN = (SData_.empty() ? IData_ : std::stol(SData_));
    long int rN = (rhs.SData_.empty() ? rhs.IData_ : std::stol(rhs.SData_));
    return lN < rN;
  }

  bool operator>(TNumber &rhs) const {
    long int lN = (SData_.empty() ? IData_ : std::stol(SData_));
    long int rN = (rhs.SData_.empty() ? rhs.IData_ : std::stol(rhs.SData_));
    return lN > rN;
  }

  bool operator<=(TNumber &rhs) const {
    return (*this) < rhs || (*this) == rhs;
  }

  bool operator>=(TNumber &rhs) const {
    return (*this) > rhs || (*this) == rhs;
  }

  TNumber operator+(TNumber &rhs) const {
    if (SData_.empty()) {
      if (rhs.SData_.empty()) {
        return {IData_ + rhs.IData_};
      } else {
        return {IData_ + std::stol(rhs.SData_)};
      }
    } else {
      if (rhs.SData_.empty()) {
        return {SData_ + std::to_string(rhs.IData_)};
      } else {
        return {SData_ + rhs.SData_};
      };
    }
  }

  TNumber &operator+=(TNumber &rhs) {
    *this = (*this) + rhs;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &out, const TNumber &elem);

private:
  std::string SData_;
  long int IData_;
};

std::ostream &operator<<(std::ostream &out, const TNumber &elem) {
  long int value = (elem.SData_.empty() ? elem.IData_ : std::stol(elem.SData_));
  std::string type = (elem.SData_.empty() ? "long int" : "std::string");
  out << "(" << type << ", " << value << ")";
  return out;
}

std::ostream &operator<<(std::ostream &out, const std::vector<TNumber> &vec) {
  for (const auto &e : vec) {
    out << e << ' ';
  }
  return out;
}

template <class TIterator>
TNumber accumulate(TIterator begin, TIterator end, TNumber init) {
  for (; begin != end; ++begin) {
    init += *begin;
  }
  return init;
}

void KeepUnique(std::vector<TNumber> &vec) {
  std::sort(vec.begin(), vec.end());

  size_t uInd = 1;
  size_t pInd = 0;
  for (size_t i = 1; i < vec.size(); ++i) {
    if (vec[i] > vec[pInd]) {
      vec[uInd] = vec[i];
      pInd = i;
      ++uInd;
    }
  }
  vec.erase(vec.begin() + uInd, vec.end());
}
