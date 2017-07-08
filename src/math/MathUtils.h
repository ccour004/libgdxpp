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
		Sin(int SIN_COUNT,int SIN_MASK,float radFull,float degToIndex,float degreesToRadians);
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
	static float sin (float radians);

	/** Returns the cosine in radians from a lookup table. */
	static float cos (float radians);

	/** Returns the sine in radians from a lookup table. */
	static float sinDeg (float degrees);

	/** Returns the cosine in radians from a lookup table. */
	static float cosDeg (float degrees);
	// ---

	/** Returns atan2 in radians, faster but less accurate than Math.atan2. Average error of 0.00231 radians (0.1323 degrees),
	 * largest error of 0.00488 radians (0.2796 degrees). */
	static float atan2 (float y, float x);

	// ---

	static RandomXS128 Random;

	/** Returns a random number between 0 (inclusive) and the specified value (inclusive). */
	static int random (int range);

	/** Returns a random number between start (inclusive) and end (inclusive). */
	static int random (int start, int end);

	/** Returns a random number between 0 (inclusive) and the specified value (inclusive). */
	static long random (long range);

	/** Returns a random number between start (inclusive) and end (inclusive). */
	static long random (long start, long end);

	/** Returns a random bool value. */
	static bool randomBoolean ();

	/** Returns true if a random value between 0 and 1 is less than the specified value. */
	static bool randomBoolean (float chance);

	/** Returns random number between 0.0 (inclusive) and 1.0 (exclusive). */
	static float random ();

	/** Returns a random number between 0 (inclusive) and the specified value (exclusive). */
	static float random (float range);

	/** Returns a random number between start (inclusive) and end (exclusive). */
	static float random (float start, float end);

	/** Returns -1 or 1, randomly. */
	static int randomSign ();

	/** Returns a triangularly distributed random number between -1.0 (exclusive) and 1.0 (exclusive), where values around zero are
	 * more likely.
	 * <p>
	 * This is an optimized version of {@link #randomTriangular(float, float, float) randomTriangular(-1, 1, 0)} */
	static float randomTriangular ();

	/** Returns a triangularly distributed random number between {@code -max} (exclusive) and {@code max} (exclusive), where values
	 * around zero are more likely.
	 * <p>
	 * This is an optimized version of {@link #randomTriangular(float, float, float) randomTriangular(-max, max, 0)}
	 * @param max the upper limit */
	static float randomTriangular (float max);

	/** Returns a triangularly distributed random number between {@code min} (inclusive) and {@code max} (exclusive), where the
	 * {@code mode} argument defaults to the midpoint between the bounds, giving a symmetric distribution.
	 * <p>
	 * This method is equivalent of {@link #randomTriangular(float, float, float) randomTriangular(min, max, (min + max) * .5f)}
	 * @param min the lower limit
	 * @param max the upper limit */
	static float randomTriangular (float min, float max);

	/** Returns a triangularly distributed random number between {@code min} (inclusive) and {@code max} (exclusive), where values
	 * around {@code mode} are more likely.
	 * @param min the lower limit
	 * @param max the upper limit
	 * @param mode the point around which the values are more likely */
	static float randomTriangular (float min, float max, float mode);

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
	static int floor (float value);

	/** Returns the largest integer less than or equal to the specified float. This method will only properly floor floats that are
	 * positive. Note this method simply casts the float to int. */
	static int floorPositive (float value) {
		return (int)value;
	}

	/** Returns the smallest integer greater than or equal to the specified float. This method will only properly ceil floats from
	 * -(2^14) to (Float.MAX_VALUE - 2^14). */
	static int ceil (float value);

	/** Returns the smallest integer greater than or equal to the specified float. This method will only properly ceil floats that
	 * are positive. */
	static int ceilPositive (float value);

	/** Returns the closest integer to the specified float. This method will only properly round floats from -(2^14) to
	 * (Float.MAX_VALUE - 2^14). */
	static int round (float value);

	/** Returns the closest integer to the specified float. This method will only properly round floats that are positive. */
	static int roundPositive (float value) {
		return (int)(value + 0.5f);
	}

	/** Returns true if the value is zero (using the default tolerance as upper bound) */
	static bool isZero (float value);

	/** Returns true if the value is zero.
	 * @param tolerance represent an upper bound below which the value is considered zero. */
	static bool isZero (float value, float tolerance);

	/** Returns true if a is nearly equal to b. The function uses the default floating error tolerance.
	 * @param a the first value.
	 * @param b the second value. */
	static bool isEqual (float a, float b);

	/** Returns true if a is nearly equal to b.
	 * @param a the first value.
	 * @param b the second value.
	 * @param tolerance represent an upper bound below which the two values are considered equal. */
	static bool isEqual (float a, float b, float tolerance) {
		return abs(a - b) <= tolerance;
	}

	/** @return the logarithm of value with base a */
	static float Log (float a, float value) {
		return (float)(log(value) / log(a));
	}

	/** @return the logarithm of value with base 2 */
	static float log2 (float value) {
		return Log(2, value);
	}
};