#include "PolygonValidation.h"
#include <stdexcept>
#include <algorithm>

bool validatePolygonVertices(const std::vector<Point>& polygon) {
    if (polygon.size() < 3) {
        throw std::invalid_argument("Error: At least three vertices are required for a polygon.");
    }

    for (size_t i = 0; i < polygon.size(); ++i) {
        for (size_t j = i + 1; j < polygon.size(); ++j) {
            if (std::abs(polygon[i].x - polygon[j].x) < 1e-9 && std::abs(polygon[i].y - polygon[j].y) < 1e-9) {
                throw std::invalid_argument("Error: Duplicate vertices found in the polygon.");
            }
        }
    }

    return true;
}
