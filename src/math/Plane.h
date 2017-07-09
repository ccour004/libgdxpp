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
#include "Vector3.h"
#include "../Serializable.h"

	/** Enum specifying on which side a point lies respective to the plane and it's normal. {@link PlaneSide#Front} is the side to
	 * which the normal points.
	 * 
	 * @author mzechner */
	enum PlaneSide {
		OnPlane, Back, Front
	};

/** A plane defined via a unit length normal and the distance from the origin, as you learned in your math class.
 * 
 * @author badlogicgames@gmail.com */
class Plane:public Serializable {
	static const long serialVersionUID = -1240652082930747866L;
public:
	Vector3 normal = Vector3();
	float d = 0;
    
    friend std::ostream& operator<<(std::ostream& os, Plane &p)  
    {  
        os << "PLANE{"<<p.normal<<","<<p.d<<"}";  
        return os;  
    } 
	
	/**
	 * Constructs a new plane with all values set to 0
	 */
	Plane () {
		
	}

	/** Constructs a new plane based on the normal and distance to the origin.
	 * 
	 * @param normal The plane normal
	 * @param d The distance to the origin */
	Plane (const Vector3& normal, float d) {
		this->normal.set(normal).nor();
		this->d = d;
	}

	/** Constructs a new plane based on the normal and a point on the plane.
	 * 
	 * @param normal The normal
	 * @param point The point on the plane */
	Plane (const Vector3& normal, const Vector3& point) {
		this->normal.set(normal).nor();
		this->d = -this->normal.dot(point);
	}

	/** Constructs a new plane out of the three given points that are considered to be on the plane. The normal is calculated via a
	 * cross product between (point1-point2)x(point2-point3)
	 * 
	 * @param point1 The first point
	 * @param point2 The second point
	 * @param point3 The third point */
	Plane (Vector3& point1, const Vector3& point2, const Vector3& point3) {
		set(point1, point2, point3);
	}

	/** Sets the plane normal and distance to the origin based on the three given points which are considered to be on the plane.
	 * The normal is calculated via a cross product between (point1-point2)x(point2-point3)
	 * 
	 * @param point1
	 * @param point2
	 * @param point3 */
	void set (Vector3& point1, const Vector3& point2, const Vector3& point3) {
		normal.set(point1).sub(point2).crs(point2.x-point3.x, point2.y-point3.y, point2.z-point3.z).nor();
		d = -(point1.dot(normal));
	}

	/** Sets the plane normal and distance
	 * 
	 * @param nx normal x-component
	 * @param ny normal y-component
	 * @param nz normal z-component
	 * @param d distance to origin */
	void set (float nx, float ny, float nz, float d) {
		normal.set(nx, ny, nz);
		this->d = d;
	}

	/** Calculates the shortest signed distance between the plane and the given point.
	 * 
	 * @param point The point
	 * @return the shortest signed distance between the plane and the point */
	float distance (const Vector3& point) {
		return normal.dot(point) + d;
	}

	/** Returns on which side the given point lies relative to the plane and its normal. PlaneSide.Front refers to the side the
	 * plane normal points to.
	 * 
	 * @param point The point
	 * @return The side the point lies relative to the plane */
	PlaneSide testPoint (const Vector3& point) {
		float dist = normal.dot(point) + d;

		if (dist == 0)
			return OnPlane;
		else if (dist < 0)
			return Back;
		else
			return Front;
	}

	/** Returns on which side the given point lies relative to the plane and its normal. PlaneSide.Front refers to the side the
	 * plane normal points to.
	 * 
	 * @param x
	 * @param y
	 * @param z
	 * @return The side the point lies relative to the plane */
	PlaneSide testPoint (float x, float y, float z) {
		float dist = normal.dot(x, y, z) + d;

		if (dist == 0)
			return OnPlane;
		else if (dist < 0)
			return Back;
		else
			return Front;
	}

	/** Returns whether the plane is facing the direction vector. Think of the direction vector as the direction a camera looks in.
	 * This method will return true if the front side of the plane determined by its normal faces the camera.
	 * 
	 * @param direction the direction
	 * @return whether the plane is front facing */
	bool isFrontFacing (const Vector3& direction) {
		float dot = normal.dot(direction);
		return dot <= 0;
	}

	/** @return The normal */
	Vector3 getNormal () {
		return normal;
	}

	/** @return The distance to the origin */
	float getD () {
		return d;
	}

	/** Sets the plane to the given point and normal.
	 * 
	 * @param point the point on the plane
	 * @param normal the normal of the plane */
	void set (Vector3& point, const Vector3& normal) {
		this->normal.set(normal);
		d = -point.dot(normal);
	}

	void set (float pointX, float pointY, float pointZ, float norX, float norY, float norZ) {
		this->normal.set(norX, norY, norZ);
		d = -(pointX * norX + pointY * norY + pointZ * norZ);
	}

	/** Sets this plane from the given plane
	 * 
	 * @param plane the plane */
	void set (const Plane& plane) {
		this->normal.set(plane.normal);
		this->d = plane.d;
	}

	std::string toString () {
        std::stringstream ss;
		ss<< normal.toString() << ", " << d;
        return ss.str();
	}
};

