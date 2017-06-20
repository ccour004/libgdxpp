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
#include "../GL.h"
#include <vector>
#include <math.h>

template<class T>
const T& clamp(const T& x, const T& upper, const T& lower) {
    return std::min(upper, std::max(x, lower));
}

/** Takes a linear value in the range of 0-1 and outputs a (usually) non-linear, interpolated value.
 * @author Nathan Sweet */
class Interpolation {
	/** @param a Alpha value between 0 and 1. */
	virtual float apply (float a) = 0;

	/** @param a Alpha value between 0 and 1. */
	float apply (float start, float end, float a) {
		return start + (end - start) * apply(a);
	}
};

class Pow:public Interpolation {
    protected:
		int power;
public:
		Pow (int power) {
			this->power = power;
		}

		float apply (float a) {
			if (a <= 0.5f) return pow(a * 2, power) / 2;
			return pow((a - 1) * 2, power) / (power % 2 == 0 ? -2 : 2) + 1;
		}
	};

class PowIn:public Pow {
    public:
		PowIn (int power):Pow(power) {}

		float apply (float a) {
			return pow(a, power);
		}
};

class PowOut:public Pow {
    public:
		PowOut (int power):Pow(power) {}

		float apply (float a) {
			return pow(a - 1, power) * (power % 2 == 0 ? -1 : 1) + 1;
		}
};

	static Pow pow2 = Pow(2);
	/** Slow, then fast. */
	static PowIn pow2In = PowIn(2);
	/** Fast, then slow. */
	static PowOut pow2Out = PowOut(2);
	class pow2InInverse :public Interpolation {
		public: float apply (float a) {
			return sqrt(a);
		}
	};
	class pow2OutInverse :public Interpolation {
		public: float apply (float a) {
			return 1 - sqrt(-(a - 1));
		}
	};

	static Pow pow3 =  Pow(3);
	static PowIn pow3In =  PowIn(3);
	static PowOut pow3Out =  PowOut(3);
	class pow3InInverse :public Interpolation {
		public: float apply (float a) {
			return cbrt(a);
		}
	};
	class pow3OutInverse :public Interpolation {
		public: float apply (float a) {
			return 1 - cbrt(-(a - 1));
		}
	};

	static Pow pow4 =  Pow(4);
	static PowIn pow4In =  PowIn(4);
	static PowOut pow4Out =  PowOut(4);

	static Pow pow5 =  Pow(5);
	static PowIn pow5In =  PowIn(5);
	static PowOut pow5Out =  PowOut(5);

	class sine :public Interpolation {
		public: float apply (float a) {
			return (1 - cos(a * M_PI)) / 2;
		}
	};

	class sineIn :public Interpolation {
		public: float apply (float a) {
			return 1 - cos(a * M_PI / 2);
		}
	};

	class sineOut :public Interpolation {
		public: float apply (float a) {
			return sin(a * M_PI / 2);
		}
	};
    
    class Exp: public Interpolation {
        protected:
		float value, power, min, scale;
public:
		Exp (float value, float power) {
			this->value = value;
			this->power = power;
			min = pow(value, -power);
			scale = 1 / (1 - min);
		}

		 float apply (float a) {
			if (a <= 0.5f) return (pow(value, power * (a * 2 - 1)) - min) * scale / 2;
			return (2 - (pow(value, -power * (a * 2 - 1)) - min) * scale) / 2;
		}
	};
    
    class ExpIn:public Exp {
        public:
		ExpIn (float value, float power):Exp(value,power) {
		}

		 float apply (float a) {
			return (pow(value, power * (a - 1)) - min) * scale;
		}
	};

	class ExpOut:public Exp {
        public:
		ExpOut (float value, float power):Exp(value,power) {
		}

		float apply (float a) {
			return 1 - (pow(value, -power * a) - min) * scale;
		}
	};

	static Exp expTen =  Exp(2, 10);
	static ExpIn exp10In =  ExpIn(2, 10);
	static ExpOut exp10Out =  ExpOut(2, 10);

	static Exp exp5 =  Exp(2, 5);
	static ExpIn exp5In =  ExpIn(2, 5);
	static ExpOut exp5Out =  ExpOut(2, 5);

	class circle :public Interpolation {
		public: float apply (float a) {
			if (a <= 0.5f) {
				a *= 2;
				return (1 - sqrt(1 - a * a)) / 2;
			}
			a--;
			a *= 2;
			return (sqrt(1 - a * a) + 1) / 2;
		}
	};

	class circleIn :public Interpolation {
		public: float apply (float a) {
			return 1 - sqrt(1 - a * a);
		}
	};

	class circleOut :public Interpolation {
		public: float apply (float a) {
			a--;
			return sqrt(1 - a * a);
		}
	};
    
    	 class Elastic :public Interpolation {
             protected:
		float value, power, scale, bounces;
public:
		Elastic (float value, float power, int bounces, float scale) {
			this->value = value;
			this->power = power;
			this->scale = scale;
			this->bounces = bounces * M_PI * (bounces % 2 == 0 ? 1 : -1);
		}

		float apply (float a) {
			if (a <= 0.5f) {
				a *= 2;
				return pow(value, power * (a - 1)) * sin(a * bounces) * scale / 2;
			}
			a = 1 - a;
			a *= 2;
			return 1 - pow(value, power * (a - 1)) * sin((a) * bounces) * scale / 2;
		}
	};
    
    	 class ElasticIn :public Elastic {
             public:
		ElasticIn (float value, float power, int bounces, float scale) :Elastic(value, power, bounces, scale){
			
		}

		float apply (float a) {
			if (a >= 0.99) return 1;
			return pow(value, power * (a - 1)) * sin(a * bounces) * scale;
		}
	};

	 class ElasticOut :public Elastic {
         public:
		ElasticOut (float value, float power, int bounces, float scale):Elastic(value, power, bounces, scale) {
		}

		 float apply (float a) {
			if (a == 0) return 0;
			a = 1 - a;
			return (1 - pow(value, power * (a - 1)) * sin(a * bounces) * scale);
		}
	};

	static Elastic elastic =  Elastic(2, 10, 7, 1);
	static ElasticIn elasticIn =  ElasticIn(2, 10, 6, 1);
	static ElasticOut elasticOut =  ElasticOut(2, 10, 7, 1);
    
    class Swing :public Interpolation {
		float scale;
public: 
		Swing (float scale) {
			this->scale = scale * 2;
		}

		float apply (float a) {
			if (a <= 0.5f) {
				a *= 2;
				return a * a * ((scale + 1) * a - scale) / 2;
			}
			a--;
			a *= 2;
			return a * a * ((scale + 1) * a + scale) / 2 + 1;
		}
	};

	 class SwingOut :public Interpolation {
        float scale;
public: 
		SwingOut (float scale) {
			this->scale = scale;
		}

		float apply (float a) {
			a--;
			return a * a * ((scale + 1) * a + scale) + 1;
		}
	};

	 class SwingIn :public Interpolation {
		float scale;
public: 
		SwingIn (float scale) {
			this->scale = scale;
		}

		float apply (float a) {
			return a * a * ((scale + 1) * a - scale);
		}
	};

	static Swing swing =  Swing(1.5f);
	static SwingIn swingIn =  SwingIn(2.0f);
	static SwingOut swingOut =  SwingOut(2.0f);
    
    	 class BounceOut :public Interpolation {
             protected:
		std::vector<float> widths, heights;
public: 
		BounceOut (std::vector<float> widths, std::vector<float> heights) {
			if (widths.size() != heights.size())
				SDL_Log("IllegalArgumentException: Must be the same number of widths and heights.");
			this->widths = widths;
			this->heights = heights;
		}

		BounceOut (int bounces) {
			if (bounces < 2 || bounces > 5) SDL_Log("IllegalArgumentException: bounces cannot be < 2 or > 5: %i",bounces);
			widths =  std::vector<float>(bounces);
			heights =  std::vector<float>(bounces);
			heights[0] = 1;
			switch (bounces) {
			case 2:
				widths[0] = 0.6f;
				widths[1] = 0.4f;
				heights[1] = 0.33f;
				break;
			case 3:
				widths[0] = 0.4f;
				widths[1] = 0.4f;
				widths[2] = 0.2f;
				heights[1] = 0.33f;
				heights[2] = 0.1f;
				break;
			case 4:
				widths[0] = 0.34f;
				widths[1] = 0.34f;
				widths[2] = 0.2f;
				widths[3] = 0.15f;
				heights[1] = 0.26f;
				heights[2] = 0.11f;
				heights[3] = 0.03f;
				break;
			case 5:
				widths[0] = 0.3f;
				widths[1] = 0.3f;
				widths[2] = 0.2f;
				widths[3] = 0.1f;
				widths[4] = 0.1f;
				heights[1] = 0.45f;
				heights[2] = 0.3f;
				heights[3] = 0.15f;
				heights[4] = 0.06f;
				break;
			}
			widths[0] *= 2;
		}

		float apply (float a) {
			if (a == 1) return 1;
			a += widths[0] / 2;
			float width = 0, height = 0;
			for (int i = 0, n = widths.size(); i < n; i++) {
				width = widths[i];
				if (a <= width) {
					height = heights[i];
					break;
				}
				a -= width;
			}
			a /= width;
			float z = 4 / width * height * a;
			return 1 - (z - z * a) * width;
		}
	};
    
    class Bounce :public BounceOut {
        public:
        float out (float a) {
			float test = a + widths[0] / 2;
			if (test < widths[0]) return test / (widths[0] / 2) - 1;
			return BounceOut::apply(a);
		}
        
		Bounce (std::vector<float> widths, std::vector<float> heights):BounceOut(widths,heights) {
		}

		Bounce (int bounces):BounceOut(bounces) {
		}

        float apply (float a) {
			if (a <= 0.5f) return (1 - out(1 - a * 2)) / 2;
			return out(a * 2 - 1) / 2 + 0.5f;
		}
	};

	 class BounceIn :public BounceOut {
         public: 
        BounceIn (std::vector<float> widths, std::vector<float> heights):BounceOut(widths,heights) {
		}

		BounceIn (int bounces):BounceOut(bounces) {
		}

		float apply (float a) {
			return 1 - BounceOut::apply(1 - a);
		}
	};

	static Bounce bounce =  Bounce(4);
	static BounceIn bounceIn =  BounceIn(4);
	static BounceOut bounceOut =  BounceOut(4);

class LinearInterpolation:public Interpolation{
		float apply (float a) {
			return a;
		}
};

	/** Aka "smoothstep". */
class SmoothInterpolation:public Interpolation{
		public: float apply (float a) {
			return a * a * (3 - 2 * a);
		}
};

class Smooth2Interpolation:public Interpolation{
		public: float apply (float a) {
			a = a * a * (3 - 2 * a);
			return a * a * (3 - 2 * a);
		}
};

/** By Ken Perlin. */
class SmootherInterpolation:public Interpolation{
		public: float apply (float a) {
			return clamp(a * a * a * (a * (a * 6 - 15) + 10), 0.0f, 1.0f);
    }
};

typedef SmootherInterpolation FadeInterpolation;


