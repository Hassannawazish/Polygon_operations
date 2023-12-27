#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <omp.h> 

#include "Point.h"  
#include "LineDrawing.h"
#include "PolygonValidation.h"
#include "Intersection.h"
#include "Union.h"
#include "Difference.h"
#include "GridVisualization.h"

int main() {
    try {
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
                throw std::invalid_argument("Error: At least two polygons are required.");
            }

            for (int i = 0; i < totalPolygons; ++i) {
                std::cout << "\nEnter the number of vertices for Polygon " << i + 1 << ": ";
                int numVertices;
                std::cin >> numVertices;

                // Validate input
                if (numVertices < 3) {
                    throw std::invalid_argument("Error: At least three vertices are required for a polygon.");
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

                // Validate polygon vertices
                validatePolygonVertices(currentPolygon);

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
        if (intersectionResult.empty()) {
            std::cout << "\nNo intersection.\n";
        } else {
            std::vector<std::vector<char>> intersectionGrid(gridSize, std::vector<char>(gridSize, '.'));
            for (const auto& p : intersectionResult) {
                intersectionGrid[static_cast<int>(p.y)][static_cast<int>(p.x)] = '*'; // Use '*' to represent output polygons
            }
            std::cout << "\nIntersection Result:\n";
            visualizeGrid(intersectionGrid);
        }

        // Perform union
        std::vector<Point> unionResult = unionOfPolygons(polygons);

        // Visualize the union result
        if (unionResult.empty()) {
            std::cout << "\nNo union.\n";
        } else {
            std::vector<std::vector<char>> unionGrid(gridSize, std::vector<char>(gridSize, '.'));
            for (size_t i = 0; i < unionResult.size(); ++i) {
                const auto& p1 = unionResult[i];
                const auto& p2 = unionResult[(i + 1) % unionResult.size()];
                drawLine(unionGrid, p1, p2, '*'); // Use '*' to represent output polygons
            }
            std::cout << "\nUnion Result:\n";
            visualizeGrid(unionGrid);
        }

        // Perform difference
        std::vector<Point> differenceResult = differenceOfPolygons(polygons[0], polygons[1]);

        // Visualize the difference result
        if (differenceResult.empty()) {
            std::cout << "\nNo difference.\n";
        } else {
            std::vector<std::vector<char>> differenceGrid(gridSize, std::vector<char>(gridSize, '.'));
            for (size_t i = 0; i < differenceResult.size(); ++i) {
                const auto& p1 = differenceResult[i];
                const auto& p2 = differenceResult[(i + 1) % differenceResult.size()];
                drawLine(differenceGrid, p1, p2, '+'); // Use '+' to represent output polygons
            }
            std::cout << "\nDifference Result:\n";
            visualizeGrid(differenceGrid);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
