#include <cstddef> 

#include "Difference.h"
#include "PolygonValidation.h"

std::vector<Point> differenceOfPolygons(const std::vector<Point>& polygonA, const std::vector<Point>& polygonB) {
    validatePolygonVertices(polygonA);
    validatePolygonVertices(polygonB);

    std::vector<Point> result;

    for (size_t i = 0; i < polygonA.size(); ++i) {
        const auto& p1A = polygonA[i];
        const auto& p2A = polygonA[(i + 1) % polygonA.size()];

        bool inside = true;

        for (size_t j = 0; j < polygonB.size(); ++j) {
            const auto& pB = polygonB[j];
            const auto& pNextB = polygonB[(j + 1) % polygonB.size()];

            double det = (pNextB.x - pB.x) * (p1A.y - pB.y) - (p1A.x - pB.x) * (pNextB.y - pB.y);
            if (det < 0) {
                inside = false;
                break;
            }
        }

        if (inside) {
            result.push_back(p1A);
        }
    }

    return result;
}
