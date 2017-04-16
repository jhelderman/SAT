#include "sat_utils.h"


double get_time(std::chrono::steady_clock::time_point start_time) {
  std::chrono::steady_clock::time_point now =
      std::chrono::steady_clock::now();
  std::chrono::duration<double> T =
      std::chrono::duration_cast<std::chrono::duration<double> >(now - start_time);
  return T.count();
}
