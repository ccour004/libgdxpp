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
#include <math.h>
#include "RandomXS128.h"

class Sin {
        public:
		std::vector<float> table;

		Sin(int SIN_COUNT,int SIN_MASK,float radFull,float degToIndex,float degreesToRadians) {
            table = std::vector<float>(SIN_COUNT);
			for (int i = 0; i < SIN_COUNT; i++)
				table[i] = sin((i + 0.5f) / SIN_COUNT * radFull);
			for (int i = 0; i < 360; i += 90)
				table[(int)(i * degToIndex) & SIN_MASK] = sin(i * degreesToRadians);
		}
	};

/** Utility and fast math functions.
 * <p>
 * Thanks to Riven on JavaGaming.org for the basis of sin/cos/floor/ceil.
 * @author Nathan Sweet */
class MathUtils {

    static const int BIG_ENOUGH_INT;
	static const double BIG_ENOUGH_FLOOR;
	static const double CEIL;
	static const double BIG_ENOUGH_CEIL;
	static const double BIG_ENOUGH_ROUND;
public:
	static const float nanoToSec;

	// ---
	static const float FLOAT_ROUNDING_ERROR; // 32 bits
	static const float PI;
	static const float PI2;

	static const float E;

	static const int SIN_BITS; // 16KB. Adjust for accuracy.
	static const int SIN_MASK;
	static const int SIN_COUNT;

	static const float radFull;
	static const float degFull;
	static const float radToIndex;
	static const float degToIndex;

	/** multiply by this to convert from radians to degrees */
	static const float radiansToDegrees;
	static const float radDeg;
	/** multiply by this to convert from degrees to radians */
	static const float degreesToRadians;
	static const float degRad;
    
    static Sin SIN;

	/** Returns the sine in radians from a lookup table. */
	static float sin (float radians) {
		return SIN.table[(int)(radians * radToIndex) & SIN_MASK];
	}

	/** Returns the cosine in radians from a lookup table. */
	static float cos (float radians) {
		return SIN.table[(int)((radians + PI / 2) * radToIndex) & SIN_MASK];
	}

	/** Returns the sine in radians from a lookup table. */
	static float sinDeg (float degrees) {
		return SIN.table[(int)(degrees * degToIndex) & SIN_MASK];
	}

	/** Returns the cosine in radians from a lookup table. */
	static float cosDeg (float degrees) {
		return SIN.table[(int)((degrees + 90) * degToIndex) & SIN_MASK];
	}

	// ---

	/** Returns atan2 in radians, faster but less accurate than Math.atan2. Average error of 0.00231 radians (0.1323 degrees),
	 * largest error of 0.00488 radians (0.2796 degrees). */
	static float atan2 (float y, float x) {
		if (x == 0.0f) {
			if (y > 0.0f) return PI / 2;
			if (y == 0.0f) return 0.0f;
			return -PI / 2;
		}
		float atan, z = y / x;
		if (abs(z) < 1.0f) {
			atan = z / (1.0f + 0.28f * z * z);
			if (x < 0.0f) return atan + (y < 0.0f ? -PI : PI);
			return atan;
		}
		atan = PI / 2 - z / (z * z + 0.28f);
		return y < 0.0f ? atan - PI : atan;
	}

	// ---

	static RandomXS128 Random;

	/** Returns a random number between 0 (inclusive) and the specified value (inclusive). */
	static int random (int range) {
		return Random.nextInt(range + 1);
	}

	/** Returns a random number between start (inclusive) and end (inclusive). */
	static int random (int start, int end) {
		return start + Random.nextInt(end - start + 1);
	}

	/** Returns a random number between 0 (inclusive) and the specified value (inclusive). */
	static long random (long range) {
		return (long)(Random.nextDouble() * range);
	}

	/** Returns a random number between start (inclusive) and end (inclusive). */
	static long random (long start, long end) {
		return start + (long)(Random.nextDouble() * (end - start));
	}

	/** Returns a random bool value. */
	static bool randomBoolean () {
		return Random.nextBoolean();
	}

	/** Returns true if a random value between 0 and 1 is less than the specified value. */
	static bool randomBoolean (float chance) {
		return random() < chance;
	}

	/** Returns random number between 0.0 (inclusive) and 1.0 (exclusive). */
	static float random () {
		return Random.nextFloat();
	}

