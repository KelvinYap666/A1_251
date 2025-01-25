#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

/*------------------------------------------------------------------

THIS CPP FILE CONTAINS CALCULATIONS FOR THE AVG CLOUD COVER AND PRESSURE

--------------------------------------------------------------------*/

// Function to display the average cloud cover for all cities
double calculateAverageCloudCover(int cityID, string** cityGrid, string** cloudGrid, int rows, int cols) {
    int sum = 0;     // Total sum of cloud coverage values
    int count = 0;   // Total number of grids considered

    // A helper 2D vector to track visited cells and avoid double-counting

    //check grid value for bool T/F if visited
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            // Adjust row index for flipped grid orientation
            int flippedRow = rows - row - 1;

            // Process only cells belonging to the given City ID
            if (!cityGrid[flippedRow][col].empty() && stoi(cityGrid[flippedRow][col]) == cityID) {
                // Mark the center cell as visited and include it
                if (!visited[flippedRow][col]) {
                    sum += stoi(cloudGrid[flippedRow][col]);
                    count++;
                    visited[flippedRow][col] = true; // Mark as visited
                }

                // Process surrounding cells (8 directions)
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        int newRow = flippedRow + dr;
                        int newCol = col + dc;

                        // Skip out-of-bounds cells
                        if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols) continue;

                        // Include surrounding cells only if not already visited
                        if (!visited[newRow][newCol]) {
                            sum += stoi(cloudGrid[newRow][newCol]);
                            count++;
                            visited[newRow][newCol] = true; // Mark as visited
                        }
                    }
                }
            }
        }
    }

    return count > 0 ? static_cast<double>(sum) / count : 0.0;
}

// call getNumCities to init integer and collect value
int getNumCities(string** cityGrid, int rows, int cols) {
    unordered_set<int> cityIDs;

    // Iterate over the grid to collect unique city IDs STOI
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (!cityGrid[row][col].empty()) {
                cityIDs.insert(stoi(cityGrid[row][col])); // Convert string to integer and add to the set
            }
        }
    }

    return cityIDs.size(); // Return the count of unique city IDs
}

double calculateAveragePressure(int cityID, string** cityGrid, string** pressureGrid, int rows, int cols) {
    int sum = 0;     // Total sum of pressure values
    int count = 0;   // Total number of grids considered

    // A helper 2D vector to track visited cells and avoid double-counting
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            // Adjust row index for flipped grid orientation
            int flippedRow = rows - row - 1;

            // Process only cells belonging to the given City ID
            if (!cityGrid[flippedRow][col].empty() && stoi(cityGrid[flippedRow][col]) == cityID) {
                // Mark the center cell as visited and include it
                if (!visited[flippedRow][col]) {
                    sum += stoi(pressureGrid[flippedRow][col]);
                    count++;
                    visited[flippedRow][col] = true; // Mark as visited
                }

                // Process surrounding cells
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        int newRow = flippedRow + dr;
                        int newCol = col + dc;

                        // Skip out-of-bounds cells
                        if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols) continue;

                        // Include surrounding cells only if not already visited
                        if (!visited[newRow][newCol]) {
                            sum += stoi(pressureGrid[newRow][newCol]);
                            count++;
                            visited[newRow][newCol] = true; // Mark as visited
                        }
                    }
                }
            }
        }
    }

    // return value
    return count > 0 ? static_cast<double>(sum) / count : 0.0;
}