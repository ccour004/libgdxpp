/*******************************************************************************
 * Copyright 2011 See AUTHORS file.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#pragma once
#include <limits.h>

/** This class implements the xorshift128+ algorithm that is a very fast, top-quality 64-bit pseudo-random number generator. The
 * quality of this PRNG is much higher than {@link Random}'s, and its cycle length is 2<sup>128</sup>&nbsp;&minus;&nbsp;1, which
 * is more than enough for any single-thread application. More details and algorithms can be found <a
 * href="http://xorshift.di.unimi.it/">here</a>.
 * <p>
 * Instances of RandomXS128 are not thread-safe.
 * 
 * @author Inferno
 * @author davebaol */
class RandomXS128{
private:
	/** Normalization constant for double. */
	static const double NORM_DOUBLE;

	/** Normalization constant for float. */
	static const double NORM_FLOAT;

	/** The first half of the internal state of this pseudo-random number generator. */
	long seed0;

	/** The second half of the internal state of this pseudo-random number generator. */
	long seed1;
    
   static unsigned long murmurHash3 (unsigned long x) {
		x ^= x >> 33;
		x *= 0xff51afd7ed558ccdL;
		x ^= x >> 33;
		x *= 0xc4ceb9fe1a85ec53L;
		x ^= x >> 33;

		return x;
	}

protected:
	/** This protected method is final because, contrary to the superclass, it's not used anymore by the other methods. */
	int next (int bits) {
		return (int)(nextLong() & ((1L << bits) - 1));
	}
public:
	/** Creates a new random number generator. This constructor sets the seed of the random number generator to a value very likely
	 * to be distinct from any other invocation of this constructor.
	 * <p>
	 * This implementation creates a {@link Random} instance to generate the initial seed. */
	RandomXS128 () {
		setSeed(time(NULL));
	}

	/** Creates a new random number generator using a single {@code long} seed.
	 * @param seed the initial seed */
	RandomXS128 (long seed) {
		srand (seed);
	}

	/** Creates a new random number generator using two {@code long} seeds.
	 * @param seed0 the first part of the initial seed
	 * @param seed1 the second part of the initial seed */
	RandomXS128 (long seed0, long seed1) {
		setState(seed0, seed1);
	}

	/** Returns the next pseudo-random, uniformly distributed {@code long} value from this random number generator's sequence.
	 * <p>
	 * Subclasses should override this, as this is used by all other methods. */
	unsigned long nextLong () {
		unsigned long s1 = seed0;
		const unsigned long s0 = seed1;
		seed0 = s0;
		s1 ^= s1 << 23;
		return (seed1 = (s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26))) + s0;
	}

	/** Returns the next pseudo-random, uniformly distributed {@code int} value from this random number generator's sequence.
	 * <p>
	 * This implementation uses {@link #nextLong()} internally. */
	int nextInt () {
		return (int)nextLong();
	}

	/** Returns a pseudo-random, uniformly distributed {@code int} value between 0 (inclusive) and the specified value (exclusive),
	 * drawn from this random number generator's sequence.
	 * <p>
	 * This implementation uses {@link #nextLong()} internally.
	 * @param n the positive bound on the random number to be returned.
	 * @return the next pseudo-random {@code int} value between {@code 0} (inclusive) and {@code n} (exclusive). */
	int nextInt (const int n) {
		return (int)nextLong(n);
	}

	/** Returns a pseudo-random, uniformly distributed {@code long} value between 0 (inclusive) and the specified value (exclusive),
	 * drawn from this random number generator's sequence. The algorithm used to generate the value guarantees that the result is
	 * uniform, provided that the sequence of 64-bit values produced by this generator is.
	 * <p>
	 * This implementation uses {@link #nextLong()} internally.
	 * @param n the positive bound on the random number to be returned.
	 * @return the next pseudo-random {@code long} value between {@code 0} (inclusive) and {@code n} (exclusive). */
	unsigned long nextLong (const unsigned long n) {
		if (n <= 0) throw "IllegalArgumentException: n must be positive";
		for (;;) {
			const long bits = nextLong() >> 1;
			const long value = bits % n;
			if (bits - value + (n - 1) >= 0) return value;
		}
	}

	/** Returns a pseudo-random, uniformly distributed {@code double} value between 0.0 and 1.0 from this random number generator's
	 * sequence.
	 * <p>
	 * This implementation uses {@link #nextLong()} internally. */
	double nextDouble () {
		return (nextLong() >> 11) * NORM_DOUBLE;
	}

	/** Returns a pseudo-random, uniformly distributed {@code float} value between 0.0 and 1.0 from this random number generator's
	 * sequence.
	 * <p>
	 * This implementation uses {@link #nextLong()} internally. */
	float nextFloat () {
		return (float)((nextLong() >> 40) * NORM_FLOAT);
	}

	/** Returns a pseudo-random, uniformly distributed {@code boolean } value from this random number generator's sequence.
	 * <p>
	 * This implementation uses {@link #nextLong()} internally. */
	bool nextBoolean () {
		return (nextLong() & 1) != 0;
	}

	/** Generates random bytes and places them into a user-supplied byte array. The number of random bytes produced is equal to the
	 * length of the byte array.
	 * <p>
	 * This implementation uses {@link #nextLong()} internally. */
	void nextBytes (std::vector<std::uint8_t>& bytes) {
		int n = 0;
		int i = bytes.size();
		while (i != 0) {
			n = i < 8 ? i : 8; // min(i, 8);
			for (long bits = nextLong(); n-- != 0; bits >>= 8)
				bytes[--i] = (std::uint8_t)bits;
		}
	}

	/** Sets the internal seed of this generator based on the given {@code long} value.
	 * <p>
	 * The given seed is passed twice through a hash function. This way, if the user passes a small value we avoid the short
	 * irregular transient associated with states having a very small number of bits set.
	 * @param seed a nonzero seed for this generator (if zero, the generator will be seeded with {@link Long#MIN_VALUE}). */
	void setSeed (const long seed) {
		long seed0 = murmurHash3(seed == 0 ? LONG_MIN : seed);
		setState(seed0, murmurHash3(seed0));
	}

	/** Sets the internal state of this generator.
	 * @param seed0 the first part of the internal state
	 * @param seed1 the second part of the internal state */
	void setState (const long seed0, const long seed1) {
		this->seed0 = seed0;
		this->seed1 = seed1;
	}
	
	/**
	 * Returns the internal seeds to allow state saving.
	 * @param seed must be 0 or 1, designating which of the 2 long seeds to return
	 * @return the internal seed that can be used in setState
	 */
	long getState(int seed) {
		return seed == 0 ? seed0 : seed1;
	}
};

const double RandomXS128::NORM_DOUBLE = 1.0 / (1L << 53);
	/** Normalization constant for float. */
const double RandomXS128::NORM_FLOAT = 1.0 / (1L << 24);