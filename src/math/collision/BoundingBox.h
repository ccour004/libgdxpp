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
#include <limits>
#include "../Vector3.h"
#include "../Matrix4.h"
#include "../../Serializable.h"

/** Encapsulates an axis aligned bounding box represented by a minimum and a maximum Vector. Additionally you can query for the
 * bounding box's center, dimensions and corner points.
 *
 * @author badlogicgames@gmail.com, Xoppa */
class BoundingBox:public Serializable {
	static const long serialVersionUID = -1286036817192127343L;

	static Vector3 tmpVector;

	Vector3 cnt = Vector3();
	Vector3 dim = Vector3();
public:
	 Vector3 min = Vector3();
	 Vector3 max = Vector3();
	/** @param out The {@link Vector3} to receive the center of the bounding box.
	 * @return The vector specified with the out argument. */
	 Vector3 getCenter (Vector3 out) {
		return out.set(cnt);
	}

	 float getCenterX () {
		return cnt.x;
	}

	 float getCenterY () {
		return cnt.y;
	}

	 float getCenterZ () {
		return cnt.z;
	}

	 Vector3& getCorner000 (Vector3& out) {
		return out.set(min.x, min.y, min.z);
	}

	 Vector3& getCorner001 (Vector3& out) {
		return out.set(min.x, min.y, max.z);
	}

	 Vector3& getCorner010 (Vector3& out) {
		return out.set(min.x, max.y, min.z);
	}

	 Vector3& getCorner011 (Vector3& out) {
		return out.set(min.x, max.y, max.z);
	}

	 Vector3& getCorner100 (Vector3& out) {
		return out.set(max.x, min.y, min.z);
	}

	 Vector3& getCorner101 (Vector3& out) {
		return out.set(max.x, min.y, max.z);
	}

	 Vector3& getCorner110 (Vector3& out) {
		return out.set(max.x, max.y, min.z);
	}

	 Vector3& getCorner111 (Vector3&out) {
		return out.set(max.x, max.y, max.z);
	}

	/** @param out The {@link Vector3} to receive the dimensions of this bounding box on all three axis.
	 * @return The vector specified with the out argument */
	 Vector3& getDimensions (Vector3& out) {
		return out.set(dim);
	}

	 float getWidth () {
		return dim.x;
	}

	 float getHeight () {
		return dim.y;
	}

	 float getDepth () {
		return dim.z;
	}

	/** @param out The {@link Vector3} to receive the minimum values.
	 * @return The vector specified with the out argument */
	 Vector3& getMin (Vector3& out) {
		return out.set(min);
	}

	/** @param out The {@link Vector3} to receive the maximum values.
	 * @return The vector specified with the out argument */
	 Vector3& getMax (Vector3& out) {
		return out.set(max);
	}

	/** Constructs a new bounding box with the minimum and maximum vector set to zeros. */
	 BoundingBox () {
		clr();
	}

	/** Constructs a new bounding box from the given bounding box.
	 *
	 * @param bounds The bounding box to copy */
	 BoundingBox (const BoundingBox& bounds) {
		this->set(bounds);
	}

	/** Constructs the new bounding box using the given minimum and maximum vector.
	 *
	 * @param minimum The minimum vector
	 * @param maximum The maximum vector */
	 BoundingBox (const Vector3& minimum, const Vector3& maximum) {
		this->set(minimum, maximum);
	}

	/** Sets the given bounding box.
	 *
	 * @param bounds The bounds.
	 * @return This bounding box for chaining. */
	 BoundingBox& set (const BoundingBox& bounds) {
		this->set(bounds.min, bounds.max);
        return *this;
	}

