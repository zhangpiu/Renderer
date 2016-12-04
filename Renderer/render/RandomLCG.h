#pragma once

// Linear congruential generator, LCG
class RandomLCG {
public:
	RandomLCG(unsigned seed = 0) : mSeed(seed) {}

	double operator()() {
		mSeed = 214013 * mSeed + 2531011;
		return mSeed * (1. / 4294967296);
	}

private:
	unsigned mSeed;
};