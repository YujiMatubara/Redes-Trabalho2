#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#define PARITY_RANGE 8 //if(bitPos != 0 && bitPos % (PARITY_RANGE) == 0) std::cout << "bit of pos" << PARITY_RAGE << "is a parity bit";
#define ERROR_PROB 5 // change the error probabilty in percent 0 - 100
const std::vector<bool> CRC_DIVISOR = {1, 0, 1, 1, 0};
const std::vector<bool> frameFlag = { 0, 1, 1, 1, 1, 1, 1, 0 } // 0111 1110 inverted (they look the same...)