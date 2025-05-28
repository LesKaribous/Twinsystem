#pragma once
#include "utils/geometry.h"
#include "services/lidar/occupancy.h"
#include <vector>

std::vector<Vec2> computePath(const Vec2& start, const Vec2& goal){};
std::vector<Vec2> simplifyPath(const std::vector<Vec2>& input);

