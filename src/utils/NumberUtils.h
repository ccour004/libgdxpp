#pragma once

class NumberUtils{
    union int_to_float_bits {
        int32_t integer_bits;
        float float_bits;
    };
    union long_to_double_bits{
        int64_t long_bits;
        double double_bits;
    };
public:
	/*static int floatToIntBits (float value) {
		return Float.floatToIntBits(value);
	}*/

	static int floatToRawIntBits (float value) {
        union int_to_float_bits bits;
        bits.float_bits = value;
        return bits.integer_bits;
	}

	static int floatToIntColor (float value) {
		return floatToRawIntBits(value);
	}

	/** Encodes the ABGR int color as a float. The high bits are masked to avoid using floats in the NaN range, which unfortunately
	 * means the full range of alpha cannot be used. See {@link Float#intBitsToFloat(int)} javadocs. */
	static float intToFloatColor (int value) {
		return intBitsToFloat(value & 0xfeffffff);
	}

	static float intBitsToFloat (int value) {
        union int_to_float_bits bits;
        bits.integer_bits = value;
        return bits.float_bits;
	}

	/*static long doubleToLongBits (double value) {
		return Double.doubleToLongBits(value);
	}

	static double longBitsToDouble (long value) {
		return Double.longBitsToDouble(value);
	}*/
};