	/** Returns a random number between 0 (inclusive) and the specified value (exclusive). */
	static float random (float range) {
		return Random.nextFloat() * range;
	}

	/** Returns a random number between start (inclusive) and end (exclusive). */
	static float random (float start, float end) {
		return start + Random.nextFloat() * (end - start);
	}

	/** Returns -1 or 1, randomly. */
	static int randomSign () {
		return 1 | (Random.nextInt() >> 31);
	}

	/** Returns a triangularly distributed random number between -1.0 (exclusive) and 1.0 (exclusive), where values around zero are
	 * more likely.
	 * <p>
	 * This is an optimized version of {@link #randomTriangular(float, float, float) randomTriangular(-1, 1, 0)} */
	static float randomTriangular () {
		return Random.nextFloat() - Random.nextFloat();
	}

	/** Returns a triangularly distributed random number between {@code -max} (exclusive) and {@code max} (exclusive), where values
	 * around zero are more likely.
	 * <p>
	 * This is an optimized version of {@link #randomTriangular(float, float, float) randomTriangular(-max, max, 0)}
	 * @param max the upper limit */
	static float randomTriangular (float max) {
		return (Random.nextFloat() - Random.nextFloat()) * max;
	}

	/** Returns a triangularly distributed random number between {@code min} (inclusive) and {@code max} (exclusive), where the
	 * {@code mode} argument defaults to the midpoint between the bounds, giving a symmetric distribution.
	 * <p>
	 * This method is equivalent of {@link #randomTriangular(float, float, float) randomTriangular(min, max, (min + max) * .5f)}
	 * @param min the lower limit
	 * @param max the upper limit */
	static float randomTriangular (float min, float max) {
		return randomTriangular(min, max, (min + max) * 0.5f);
	}

	/** Returns a triangularly distributed random number between {@code min} (inclusive) and {@code max} (exclusive), where values
	 * around {@code mode} are more likely.
	 * @param min the lower limit
	 * @param max the upper limit
	 * @param mode the point around which the values are more likely */
	static float randomTriangular (float min, float max, float mode) {
		float u = Random.nextFloat();
		float d = max - min;
		if (u <= (mode - min) / d) return min + sqrt(u * d * (mode - min));
		return max - sqrt((1 - u) * d * (max - mode));
	}

	// ---

	/** Returns the next power of two. Returns the specified value if the value is already a power of two. */
	static int nextPowerOfTwo (int value) {
		if (value == 0) return 1;
		value--;
		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		return value + 1;
	}

	static bool isPowerOfTwo (int value) {
		return value != 0 && (value & value - 1) == 0;
	}

	// ---

