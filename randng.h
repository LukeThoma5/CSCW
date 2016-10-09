#ifndef RANDNG_H
#define RANDNG_H

#include <random>

int randNG(int low, int high)
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(low,high);
	return dist6(rng);
}

#endif