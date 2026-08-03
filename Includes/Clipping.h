#pragma once


#include "../Math/Includes/Transform.h"
#include "Geometry.h"


//Sutherland-Hodgman clipping algorithm
void ClipPolygon(Polygon& polygon);

//Clip test in clip space, returns dot product
float ClipAgainstPlane(const math::vec4& vertex, unsigned int plane);

