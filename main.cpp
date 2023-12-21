#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>


struct Point {
    double x, y;

    Point(double _x, double _y) : x(_x), y(_y) {}
};

// Function to draw a line on the grid
void drawLine(std::vector<std::vector<char>>& grid, const Point& p1, const Point& p2) {
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
            grid[y1][x1] = '@'; // Use '#' to represent the line
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

// Function to perform the union of multiple polygons
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
    // Define polygons
    std::vector<std::vector<Point>> polygons;

    // Polygon with 4 sides
    polygons.push_back({{0, 0}, {0, 5}, {5, 5}, {5, 0}});

    // Polygon with 3 sides
    polygons.push_back({{2, 2}, {2, 7}, {7, 2}});

    // Polygon with 5 sides
    polygons.push_back({{10, 10}, {8, 15}, {12, 18}, {15, 15}, {13, 10}});

    // Perform union
    std::vector<Point> result = unionOfPolygons(polygons);

    // Create a grid for visualization
    const int gridSize = 20;
    std::vector<std::vector<char>> grid(gridSize, std::vector<char>(gridSize, '.'));

    // Draw polygons on the grid
    for (const auto& polygon : polygons) {
        for (size_t i = 0; i < polygon.size(); ++i) {
            const auto& p1 = polygon[i];
            const auto& p2 = polygon[(i + 1) % polygon.size()];
            drawLine(grid, p1, p2);
        }
    }

    // Draw the result polygon on the grid
    for (size_t i = 0; i < result.size(); ++i) {
        const auto& p1 = result[i];
        const auto& p2 = result[(i + 1) % result.size()];
        drawLine(grid, p1, p2);
    }

    // Visualize the grid
    visualizeGrid(grid);

    return 0;
}
