#include "Frustum.h"

Vector3 Frustum::tmpV = Vector3();
std::vector<float> Frustum::clipSpacePlanePointsArray = std::vector<float>(8 * 3);
const std::vector<Vector3> Frustum::clipSpacePlanePoints = {Vector3(-1, -1, -1), Vector3(1, -1, -1),
		Vector3(1, 1, -1), Vector3(-1, 1, -1), // near clip
		Vector3(-1, -1, 1), Vector3(1, -1, 1), Vector3(1, 1, 1), Vector3(-1, 1, 1)}; // far clip
bool Frustum::created = false;