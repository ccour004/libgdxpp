#include "MathUtils.h"

     RandomXS128 MathUtils::Random;
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
    
    Sin::Sin(int SIN_COUNT,int SIN_MASK,float radFull,float degToIndex,float degreesToRadians) {
            table = std::vector<float>(SIN_COUNT);
			for (int i = 0; i < SIN_COUNT; i++)
				table[i] = sin((i + 0.5f) / SIN_COUNT * radFull);
			for (int i = 0; i < 360; i += 90)
				table[(int)(i * degToIndex) & SIN_MASK] = sin(i * degreesToRadians);
		}
    
	int MathUtils::random (int range) {
		return Random.nextInt(range + 1);
	}
    
	int MathUtils::random (int start, int end) {
		return start + Random.nextInt(end - start + 1);
	}
    
	long MathUtils::random (long range) {
		return (long)(Random.nextDouble() * range);
	}
    
	long MathUtils::random (long start, long end) {
		return start + (long)(Random.nextDouble() * (end - start));
	}
    
	bool MathUtils::randomBoolean () {
		return Random.nextBoolean();
	}
    
	bool MathUtils::randomBoolean (float chance) {
		return random() < chance;
	}
    
	float MathUtils::random () {
		return Random.nextFloat();
	}
    
	float MathUtils::random (float range) {
		return Random.nextFloat() * range;
	}
    
	float MathUtils::random (float start, float end) {
		return start + Random.nextFloat() * (end - start);
	}
    
	int MathUtils::randomSign () {
		return 1 | (Random.nextInt() >> 31);
	}
    
	float MathUtils::randomTriangular () {
		return Random.nextFloat() - Random.nextFloat();
	}
    
	float MathUtils::randomTriangular (float max) {
		return (Random.nextFloat() - Random.nextFloat()) * max;
	}
    
	float MathUtils::randomTriangular (float min, float max) {
		return randomTriangular(min, max, (min + max) * 0.5f);
	}
    
	float MathUtils::randomTriangular (float min, float max, float mode) {
		float u = Random.nextFloat();
		float d = max - min;
		if (u <= (mode - min) / d) return min + sqrt(u * d * (mode - min));
		return max - sqrt((1 - u) * d * (max - mode));
	}
    
	float MathUtils::sin (float radians) {
		return SIN.table[(int)(radians * radToIndex) & SIN_MASK];
	}
    
	float MathUtils::cos (float radians) {
		return SIN.table[(int)((radians + PI / 2) * radToIndex) & SIN_MASK];
	}
    
	float MathUtils::sinDeg (float degrees) {
		return SIN.table[(int)(degrees * degToIndex) & SIN_MASK];
	}
    
	float MathUtils::cosDeg (float degrees) {
		return SIN.table[(int)((degrees + 90) * degToIndex) & SIN_MASK];
	}
    
	float MathUtils::atan2 (float y, float x) {
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
    
	int MathUtils::floor (float value) {
		return (int)(value + BIG_ENOUGH_FLOOR) - BIG_ENOUGH_INT;
	}
    
	int MathUtils::ceil (float value) {
		return BIG_ENOUGH_INT - (int)(BIG_ENOUGH_FLOOR - value);
	}
    
	int MathUtils::ceilPositive (float value) {
		return (int)(value + CEIL);
	}
    
	int MathUtils::round (float value) {
		return (int)(value + BIG_ENOUGH_ROUND) - BIG_ENOUGH_INT;
	}
    
	bool MathUtils::isZero (float value) {
		return abs(value) <= FLOAT_ROUNDING_ERROR;
	}
    
	bool MathUtils::isZero (float value, float tolerance) {
		return abs(value) <= tolerance;
	}
    
	bool MathUtils::isEqual (float a, float b) {
		return abs(a - b) <= FLOAT_ROUNDING_ERROR;
	}