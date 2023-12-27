#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

struct Point {
    double x, y;

    Point(double _x, double _y) : x(_x), y(_y) {}
};

// Function to draw a line on the grid
void drawLine(std::vector<std::vector<char>>& grid, const Point& p1, const Point& p2, char symbol) {
    int x1 = static_cast<int>(p1.x);
    int y1 = static_cast<int>(p1.y);
    int x2 = static_cast<int>(p2.x);
    int y2 = static_cast<int>(p2.y);

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (x1 >= 0 && x1 < grid[0].size() && y1 >= 0 && y1 < grid.size()) {
            grid[y1][x1] = symbol;
        }

        if (x1 == x2 && y1 == y2) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Function to find the intersection of line segments
std::vector<Point> intersectionOfPolygons(const std::vector<std::vector<Point>>& polygons) {
    if (polygons.size() < 2) {
        // Intersection is not defined for less than two polygons
        return {};
    }

    // Perform intersection by finding the intersection of line segments
    std::vector<Point> intersectionResult;

    for (size_t i = 0; i < polygons.size(); ++i) {
        const auto& polygonA = polygons[i];
        const auto& polygonB = polygons[(i + 1) % polygons.size()];

        for (size_t j = 0; j < polygonA.size(); ++j) {
            const auto& p1A = polygonA[j];
            const auto& p2A = polygonA[(j + 1) % polygonA.size()];

            for (size_t k = 0; k < polygonB.size(); ++k) {
                const auto& p1B = polygonB[k];
                const auto& p2B = polygonB[(k + 1) % polygonB.size()];

                // Check if line segments intersect
                double det = (p2B.x - p1B.x) * (p2A.y - p1A.y) - (p2A.x - p1A.x) * (p2B.y - p1B.y);
                if (std::abs(det) < 1e-9) {
                    // Line segments are parallel or colinear, no intersection
                    continue;
                }

                double s = ((p2B.x - p1B.x) * (p1B.y - p1A.y) - (p2B.y - p1B.y) * (p1B.x - p1A.x)) / det;
                double t = ((p2A.x - p1A.x) * (p1B.y - p1A.y) - (p2A.y - p1A.y) * (p1B.x - p1A.x)) / det;

                if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
                    // Intersection point found
                    double x = p1A.x + t * (p2A.x - p1A.x);
                    double y = p1A.y + t * (p2A.y - p1A.y);
                    intersectionResult.emplace_back(x, y);
                }
            }
        }
    }

    return intersectionResult;
}

// Function to find the union of multiple polygons
std::vector<Point> unionOfPolygons(const std::vector<std::vector<Point>>& polygons) {
    std::vector<Point> mergedVertices;

    // Merge all vertices from all polygons
    for (const auto& polygon : polygons) {
        mergedVertices.insert(mergedVertices.end(), polygon.begin(), polygon.end());
    }

    // Sort vertices based on polar angle
    std::sort(mergedVertices.begin(), mergedVertices.end(), [](const Point& p1, const Point& p2) {
        return std::atan2(p1.y, p1.x) < std::atan2(p2.y, p2.x);
    });

    // Remove duplicate vertices
    auto last = std::unique(mergedVertices.begin(), mergedVertices.end(),
                            [](const Point& p1, const Point& p2) {
                                return std::abs(p1.x - p2.x) < 1e-9 && std::abs(p1.y - p2.y) < 1e-9;
                            });

    mergedVertices.erase(last, mergedVertices.end());

    return mergedVertices;
}

