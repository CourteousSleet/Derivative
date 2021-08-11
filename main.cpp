#include <cstdint>
#include <iostream>
//#include <vector>
//#include <list>
#include <forward_list>
//#include <queue>
//#include <deque>
#include <map>
#include <string>

// <queue> has no iterator for <for &i : queue>
// <forward_list> and <list> have some conversions inside,
// so they do not work with this type of bypass

/*
 * Now we have a debugging stage.
 * I will mark everything good
 * but bad code would be removed.
 */
std::string Derivative(std::string polynomial) {

  /*
   * I.  Map is storing degrees and coefficients of the polynomial.
   * II. Monomials forward list is for storing divided monomials
   */
  std::map<int_fast32_t, int_fast32_t> powers_and_coefficients_map;
  std::forward_list<std::string> monomials;

  /*
   * If we see, that there is no 'x' in the polynomial,
   * then the whole derivative would be 0. So we just return "0".
   */
  if (polynomial.find_first_of('x') == std::string::npos)
    return (std::string) "0";

  /*
   * Here we divide the string into monomials split up by spaces.
   */
  for (uint_fast32_t i = 0; i < polynomial.length(); ++i) {
    if (polynomial.at(i) == '+') {
      polynomial.erase(i, 1);
      polynomial.insert(i, 1, ' ');
    } else if (polynomial.at(i) == '-' && i != 0) {
      polynomial.erase(i, 1);
      polynomial.insert(i, " -");
      i += 2;
    }
  }

  /*
   * Here we send each monomial in the node of the forward list.
   */
  uint_fast32_t current_position = 0;
  for (uint_fast32_t i = 0; i < polynomial.length(); ++i) {
    if (polynomial.at(i) == ' ') {
      monomials.emplace_front(polynomial.substr(current_position, i - current_position));
      current_position = i + 1;
    }
  }
  monomials.emplace_front(polynomial.substr(current_position, polynomial.length() - current_position + 1));

  /*
   * The most buggy place.
   * I didn't look for the solution
   * on the Internet, so mine is simple (maybe I'll think of the optimization later):
   * Each monomial should be parsed. So that its coefficient and degree are sent
   * to a map in the end of the cycle.
   */
  for (auto &odnochlen : monomials) {
    std::string::size_type current_position_of_minus = odnochlen.find_first_of('-');
    std::string::size_type current_position_of_x = odnochlen.find_first_of('x');
    std::string::size_type current_position_of_degree = odnochlen.find_first_of('^');

    if (current_position_of_x == std::string::npos) continue;
    else if (current_position_of_minus == std::string::npos) {                               // x > 0
      if (current_position_of_degree == std::string::npos and current_position_of_x == 0) {  // just x
        powers_and_coefficients_map[1] += 1;
      } else if (current_position_of_x == 0) {                                               // x^n
        powers_and_coefficients_map[std::atoi(odnochlen.substr(current_position_of_x + 2, odnochlen.length()).c_str())]
            += 1;
      } else {
        if (current_position_of_x == odnochlen.length() - 1) {                               // n*x

          powers_and_coefficients_map[1] += std::atoi(odnochlen.substr(0, current_position_of_x - 1).c_str());
        } else {                                                                             // n*x^m

          powers_and_coefficients_map[std::atoi(odnochlen.substr(current_position_of_x + 2,
                                                                 odnochlen.length()).c_str())] +=
              std::atoi(odnochlen.substr(0, current_position_of_x - 1).c_str());
        }
      }
    } else {                                                                                 // x < 0
      if (current_position_of_degree == std::string::npos and current_position_of_x == 1     // -x
          and current_position_of_minus == 0) {
        powers_and_coefficients_map[1] -= 1;
      } else if (current_position_of_x == 1 and current_position_of_minus == 0) {
        powers_and_coefficients_map[std::atoi(odnochlen.substr(current_position_of_x + 2, odnochlen.length()).c_str())]
            -= 1;
      } else {
        if (current_position_of_x == odnochlen.length() - 1) {

          powers_and_coefficients_map[1] -= std::atoi(odnochlen.substr(1, current_position_of_x - 1).c_str());
        } else {

          powers_and_coefficients_map[std::atoi(odnochlen.substr(current_position_of_x + 2,
                                                                 odnochlen.length()).c_str())] -=
              std::atoi(odnochlen.substr(1, current_position_of_x - 1).c_str());
        }
      }
    }
  }

  /*
   * Now we do the job.
   * We find the derivative of the whole polynomial,
   * but still work with only one monomial per cycle.
   * Map is sorted in increasing order. So I decided
   * to do insert in the beginning new string of the
   * result string.
   */
  std::string result;

  for (auto &power : powers_and_coefficients_map) {
    std::string intermediate_result;
    if (power.first != 2 and power.first != 1 and power.second != 0) {
      if (power.first * power.second < 0) {
        intermediate_result += std::to_string(power.first * power.second) + "*x^" + std::to_string(power.first - 1);
      } else {
        intermediate_result +=
            '+' + std::to_string(power.first * power.second) + "*x^" + std::to_string(power.first - 1);
      }
    } else if (power.first == 2 and power.second != 0) {
      if (power.first * power.second < 0) {
        intermediate_result += std::to_string(power.first * power.second) + "*x";
      } else {
        intermediate_result += '+' + std::to_string(power.first * power.second) + "*x";
      }
    } else if (power.first == 1 and power.second != 0) {
      if (power.first * power.second < 0) {
        intermediate_result += std::to_string(power.first * power.second);
      } else {
        intermediate_result += '+' + std::to_string(power.first * power.second);
      }
    } else if (power.first == 0)
      continue;

    result.insert(0, intermediate_result);
  }

  if (*result.cbegin() == '+') result.erase(0, 1);

  return result;
}

int main() {
  std::string polynomial;

  std::cin >> polynomial;

  std::cout << Derivative(polynomial);

  return 0;
}
