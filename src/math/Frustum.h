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
#include "Matrix4.h"
#include "collision/BoundingBox.h"
#include "Plane.h"

/** A truncated rectangular pyramid. Used to define the viewable region and its projection onto the screen.
 * @see Camera#frustum */
class Frustum {
    static Vector3 tmpV;
protected:
	static const std::vector<Vector3> clipSpacePlanePoints;
	static std::vector<float> clipSpacePlanePointsArray;
    std::vector<float> planePointsArray = std::vector<float>(8 * 3);
    static bool created;
public:
    std::string toString(){
        std::stringstream ss;
        ss<<"CLIP SPACE PLANE POINTS: [";
        for(Vector3 cspp:clipSpacePlanePoints)
            ss<< cspp<<",";
        ss<<"]";
        ss<<"CLIP SPACE POINTS ARRAY: [";
        for(float cspa:clipSpacePlanePointsArray)
            ss<< cspa<<",";
        ss<<"]";
        ss<<"PLANE POINTS ARRAY: [";
        
        for(float ppa:planePointsArray)
            ss<< ppa<<",";
        ss<<"]";
        ss<<"PLANES: [";
        for(Plane plane:planes)
            ss<< plane<<",";
        ss<<"]";
        ss<<"PLANE POINTS: [";
        for(Vector3 pp:planePoints)
            ss<< pp<<",";
        ss<<"]";
        return ss.str();
    }
	/** the six clipping planes, near, far, left, right, top, bottom **/
	std::vector<Plane> planes = std::vector<Plane>(6);

	/** eight points making up the near and far clipping "rectangles". order is counter clockwise, starting at bottom left **/
	std::vector<Vector3> planePoints = {Vector3(), Vector3(), Vector3(), Vector3(), Vector3(),
		Vector3(), Vector3(), Vector3()};

	Frustum () {
        if(!created){
            int j = 0;
            for (Vector3 v : clipSpacePlanePoints) {
                clipSpacePlanePointsArray[j++] = v.x;
                clipSpacePlanePointsArray[j++] = v.y;
                clipSpacePlanePointsArray[j++] = v.z;
            }
            created = true;
        }
		for (int i = 0; i < 6; i++) {
			planes[i] = Plane(Vector3(), 0);
		}
	}

	/** Updates the clipping plane's based on the given inverse combined projection and view matrix, e.g. from an
	 * {@link OrthographicCamera} or {@link PerspectiveCamera}.
	 * @param inverseProjectionView the combined projection and view matrices. */
	void update (const Matrix4& inverseProjectionView) {
        planePointsArray = clipSpacePlanePointsArray;
		Matrix4::proj(inverseProjectionView.val, planePointsArray);
		for (int i = 0, j = 0; i < 8; i++) {
			Vector3 v = planePoints[i];
			v.x = planePointsArray[j++];
			v.y = planePointsArray[j++];
			v.z = planePointsArray[j++];
		}

		planes[0].set(planePoints[1], planePoints[0], planePoints[2]);
		planes[1].set(planePoints[4], planePoints[5], planePoints[7]);
		planes[2].set(planePoints[0], planePoints[4], planePoints[3]);
		planes[3].set(planePoints[5], planePoints[1], planePoints[6]);
		planes[4].set(planePoints[2], planePoints[3], planePoints[6]);
		planes[5].set(planePoints[4], planePoints[0], planePoints[1]);
	}

	/** Returns whether the point is in the frustum.
	 * 
	 * @param point The point
	 * @return Whether the point is in the frustum. */
	bool pointInFrustum (const Vector3& point) {
		for (int i = 0; i < planes.size(); i++) {
			PlaneSide result = planes[i].testPoint(point);
			if (result == Back) return false;
		}
		return true;
	}

	/** Returns whether the point is in the frustum.
	 * 
	 * @param x The X coordinate of the point
	 * @param y The Y coordinate of the point
	 * @param z The Z coordinate of the point
	 * @return Whether the point is in the frustum. */
	bool pointInFrustum (float x, float y, float z) {
		for (int i = 0; i < planes.size(); i++) {
			PlaneSide result = planes[i].testPoint(x, y, z);
			if (result == Back) return false;
		}
		return true;
	}

	/** Returns whether the given sphere is in the frustum.
	 * 
	 * @param center The center of the sphere
	 * @param radius The radius of the sphere
	 * @return Whether the sphere is in the frustum */
	bool sphereInFrustum (const Vector3& center, float radius) {
		for (int i = 0; i < 6; i++)
			if ((planes[i].normal.x * center.x + planes[i].normal.y * center.y + planes[i].normal.z * center.z) < (-radius - planes[i].d))
				return false;
		return true;
	}

	/** Returns whether the given sphere is in the frustum.
	 * 
	 * @param x The X coordinate of the center of the sphere
	 * @param y The Y coordinate of the center of the sphere
	 * @param z The Z coordinate of the center of the sphere
	 * @param radius The radius of the sphere
	 * @return Whether the sphere is in the frustum */
	bool sphereInFrustum (float x, float y, float z, float radius) {
		for (int i = 0; i < 6; i++)
			if ((planes[i].normal.x * x + planes[i].normal.y * y + planes[i].normal.z * z) < (-radius - planes[i].d)) return false;
		return true;
	}

	/** Returns whether the given sphere is in the frustum not checking whether it is behind the near and far clipping plane.
	 * 
	 * @param center The center of the sphere
	 * @param radius The radius of the sphere
	 * @return Whether the sphere is in the frustum */
	bool sphereInFrustumWithoutNearFar (const Vector3& center, float radius) {
		for (int i = 2; i < 6; i++)
			if ((planes[i].normal.x * center.x + planes[i].normal.y * center.y + planes[i].normal.z * center.z) < (-radius - planes[i].d))
				return false;
		return true;
	}

