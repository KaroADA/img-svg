#include "example.hpp"

uint32_t factorial(uint32_t number) {
    return number <= 1 ? 1 : factorial(number - 1) * number;
}
