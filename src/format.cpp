#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hours = seconds / 3600;
  int rest_seconds = seconds % 3600;
  int minutes = rest_seconds / 60;
  int rest = rest_seconds % 60;
  string time;
  if (hours < 10)
    time = '0' + to_string(hours) + ':';
  else
    time = to_string(hours) + ':';
  if (minutes < 10)
    time += '0' + to_string(minutes) + ':';
  else
    time += to_string(minutes) + ':';
  if (rest < 10)
    time += '0' + to_string(rest);
  else
    time += to_string(rest);
  return time;
}