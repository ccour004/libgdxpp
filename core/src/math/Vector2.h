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
#include "Vector.h"
#include "Matrix3.h"
#include "../Serializable.h"

/** Encapsulates a 2D vector. Allows chaining methods by returning a reference to itself
 * @author badlogicgames@gmail.com */
class Matrix3;
class Vector2: public Serializable,  Vector<Vector2> {
    public:
	 static const long serialVersionUID = 913902788239530931L;

	 static Vector2 X;
	 static Vector2 Y;
	 static Vector2 Zero;

	/** the x-component of this vector **/
	 float x;
	/** the y-component of this vector **/
	 float y;

public:
	/** Constructs a  vector at (0,0) */
	 Vector2 () {
	}

	/** Constructs a vector with the given components
	 * @param x The x-component
	 * @param y The y-component */
	 Vector2 (float x, float y) {
		this->x = x;
		this->y = y;
	}

	/** Constructs a vector from the given vector
	 * @param v The vector */
	 Vector2 (const Vector2& v) {
		set(v);
	}

	
	 Vector2 cpy () {
		return  Vector2(*this);
	}

	 static float len (float x, float y) {
		return sqrt(x * x + y * y);
	}

	
	 float len () {
		return sqrt(x * x + y * y);
	}

	 static float len2 (float x, float y) {
		return x * x + y * y;
	}

	
	 float len2 () {
		return x * x + y * y;
	}

	
	 Vector2& set (const Vector2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	/** Sets the components of this vector
	 * @param x The x-component
	 * @param y The y-component
	 * @return This vector for chaining */
	 Vector2& set (float x, float y) {
		this->x = x;
		this->y = y;
		return *this;
	}

	
	 Vector2& sub (const Vector2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	/** Substracts the other vector from this vector.
	 * @param x The x-component of the other vector
	 * @param y The y-component of the other vector
	 * @return This vector for chaining */
	 Vector2& sub (float x, float y) {
		this->x -= x;
		this->y -= y;
		return *this;
	}

	
	 Vector2& nor () {
		float length = len();
		if (length != 0) {
			x /= length;
			y /= length;
		}
		return *this;
	}

	
	 Vector2& add (const Vector2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	/** Adds the given components to this vector
	 * @param x The x-component
	 * @param y The y-component
	 * @return This vector for chaining */
	 Vector2& add (float x, float y) {
		this->x += x;
		this->y += y;
		return *this;
	}

	 static float dot (float x1, float y1, float x2, float y2) {
		return x1 * x2 + y1 * y2;
	}

	
	 float dot (const Vector2& v) {
		return x * v.x + y * v.y;
	}

	 float dot (float ox, float oy) {
		return x * ox + y * oy;
	}

	
	 Vector2& scl (float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	/** Multiplies this vector by a scalar
	 * @return This vector for chaining */
	 Vector2& scl (float x,float y) {
		this->x *= x;
		this->y *= y;
		return *this;
	}

	
	 Vector2& scl (const Vector2& v) {
		this->x *= v.x;
		this->y *= v.y;
		return *this;
	}

	
	 Vector2& mulAdd (const Vector2& vec, float scalar) {
		this->x += vec.x * scalar;
		this->y += vec.y * scalar;
		return *this;
	}

	
	 Vector2& mulAdd (const Vector2& vec, const Vector2& mulVec) {
		this->x += vec.x * mulVec.x;
		this->y += vec.y * mulVec.y;
		return *this;
	}

	 static float dst (float x1, float y1, float x2, float y2) {
		float x_d = x2 - x1;
		float y_d = y2 - y1;
		return sqrt(x_d * x_d + y_d * y_d);
	}

	
	 float dst (const Vector2& v) {
		float x_d = v.x - x;
		float y_d = v.y - y;
		return sqrt(x_d * x_d + y_d * y_d);
	}

	/** @param x The x-component of the other vector
	 * @param y The y-component of the other vector
	 * @return the distance between this and the other vector */
	 float dst (float x, float y) {
		float x_d = x - this->x;
		float y_d = y - this->y;
		return sqrt(x_d * x_d + y_d * y_d);
	}

	 static float dst2 (float x1, float y1, float x2, float y2) {
		float x_d = x2 - x1;
		float y_d = y2 - y1;
		return x_d * x_d + y_d * y_d;
	}

	
	 float dst2 (const Vector2& v) {
		float x_d = v.x - x;
		float y_d = v.y - y;
		return x_d * x_d + y_d * y_d;
	}

	/** @param x The x-component of the other vector
	 * @param y The y-component of the other vector
	 * @return the squared distance between this and the other vector */
	 float dst2 (float x, float y) {
		float x_d = x - this->x;
		float y_d = y - this->y;
		return x_d * x_d + y_d * y_d;
	}

	
	 Vector2& limit (float limit) {
		return limit2(limit * limit);
	}

	
	 Vector2& limit2 (float limit2) {
		float length2 = len2();
		if (length2 > limit2) {
			return scl(sqrt(limit2 / length2));
		}
		return *this;
	}

	
	 Vector2& clamp (float min, float max) {
		float length2 = len2();
		if (length2 == 0.0f) return *this;
		float max2 = max * max;
		if (length2 > max2) return scl(sqrt(max2 / length2));
		float min2 = min * min;
		if (length2 < min2) return scl(sqrt(min2 / length2));
		return *this;
	}

	
	 Vector2& setLength (float len) {
		return setLength2(len * len);
	}

	
	 Vector2& setLength2 (float length2) {
		float oldLen2 = len2();
		return (oldLen2 == 0 || oldLen2 == length2) ? *this : scl(sqrt(length2 / oldLen2));
	}

	/** Converts this {@code Vector2} to a string in the format {@code (x,y)}.
	 * @return a string representation of this object. */
	
	 std::string toString () {
        std::stringstream ss;
		ss<< "(" << x << "," << y << ")";
        return ss.str();
	}

	/** Sets this {@code Vector2} to the value represented by the specified string according to the format of {@link #toString()}.
	 * @param v the string.
	 * @return this vector for chaining */
	 Vector2& fromString (std::string v) {
		int s = v.find(',', 1);
		if (s != -1 && v[0] == '(' && v[v.size() - 1] == ')') {
            float x = std::stof(v.substr(1, s-1));
            float y = std::stof(v.substr(s + 1, v.size()));
            return this->set(x, y);
		}
		throw  "GdxRuntimeException: Malformed Vector2: ";
	}

	/** Left-multiplies this vector by the given matrix
	 * @param mat the matrix
	 * @return this vector */
	 Vector2& mul (const Matrix3& mat);

	/** Calculates the 2D cross product between this and the given vector.
	 * @param v the other vector
	 * @return the cross product */
	 float crs (const Vector2& v) {
		return this->x * v.y - this->y * v.x;
	}

	/** Calculates the 2D cross product between this and the given vector.
	 * @param x the x-coordinate of the other vector
	 * @param y the y-coordinate of the other vector
	 * @return the cross product */
	 float crs (float x, float y) {
		return this->x * y - this->y * x;
	}

	/** @return the angle in degrees of this vector (point) relative to the x-axis. Angles are towards the positive y-axis (typically
	 *         counter-clockwise) and between 0 and 360. */
	 float angle ();

	/** @return the angle in degrees of this vector (point) relative to the given vector. Angles are towards the positive y-axis
	 *         (typically counter-clockwise.) between -180 and +180 */
	 float angle (const Vector2& reference);

	/** @return the angle in radians of this vector (point) relative to the x-axis. Angles are towards the positive y-axis.
	 *         (typically counter-clockwise) */
	 float angleRad ();

	/** @return the angle in radians of this vector (point) relative to the given vector. Angles are towards the positive y-axis.
	 *         (typically counter-clockwise.) */
	 float angleRad (const Vector2& reference);

	/** Sets the angle of the vector in degrees relative to the x-axis, towards the positive y-axis (typically counter-clockwise).
	 * @param degrees The angle in degrees to set. */
	 Vector2& setAngle (float degrees);

	/** Sets the angle of the vector in radians relative to the x-axis, towards the positive y-axis (typically counter-clockwise).
	 * @param radians The angle in radians to set. */
	 Vector2& setAngleRad (float radians);

	/** Rotates the Vector2 by the given angle, counter-clockwise assuming the y-axis points up.
	 * @param degrees the angle in degrees */
	 Vector2& rotate (float degrees);

	/** Rotates the Vector2 by the given angle, counter-clockwise assuming the y-axis points up.
	 * @param radians the angle in radians */
	 Vector2& rotateRad (float radians);

	/** Rotates the Vector2 by 90 degrees in the specified direction, where >= 0 is counter-clockwise and < 0 is clockwise. */
	 Vector2& rotate90 (int dir) {
		float x = this->x;
		if (dir >= 0) {
			this->x = -y;
			y = x;
		} else {
			this->x = y;
			y = -x;
		}
		return *this;
	}

	
	 Vector2& lerp (const Vector2& target, float alpha) {
		float invAlpha = 1.0f - alpha;
		this->x = (x * invAlpha) + (target.x * alpha);
		this->y = (y * invAlpha) + (target.y * alpha);
		return *this;
	}

	
	 Vector2& interpolate (const Vector2& target, float alpha, Interpolation& interpolation) {
		return lerp(target, interpolation.apply(alpha));
	}

	
	 Vector2& setToRandomDirection ();

	
	 int hashCode () {
		int prime = 31;
		int result = 1;
		result = prime * result + std::hash<float>{}(x);
		result = prime * result + std::hash<float>{}(y);
		return result;
	}

	
	 bool operator== (const Vector2& obj) {
		return x==obj.x && y==obj.y;
	}

	
	 bool epsilonEquals (const Vector2& other, float epsilon) {
		if (abs(other.x - x) > epsilon) return false;
		if (abs(other.y - y) > epsilon) return false;
		return true;
	}

	/** Compares this vector with the other vector, using the supplied epsilon for fuzzy equality testing.
	 * @return whether the vectors are the same. */
	 bool epsilonEquals (float x, float y, float epsilon) {
		if (abs(x - this->x) > epsilon) return false;
		if (abs(y - this->y) > epsilon) return false;
		return true;
	}

	
	 bool isUnit () {
		return isUnit(0.000000001f);
	}

	
	 bool isUnit (float margin) {
		return abs(len2() - 1.0f) < margin;
	}

	
	 bool isZero () {
		return x == 0 && y == 0;
	}

	
	 bool isZero (float margin) {
		return len2() < margin;
	}

	
	 bool isOnLine (const Vector2& other);

	
	 bool isOnLine (const Vector2& other, float epsilon);

	
	 bool isCollinear (const Vector2& other, float epsilon);

	
	 bool isCollinear (const Vector2& other);

	
	 bool isCollinearOpposite (const Vector2& other, float epsilon) ;

	
	 bool isCollinearOpposite (const Vector2& other);

	
	 bool isPerpendicular (const Vector2& vector);

	
	 bool isPerpendicular (const Vector2& vector, float epsilon);

	
	 bool hasSameDirection (const Vector2& vector);

	
	 bool hasOppositeDirection (const Vector2& vector);

	
	 Vector2& setZero ();
};

