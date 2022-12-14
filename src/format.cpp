#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  std::ostringstream stream;
  long hours = seconds / 3600;
  long mins = (seconds / 60) % 60;
  long secs = seconds % 60;
  stream << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2) << std::setfill('0') << mins << ":" << std::setw(2) << std::setfill('0') << secs;
  return stream.str();
}