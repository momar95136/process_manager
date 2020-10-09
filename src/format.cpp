#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>

using std::string;
using std::setfill;
using std::setw;
using std::stringstream;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  stringstream ss;
  int hours, minutes, secs;
  minutes = seconds / 60;
  secs = seconds % 60;

  hours = minutes / 60;
  minutes = minutes % 60;
  ss << setw(2) << setfill('0') << hours << ":" << setw(2) << std::setfill('0')
     << minutes << ":" << setw(2) << setfill('0') << secs;

  return ss.str();
}