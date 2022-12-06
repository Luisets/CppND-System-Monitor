#include <string>
#include <sstream>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
     std::ostringstream stream;
     stream << seconds % 60 << ":" << seconds % 3600 << ":" << seconds / 3600;
    return stream.str();
}