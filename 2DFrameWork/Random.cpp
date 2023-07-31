#include "framework.h"

Random::Random()
{
	random_device rd;
	randomEngine = make_shared<default_random_engine>(rd());
}

Random::~Random()
{
}

int Random::Int(const int min, const int max) const
{
	uniform_int_distribution<int> result(min, max);
	
	return result(*randomEngine);
}

float Random::Float(const float min, const float max) const
{
	uniform_real_distribution<float> result(min, max);
	
	return result(*randomEngine);
}
