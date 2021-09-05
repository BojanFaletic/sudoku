#ifndef STATIC_VECTOR_HPP
#define STATIC_VECTOR_HPP

#include <cstdint>
#include <sstream>
#include <array>

// std::vector like structure without dynamic allocation
template <typename T, std::size_t N> class static_vector {
public:
  static_vector() : sz{0} {}
  static_vector(static_vector const &v) : data{v.data}, sz{v.sz} {}
  static_vector(static_vector &&v) {
    std::swap(data, v.data);
    std::swap(sz, v.sz);
  }

  void push_back(T d) { data[sz++] = d; }

  void erase(uint idx) {
    // shift one
    for (uint i = idx; i < data.size() - 1; i++) {
      data[i] = data[i + 1];
    }
    sz--;
  }

  uint size() const { return sz; }
  uint &size() { return sz; }

  auto begin() const { return data.begin(); }
  auto end() const { return data.begin() + sz; }

  auto begin() { return data.begin(); }
  auto end() { return data.begin() + sz; }

  T operator[](uint i) const { return data[i]; }
  T &operator[](uint i) { return data[i]; }

  static_vector &operator=(static_vector const &sv) {
    data = sv.data;
    sz = sv.sz;
    return *this;
  }

  static_vector &operator=(static_vector &&sv) {
    std::swap(data, sv.data);
    std::swap(sz, sv.sz);
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  static_vector<T, N> const &v) {
    for (auto const &el : v) {
      os << el << ' ';
    }
    os << '\n';
    return os;
  }

private:
  std::array<T, N> data;
  uint sz;
};

#endif
