#ifndef UTILS_H
#define UTILS_H

template <typename T>
T inline secureDiv(T a, T b) {
  return (b == 0) ? 0 : a / b;
}

#endif