// Function to find the difference of two sets of polygons
std::vector<Point> differenceOfPolygons(const std::vector<Point>& polygonA, const std::vector<Point>& polygonB) {
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

// Function to visualize the grid
void visualizeGrid(const std::vector<std::vector<char>>& grid) {
    for (const auto& row : grid) {
        for (char cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << '\n';
    }
}

int main() {
    // Ask the user if they want to provide their own polygons or use default polygons
    std::cout << "Do you want to provide your own polygons? (yes/no): ";
    std::string userChoice;
    std::cin >> userChoice;

    // Initialize vectors to store polygons
    std::vector<std::vector<Point>> polygons;

    if (userChoice == "yes") {
        // Get information for each polygon from the user
        std::cout << "\nEnter the total number of polygons (N): ";
        int totalPolygons;
        std::cin >> totalPolygons;

        // Validate input
        if (totalPolygons < 2) {
            std::cerr << "Error: At least two polygons are required.\n";
            return 1;
        }

        for (int i = 0; i < totalPolygons; ++i) {
            std::cout << "\nEnter the number of vertices for Polygon " << i + 1 << ": ";
            int numVertices;
            std::cin >> numVertices;

            // Validate input
            if (numVertices < 3) {
                std::cerr << "Error: At least three vertices are required for a polygon.\n";
                return 1;
            }

            // Initialize vector to store vertices of the current polygon
            std::vector<Point> currentPolygon;

            // Get coordinates for each vertex of the current polygon
            for (int j = 0; j < numVertices; ++j) {
                double x, y;
                std::cout << "Enter x and y coordinates for Vertex " << j + 1 << ": ";
                std::cin >> x >> y;
                currentPolygon.emplace_back(x, y);
            }

            // Add the current polygon to the list
            polygons.push_back(currentPolygon);
        }
    } else {
        // Set default polygons
        polygons.push_back({{0, 0}, {0, 5}, {5, 5}, {5, 0}});       // 4-sided polygon
        polygons.push_back({{2, 2}, {2, 7}, {7, 2}});              // 3-sided polygon
        polygons.push_back({{10, 10}, {8, 15}, {12, 18}, {15, 15}, {13, 10}});  // 5-sided polygon
    }

    // Visualize input polygons
    const int gridSize = 20;
    std::vector<std::vector<char>> inputGrid(gridSize, std::vector<char>(gridSize, '.'));

    for (const auto& polygon : polygons) {
        for (size_t i = 0; i < polygon.size(); ++i) {
            const auto& p1 = polygon[i];
            const auto& p2 = polygon[(i + 1) % polygon.size()];
            drawLine(inputGrid, p1, p2, '@'); // Use '@' to represent input polygons
        }
    }

    // Visualize input polygons
    std::cout << "\nInput Polygons:\n";
    visualizeGrid(inputGrid);

    // Perform intersection
    std::vector<Point> intersectionResult = intersectionOfPolygons(polygons);

    // Visualize the intersection result
    std::vector<std::vector<char>> intersectionGrid(gridSize, std::vector<char>(gridSize, '.'));
    for (const auto& p : intersectionResult) {
        intersectionGrid[static_cast<int>(p.y)][static_cast<int>(p.x)] = '*'; // Use '*' to represent output polygons
    }
    std::cout << "\nIntersection Result:\n";
    visualizeGrid(intersectionGrid);

    // Perform union
    std::vector<Point> unionResult = unionOfPolygons(polygons);

    // Visualize the union result
    std::vector<std::vector<char>> unionGrid(gridSize, std::vector<char>(gridSize, '.'));
    for (size_t i = 0; i < unionResult.size(); ++i) {
        const auto& p1 = unionResult[i];
        const auto& p2 = unionResult[(i + 1) % unionResult.size()];
        drawLine(unionGrid, p1, p2, '*'); // Use '*' to represent output polygons
    }
    std::cout << "\nUnion Result:\n";
    visualizeGrid(unionGrid);

    // Perform difference
    std::vector<Point> differenceResult = differenceOfPolygons(polygons[0], polygons[1]);

    // Visualize the difference result
    std::vector<std::vector<char>> differenceGrid(gridSize, std::vector<char>(gridSize, '.'));
    for (size_t i = 0; i < differenceResult.size(); ++i) {
        const auto& p1 = differenceResult[i];
        const auto& p2 = differenceResult[(i + 1) % differenceResult.size()];
        drawLine(differenceGrid, p1, p2, '+'); // Use '+' to represent output polygons
    }
    std::cout << "\nDifference Result:\n";
    visualizeGrid(differenceGrid);

    return 0;
}