	/** Sets the given minimum and maximum vector.
	 *
	 * @param minimum The minimum vector
	 * @param maximum The maximum vector
	 * @return This bounding box for chaining. */
	 BoundingBox& set (const Vector3& minimum, const Vector3& maximum) {
		min.set(minimum.x < maximum.x ? minimum.x : maximum.x, minimum.y < maximum.y ? minimum.y : maximum.y,
			minimum.z < maximum.z ? minimum.z : maximum.z);
		max.set(minimum.x > maximum.x ? minimum.x : maximum.x, minimum.y > maximum.y ? minimum.y : maximum.y,
			minimum.z > maximum.z ? minimum.z : maximum.z);
		cnt.set(min).add(max).scl(0.5f);
		dim.set(max).sub(min);
		return *this;
	}

	/** Sets the bounding box minimum and maximum vector from the given points.
	 *
	 * @param points The points.
	 * @return This bounding box for chaining. */
	 BoundingBox& set (const std::vector<Vector3>& points) {
		this->inf();
		for (Vector3 l_point : points)
			this->ext(l_point);
		return *this;
	}

	/** Sets the minimum and maximum vector to positive and negative infinity.
	 *
	 * @return This bounding box for chaining. */
	 BoundingBox& inf () {
		min.set(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(),std::numeric_limits<float>::infinity());
		max.set(-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity());
		cnt.set(0, 0, 0);
		dim.set(0, 0, 0);
		return *this;
	}

	/** Extends the bounding box to incorporate the given {@link Vector3}.
	 * @param point The vector
	 * @return This bounding box for chaining. */
	 BoundingBox& ext (const Vector3& point) {
		this->set(min.set(getMin(min.x, point.x), getMin(min.y, point.y), getMin(min.z, point.z)),
			max.set(getMax(max.x, point.x), getMax(max.y, point.y), getMax(max.z, point.z)));
        return *this;
	}

	/** Sets the minimum and maximum vector to zeros.
	 * @return This bounding box for chaining. */
	 BoundingBox& clr () {
		this->set(min.set(0, 0, 0), max.set(0, 0, 0));
        return *this;
	}

	/** Returns whether this bounding box is valid. This means that {@link #max} is greater than or equal to {@link #min}.
	 * @return True in case the bounding box is valid, false otherwise */
	 bool isValid () {
		return min.x <= max.x && min.y <= max.y && min.z <= max.z;
	}

	/** Extends this bounding box by the given bounding box.
	 *
	 * @param a_bounds The bounding box
	 * @return This bounding box for chaining. */
	 BoundingBox& ext (const BoundingBox& a_bounds) {
		this->set(min.set(getMin(min.x, a_bounds.min.x), getMin(min.y, a_bounds.min.y), getMin(min.z, a_bounds.min.z)),
			max.set(getMax(max.x, a_bounds.max.x), getMax(max.y, a_bounds.max.y), getMax(max.z, a_bounds.max.z)));
        return *this;
	}

	/** Extends this bounding box by the given sphere.
	 *
	 * @param center Sphere center
	 * @param radius Sphere radius
	 * @return This bounding box for chaining. */
	 BoundingBox& ext (const Vector3& center, float radius) {
		this->set(min.set(getMin(min.x, center.x - radius), getMin(min.y, center.y - radius), getMin(min.z, center.z - radius)),
			max.set(getMax(max.x, center.x + radius), getMax(max.y, center.y + radius), getMax(max.z, center.z + radius)));
        return *this;
	}

	/** Extends this bounding box by the given transformed bounding box.
	 *
	 * @param bounds The bounding box
	 * @param transform The transformation matrix to apply to bounds, before using it to extend this bounding box.
	 * @return This bounding box for chaining. */
	 BoundingBox& ext (const BoundingBox& bounds, const Matrix4& transform) {
		ext(tmpVector.set(bounds.min.x, bounds.min.y, bounds.min.z).mul(transform));
		ext(tmpVector.set(bounds.min.x, bounds.min.y, bounds.max.z).mul(transform));
		ext(tmpVector.set(bounds.min.x, bounds.max.y, bounds.min.z).mul(transform));
		ext(tmpVector.set(bounds.min.x, bounds.max.y, bounds.max.z).mul(transform));
		ext(tmpVector.set(bounds.max.x, bounds.min.y, bounds.min.z).mul(transform));
		ext(tmpVector.set(bounds.max.x, bounds.min.y, bounds.max.z).mul(transform));
		ext(tmpVector.set(bounds.max.x, bounds.max.y, bounds.min.z).mul(transform));
		ext(tmpVector.set(bounds.max.x, bounds.max.y, bounds.max.z).mul(transform));
		return *this;
	}

