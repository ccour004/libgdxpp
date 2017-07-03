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
 #include "Camera.h"

/** A Camera with perspective projection.
 * 
 * @author mzechner */
class PerspectiveCamera: public Camera {
    public:
	/** the field of view of the height, in degrees **/
	float fieldOfView = 67;

	PerspectiveCamera () {
	}

	/** Constructs a new {@link PerspectiveCamera} with the given field of view and viewport size. The aspect ratio is derived from
	 * the viewport size.
	 * 
	 * @param fieldOfViewY the field of view of the height, in degrees, the field of view for the width will be calculated
	 *           according to the aspect ratio.
	 * @param viewportWidth the viewport width
	 * @param viewportHeight the viewport height */
	PerspectiveCamera (float fieldOfViewY, float viewportWidth, float viewportHeight) {
		this->fieldOfView = fieldOfViewY;
		this->viewportWidth = viewportWidth;
		this->viewportHeight = viewportHeight;
		update();
	}

	Vector3 tmp;

	void update () {
		update(true);
	}

	void update (bool updateFrustum) {
		float aspect = viewportWidth / viewportHeight;
		projection.setToProjection(abs(near), abs(far), fieldOfView, aspect);
		view.setToLookAt(position, tmp.set(position).add(direction), up);
		combined.set(projection);
		Matrix4::mul(combined.val, view.val);

		if (updateFrustum) {
			invProjectionView.set(combined);
			Matrix4::inv(invProjectionView.val);
			frustum.update(invProjectionView);
		}
        //SDL_Log("FRUSTUM: %s",frustum.toString().c_str());
        //SDL_Log("POS: %s,DIR: %s,UP: %s,PROJ: %s,VIEW: %s,COMB: %s,INV_PROJ: %s",
        //    position.toString().c_str(),direction.toString().c_str(),up.toString().c_str(),
        //    projection.toString().c_str(),view.toString().c_str(),combined.toString().c_str(),invProjectionView.toString().c_str());
	}
};