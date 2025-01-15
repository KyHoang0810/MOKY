#include "random.h"

std::mt19937 random_engine(std::random_device{}());