	/** Multiplies the bounding box by the given matrix. This is achieved by multiplying the 8 corner points and then calculating
	 * the minimum and maximum vectors from the transformed points.
	 *
	 * @param transform The matrix
	 * @return This bounding box for chaining. */
	 BoundingBox& mul (const Matrix4& transform) {
		float x0 = min.x, y0 = min.y, z0 = min.z, x1 = max.x, y1 = max.y, z1 = max.z;
		inf();
		ext(tmpVector.set(x0, y0, z0).mul(transform));
		ext(tmpVector.set(x0, y0, z1).mul(transform));
		ext(tmpVector.set(x0, y1, z0).mul(transform));
		ext(tmpVector.set(x0, y1, z1).mul(transform));
		ext(tmpVector.set(x1, y0, z0).mul(transform));
		ext(tmpVector.set(x1, y0, z1).mul(transform));
		ext(tmpVector.set(x1, y1, z0).mul(transform));
		ext(tmpVector.set(x1, y1, z1).mul(transform));
		return *this;
	}

	/** Returns whether the given bounding box is contained in this bounding box.
	 * @param b The bounding box
	 * @return Whether the given bounding box is contained */
	 bool contains (const BoundingBox& b) {
		return !isValid()
			|| (min.x <= b.min.x && min.y <= b.min.y && min.z <= b.min.z && max.x >= b.max.x && max.y >= b.max.y && max.z >= b.max.z);
	}

	/** Returns whether the given bounding box is intersecting this bounding box (at least one point in).
	 * @param b The bounding box
	 * @return Whether the given bounding box is intersected */
	 bool intersects (const BoundingBox& b) {
		if (!isValid()) return false;

		// test using SAT (separating axis theorem)

		float lx = abs(this->cnt.x - b.cnt.x);
		float sumx = (this->dim.x / 2.0f) + (b.dim.x / 2.0f);

		float ly = abs(this->cnt.y - b.cnt.y);
		float sumy = (this->dim.y / 2.0f) + (b.dim.y / 2.0f);

		float lz = abs(this->cnt.z - b.cnt.z);
		float sumz = (this->dim.z / 2.0f) + (b.dim.z / 2.0f);

		return (lx <= sumx && ly <= sumy && lz <= sumz);

	}

	/** Returns whether the given vector is contained in this bounding box.
	 * @param v The vector
	 * @return Whether the vector is contained or not. */
	 bool contains (const Vector3& v) {
		return min.x <= v.x && max.x >= v.x && min.y <= v.y && max.y >= v.y && min.z <= v.z && max.z >= v.z;
	}

	
    std::string toString () {
        std::stringstream ss;
		ss<< "[" << min.toString().c_str() << "|" << max.toString().c_str() << "]";
        return ss.str();
	}

	/** Extends the bounding box by the given vector.
	 *
	 * @param x The x-coordinate
	 * @param y The y-coordinate
	 * @param z The z-coordinate
	 * @return This bounding box for chaining. */
	 BoundingBox& ext (float x, float y, float z) {
		this->set(min.set(getMin(min.x, x), getMin(min.y, y), getMin(min.z, z)), max.set(getMax(max.x, x), getMax(max.y, y), getMax(max.z, z)));
        return *this;
    }

	static float getMin (const float a, const float b) {
		return a > b ? b : a;
	}

	static float getMax (const float a, const float b) {
		return a > b ? a : b;
	}
};