	static short clamp (short value, short min, short max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	static int clamp (int value, int min, int max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	static long clamp (long value, long min, long max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	static float clamp (float value, float min, float max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	static double clamp (double value, double min, double max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	// ---

	/** Linearly interpolates between fromValue to toValue on progress position. */
	static float lerp (float fromValue, float toValue, float progress) {
		return fromValue + (toValue - fromValue) * progress;
	}

	/** Linearly interpolates between two angles in radians. Takes into account that angles wrap at two pi and always takes the
	 * direction with the smallest delta angle.
	 * 
	 * @param fromRadians start angle in radians
	 * @param toRadians target angle in radians
	 * @param progress interpolation value in the range [0, 1]
	 * @return the interpolated angle in the range [0, PI2[ */
	static float lerpAngle (float fromRadians, float toRadians, float progress) {
		float delta = fmodf((toRadians - fromRadians + PI2 + PI),PI2) - PI;
		return fmodf((fromRadians + delta * progress + PI2),PI2);
	}

	/** Linearly interpolates between two angles in degrees. Takes into account that angles wrap at 360 degrees and always takes
	 * the direction with the smallest delta angle.
	 * 
	 * @param fromDegrees start angle in degrees
	 * @param toDegrees target angle in degrees
	 * @param progress interpolation value in the range [0, 1]
	 * @return the interpolated angle in the range [0, 360[ */
	static float lerpAngleDeg (float fromDegrees, float toDegrees, float progress) {
		float delta = fmodf((toDegrees - fromDegrees + 360 + 180),360) - 180;
		return fmodf((fromDegrees + delta * progress + 360),360);
	}

	// ---

	/** Returns the largest integer less than or equal to the specified float. This method will only properly floor floats from
	 * -(2^14) to (Float.MAX_VALUE - 2^14). */
	static int floor (float value) {
		return (int)(value + BIG_ENOUGH_FLOOR) - BIG_ENOUGH_INT;
	}

	/** Returns the largest integer less than or equal to the specified float. This method will only properly floor floats that are
	 * positive. Note this method simply casts the float to int. */
	static int floorPositive (float value) {
		return (int)value;
	}

	/** Returns the smallest integer greater than or equal to the specified float. This method will only properly ceil floats from
	 * -(2^14) to (Float.MAX_VALUE - 2^14). */
	static int ceil (float value) {
		return BIG_ENOUGH_INT - (int)(BIG_ENOUGH_FLOOR - value);
	}

	/** Returns the smallest integer greater than or equal to the specified float. This method will only properly ceil floats that
	 * are positive. */
	static int ceilPositive (float value) {
		return (int)(value + CEIL);
	}

	/** Returns the closest integer to the specified float. This method will only properly round floats from -(2^14) to
	 * (Float.MAX_VALUE - 2^14). */
	static int round (float value) {
		return (int)(value + BIG_ENOUGH_ROUND) - BIG_ENOUGH_INT;
	}

	/** Returns the closest integer to the specified float. This method will only properly round floats that are positive. */
	static int roundPositive (float value) {
		return (int)(value + 0.5f);
	}

	/** Returns true if the value is zero (using the default tolerance as upper bound) */
	static bool isZero (float value) {
		return abs(value) <= FLOAT_ROUNDING_ERROR;
	}

	/** Returns true if the value is zero.
	 * @param tolerance represent an upper bound below which the value is considered zero. */
	static bool isZero (float value, float tolerance) {
		return abs(value) <= tolerance;
	}

	/** Returns true if a is nearly equal to b. The function uses the default floating error tolerance.
	 * @param a the first value.
	 * @param b the second value. */
	static bool isEqual (float a, float b) {
		return abs(a - b) <= FLOAT_ROUNDING_ERROR;
	}

	/** Returns true if a is nearly equal to b.
	 * @param a the first value.
	 * @param b the second value.
	 * @param tolerance represent an upper bound below which the two values are considered equal. */
	static bool isEqual (float a, float b, float tolerance) {
		return abs(a - b) <= tolerance;
	}

	/** @return the logarithm of value with base a */
	static float log (float a, float value) {
		return (float)(std::log(value) / std::log(a));
	}

	/** @return the logarithm of value with base 2 */
	static float log2 (float value) {
		return log(2, value);
	}
};

     const int MathUtils::BIG_ENOUGH_INT = 16 * 1024;
	 const double MathUtils::BIG_ENOUGH_FLOOR = BIG_ENOUGH_INT;
	 const double MathUtils::CEIL = 0.9999999;
	 const double MathUtils::BIG_ENOUGH_CEIL = 16384.999999999996;
	 const double MathUtils::BIG_ENOUGH_ROUND = BIG_ENOUGH_INT + 0.5f;
    
     const float MathUtils::nanoToSec = 1 / 1000000000.0f;

	// ---
	 const float MathUtils::FLOAT_ROUNDING_ERROR = 0.000001f; // 32 bits
	 const float MathUtils::PI = 3.1415927f;
	 const float MathUtils::PI2 = PI * 2;

	 const float MathUtils::E = 2.7182818f;

	 const int MathUtils::SIN_BITS = 14; // 16KB. Adjust for accuracy.
	 const int MathUtils::SIN_MASK = ~(-1 << SIN_BITS);
	 const int MathUtils::SIN_COUNT = SIN_MASK + 1;

	 const float MathUtils::radFull = PI * 2;
	 const float MathUtils::degFull = 360;
	 const float MathUtils::radToIndex = SIN_COUNT / radFull;
	 const float MathUtils::degToIndex = SIN_COUNT / degFull;

	/** multiply by this to convert from radians to degrees */
	 const float MathUtils::radiansToDegrees = 180.0f / PI;
	 const float MathUtils::radDeg = radiansToDegrees;
	/** multiply by this to convert from degrees to radians */
	 const float MathUtils::degreesToRadians = PI / 180;
	 const float MathUtils::degRad = degreesToRadians;
    
    Sin MathUtils::SIN = Sin(SIN_COUNT,SIN_MASK,radFull,degToIndex,degreesToRadians);