#include "Union.h"
#include "PolygonValidation.h"
#include <algorithm>

std::vector<Point> unionOfPolygons(const std::vector<std::vector<Point>>& polygons) {
    for (const auto& polygon : polygons) {
        validatePolygonVertices(polygon);
    }

    std::vector<Point> mergedVertices;

    for (const auto& polygon : polygons) {
        mergedVertices.insert(mergedVertices.end(), polygon.begin(), polygon.end());
    }

    std::sort(mergedVertices.begin(), mergedVertices.end(), [](const Point& p1, const Point& p2) {
        return std::atan2(p1.y, p1.x) < std::atan2(p2.y, p2.x);
    });

    auto last = std::unique(mergedVertices.begin(), mergedVertices.end(),
                            [](const Point& p1, const Point& p2) {
                                return std::abs(p1.x - p2.x) < 1e-9 && std::abs(p1.y - p2.y) < 1e-9;
                            });

    mergedVertices.erase(last, mergedVertices.end());

    return mergedVertices;
}
