#include <cstddef>
#include <cmath>
#include <algorithm>
#include <omp.h>

#include "Intersection.h"
#include "PolygonValidation.h"

std::vector<Point> intersectionOfPolygons(const std::vector<std::vector<Point>>& polygons) {
    if (polygons.size() < 2) {
        return {};
    }

    for (const auto& polygon : polygons) {
        validatePolygonVertices(polygon);
    }

    std::vector<Point> intersectionResult;

    // Use OpenMP to parallelize the loop for larger polygons
    #pragma omp parallel for if(polygons.size() > 10) schedule(dynamic) shared(intersectionResult)
    for (size_t i = 0; i < polygons.size(); ++i) {
        const auto& polygonA = polygons[i];
        for (size_t j = i + 1; j < polygons.size(); ++j) {
            const auto& polygonB = polygons[j];

            for (size_t k = 0; k < polygonA.size(); ++k) {
                const auto& p1A = polygonA[k];
                const auto& p2A = polygonA[(k + 1) % polygonA.size()];

                for (size_t l = 0; l < polygonB.size(); ++l) {
                    const auto& p1B = polygonB[l];
                    const auto& p2B = polygonB[(l + 1) % polygonB.size()];

                    double det = (p2B.x - p1B.x) * (p2A.y - p1A.y) - (p2A.x - p1A.x) * (p2B.y - p1B.y);
                    if (std::abs(det) < 1e-9) {
                        continue;
                    }

                    double s = ((p2B.x - p1B.x) * (p1B.y - p1A.y) - (p2B.y - p1B.y) * (p1B.x - p1A.x)) / det;
                    double t = ((p2A.x - p1A.x) * (p1B.y - p1A.y) - (p2A.y - p1A.y) * (p1B.x - p1A.x)) / det;

                    if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
                        double x = p1A.x + t * (p2A.x - p1A.x);
                        double y = p1A.y + t * (p2A.y - p1A.y);

                        auto pointExists = [&](const Point& point) {
                            return std::any_of(intersectionResult.begin(), intersectionResult.end(),
                                               [&](const Point& existing) {
                                                   return std::abs(existing.x - point.x) < 1e-9 &&
                                                          std::abs(existing.y - point.y) < 1e-9;
                                               });
                        };

                        #pragma omp critical
                        if (!pointExists(Point(x, y))) {
                            intersectionResult.emplace_back(x, y);
                        }
                    }
                }
            }
        }
    }

    return intersectionResult;
}
