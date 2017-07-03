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

/** A camera with orthographic projection.
 * 
 * @author mzechner */
class OrthographicCamera:public Camera {
    Vector3 tmp;
public:
	/** the zoom of the camera **/
	float zoom = 1;

	OrthographicCamera () {
		near = 0;
	}

	/** Constructs a new OrthographicCamera, using the given viewport width and height. For pixel perfect 2D rendering just supply
	 * the screen size, for other unit scales (e.g. meters for box2d) proceed accordingly. The camera will show the region
	 * [-viewportWidth/2, -(viewportHeight/2-1)] - [(viewportWidth/2-1), viewportHeight/2]
	 * @param viewportWidth the viewport width
	 * @param viewportHeight the viewport height */
	OrthographicCamera (float viewportWidth, float viewportHeight) {
		this->viewportWidth = viewportWidth;
		this->viewportHeight = viewportHeight;
		near = 0;
		update();
	}

	void update () {
		update(true);
	}

	void update (bool updateFrustum) {
		projection.setToOrtho(zoom * -viewportWidth / 2, zoom * (viewportWidth / 2), zoom * -(viewportHeight / 2), zoom
			* viewportHeight / 2, near, far);
		view.setToLookAt(position, tmp.set(position).add(direction), up);
		combined.set(projection);
		Matrix4::mul(combined.val, view.val);

		if (updateFrustum) {
			invProjectionView.set(combined);
			Matrix4::inv(invProjectionView.val);
			frustum.update(invProjectionView);
		}
	}

	/** Sets this camera to an orthographic projection using a viewport fitting the screen resolution, centered at
	 * (Gdx.graphics.getWidth()/2, Gdx.graphics.getHeight()/2), with the y-axis pointing up or down.
	 * @param yDown whether y should be pointing down */
	/*void setToOrtho (bool yDown) {
		setToOrtho(yDown, Gdx.graphics.getWidth(), Gdx.graphics.getHeight());
	}*/

	/** Sets this camera to an orthographic projection, centered at (viewportWidth/2, viewportHeight/2), with the y-axis pointing up
	 * or down.
	 * @param yDown whether y should be pointing down.
	 * @param viewportWidth
	 * @param viewportHeight */
	void setToOrtho (bool yDown, float viewportWidth, float viewportHeight) {
		if (yDown) {
			up.set(0, -1, 0);
			direction.set(0, 0, 1);
		} else {
			up.set(0, 1, 0);
			direction.set(0, 0, -1);
		}
		position.set(zoom * viewportWidth / 2.0f, zoom * viewportHeight / 2.0f, 0);
		this->viewportWidth = viewportWidth;
		this->viewportHeight = viewportHeight;
		update();
	}

	/** Rotates the camera by the given angle around the direction vector. The direction and up vector will not be orthogonalized.
	 * @param angle */
	void rotate (float angle) {
		Camera::rotate(direction, angle);
	}

	/** Moves the camera by the given amount on each axis.
	 * @param x the displacement on the x-axis
	 * @param y the displacement on the y-axis */
	void translate (float x, float y) {
		Camera::translate(x, y, 0);
	}

	/** Moves the camera by the given vector.
	 * @param vec the displacement vector */
	void translate (Vector2 vec) {
		Camera::translate(vec.x, vec.y, 0);
	}
};