	/** Returns whether the given sphere is in the frustum not checking whether it is behind the near and far clipping plane.
	 * 
	 * @param x The X coordinate of the center of the sphere
	 * @param y The Y coordinate of the center of the sphere
	 * @param z The Z coordinate of the center of the sphere
	 * @param radius The radius of the sphere
	 * @return Whether the sphere is in the frustum */
	bool sphereInFrustumWithoutNearFar (float x, float y, float z, float radius) {
		for (int i = 2; i < 6; i++)
			if ((planes[i].normal.x * x + planes[i].normal.y * y + planes[i].normal.z * z) < (-radius - planes[i].d)) return false;
		return true;
	}

	/** Returns whether the given {@link BoundingBox} is in the frustum.
	 * 
	 * @param bounds The bounding box
	 * @return Whether the bounding box is in the frustum */
	bool boundsInFrustum (BoundingBox& bounds) {
		for (int i = 0, len2 = planes.size(); i < len2; i++) {
			if (planes[i].testPoint(bounds.getCorner000(tmpV)) != Back) continue;
			if (planes[i].testPoint(bounds.getCorner001(tmpV)) != Back) continue;
			if (planes[i].testPoint(bounds.getCorner010(tmpV)) != Back) continue;
			if (planes[i].testPoint(bounds.getCorner011(tmpV)) != Back) continue;
			if (planes[i].testPoint(bounds.getCorner100(tmpV)) != Back) continue;
			if (planes[i].testPoint(bounds.getCorner101(tmpV)) != Back) continue;
			if (planes[i].testPoint(bounds.getCorner110(tmpV)) != Back) continue;
			if (planes[i].testPoint(bounds.getCorner111(tmpV)) != Back) continue;
			return false;
		}

		return true;
	}

	/** Returns whether the given bounding box is in the frustum.
	 * @return Whether the bounding box is in the frustum */
	bool boundsInFrustum (const Vector3& center,const Vector3& dimensions) {
		return boundsInFrustum(center.x, center.y, center.z, dimensions.x / 2, dimensions.y / 2, dimensions.z / 2);
	}

	/** Returns whether the given bounding box is in the frustum.
	 * @return Whether the bounding box is in the frustum */
	bool boundsInFrustum (float x, float y, float z, float halfWidth, float halfHeight, float halfDepth) {
		for (int i = 0, len2 = planes.size(); i < len2; i++) {
			if (planes[i].testPoint(x + halfWidth, y + halfHeight, z + halfDepth) != Back) continue;
			if (planes[i].testPoint(x + halfWidth, y + halfHeight, z - halfDepth) != Back) continue;
			if (planes[i].testPoint(x + halfWidth, y - halfHeight, z + halfDepth) != Back) continue;
			if (planes[i].testPoint(x + halfWidth, y - halfHeight, z - halfDepth) != Back) continue;
			if (planes[i].testPoint(x - halfWidth, y + halfHeight, z + halfDepth) != Back) continue;
			if (planes[i].testPoint(x - halfWidth, y + halfHeight, z - halfDepth) != Back) continue;
			if (planes[i].testPoint(x - halfWidth, y - halfHeight, z + halfDepth) != Back) continue;
			if (planes[i].testPoint(x - halfWidth, y - halfHeight, z - halfDepth) != Back) continue;
			return false;
		}

		return true;
	}

// /**
// * Calculates the pick ray for the given window coordinates. Assumes the window coordinate system has it's y downwards. The
// * returned Ray is a member of this instance so don't reuse it outside this class.
// *
// * @param screen_width The window width in pixels
// * @param screen_height The window height in pixels
// * @param mouse_x The window x-coordinate
// * @param mouse_y The window y-coordinate
// * @param pos The camera position
// * @param dir The camera direction, having unit length
// * @param up The camera up vector, having unit length
// * @return the picking ray.
// */
// public Ray calculatePickRay (float screen_width, float screen_height, float mouse_x, float mouse_y, Vector3 pos, Vector3 dir,
// Vector3 up) {
// float n_x = mouse_x - screen_width / 2.0f;
// float n_y = mouse_y - screen_height / 2.0f;
// n_x /= screen_width / 2.0f;
// n_y /= screen_height / 2.0f;
//
// Z.set(dir.tmp().mul(-1)).nor();
// X.set(up.tmp().crs(Z)).nor();
// Y.set(Z.tmp().crs(X)).nor();
// near_center.set(pos.tmp3().sub(Z.tmp2().mul(near)));
// Vector3 near_point = X.tmp3().mul(near_width).mul(n_x).add(Y.tmp2().mul(near_height).mul(n_y));
// near_point.add(near_center);
//
// return ray.set(near_point.tmp(), near_point.sub(pos).nor());
// }

// public static void main(String[] argv) {
// PerspectiveCamera camera = new PerspectiveCamera(45, 2, 2);
// // camera.rotate(90, 0, 1, 0);
// camera.update();
// System.out.println(camera.direction);
// System.out.println(Arrays.toString(camera.frustum.planes));
//
// OrthographicCamera camOrtho = new OrthographicCamera(2, 2);
// camOrtho.near = 1;
// // camOrtho.rotate(90, 1, 0, 0);
// camOrtho.update();
// System.out.println(camOrtho.direction);
// System.out.println(Arrays.toString(camOrtho.frustum.planes));
// }
};
