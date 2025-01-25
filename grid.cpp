#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

//include calc.cpp and report.cpp
#include "calc.cpp"
#include "weather_report.cpp"

using namespace std;
/*----------------------------------------------------------------

THIS CPP FILE CONTAINS GRID VALUES FOR MENU.CPP
AND READ IN FILE INPUT FOR THE GRID

------------------------------------------------------------------*/


// Global variables for grid ranges and grid dimensions
pair<int, int> gridXRange = {0, 0};
pair<int, int> gridYRange = {0, 0};
int gridWidth, gridHeight;
string cityLocationFile;
string cloudCoverFile;
string pressureFile;
std::unordered_map<int, std::string> cityNames;

// Function to parse the range from a string
pair<int, int> parseRange(const string& line) {
    size_t equalPos = line.find('=');
    if (equalPos == string::npos) throw runtime_error("Invalid configuration line: " + line);
    string rangePart = line.substr(equalPos + 1);
    size_t dashPos = rangePart.find('-');
    if (dashPos == string::npos) throw runtime_error("Invalid range format: " + rangePart);
    return {stoi(rangePart.substr(0, dashPos)), stoi(rangePart.substr(dashPos + 1))};
}

// Function to process the configuration file and extract grid ranges
bool processConfigFile() {
    string cfgfile;
    cout << "[Read in and process a configuration file]" << endl;
    cout << "Please enter config filename: ";
    cin >> cfgfile;

    ifstream file(cfgfile);
    if (!file.is_open()) {
        cout << "File not found. Please check the filename.\n\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.find("GridX_IdxRange") != string::npos) {
            gridXRange = parseRange(line);
        } else if (line.find("GridY_IdxRange") != string::npos) {
            gridYRange = parseRange(line);
        } else if (line.find("citylocation") != string::npos) {
            cityLocationFile = line.substr(line.find('=') + 1);  // Capture the filename from config
        } else if (line.find("cloudcover") != string ::npos) {
            cloudCoverFile = line.substr(line.find('=') +1); 
        }else if (line.find("pressure") != string ::npos) {
            pressureFile = line.substr(line.find('=') +1); 
        } 
    }
    file.close();

    // Setting the grid dimensions
    gridWidth = gridXRange.second - gridXRange.first + 1;
    gridHeight = gridYRange.second - gridYRange.first + 1;

    // print out file read name
    cout << "Reading in GridX_IdxRange: " << gridXRange.first << " - " << gridXRange.second << "... Done!" << endl;
    cout << "Reading in GridY_IdxRange: " << gridYRange.first << " - " << gridYRange.second << "... Done!" << endl;
    cout << "Storing data from input file: ..." << endl;
    cout << "City location file: " << cityLocationFile << endl;
    cout << "cloudcover file: " << cloudCoverFile << endl;
    cout << "pressure file: " << pressureFile << endl;
    cout << endl;

    return true;

}

// Function to initialize the grid as a 2D array
void initializeGrid(string**& grid) {
    // Allocate the 2D array (grid)
    grid = new string*[gridHeight];
    for (int i = 0; i < gridHeight; i++) {
        grid[i] = new string[gridWidth];
    }
    // Initialize the grid with default values
    for (int i = 0; i < gridHeight; i++) {
        for (int j = 0; j < gridWidth; j++) {
            grid[i][j] = ""; 
        }
    }
}

// Function to display the grid with row and column indices
void displayGrid(string** grid) {
    // Print the top border with column indices
    cout << "     ";  // Extra space for row indices alignment
    for (int j = 0; j < gridWidth; j++) {
        // Print column indices, make them align to a fixed width
        cout << setw(4) << j;  // Adjusted width
    }
    cout << endl;

    // Print the top border with #
    cout << "     ";
    for (int i = 0; i < gridWidth; i++) {
        cout << "####";  // Horizontal border for columns
    }
    cout << endl;

    // Print the grid rows with left and right borders and row numbers
    for (int i = 0; i < gridHeight; i++) {
        // Print row index starting from the bottom, and align it
        cout << setw(3) << (gridHeight - i - 1) << " #";  // Left-aligned row numbers
        for (int j = 0; j < gridWidth; j++) {
            // Print grid content with consistent cell width
            cout << setw(4) << grid[i][j];  // Adjusted cell width
        }
        cout << " #" << endl;  // Right border for the row
    }

    // Print the bottom border
    cout << "     ";
    for (int i = 0; i < gridWidth; i++) {
        cout << "# # ";  // Horizontal border for columns
    }
    cout << endl;
}

// Function to process citylocation and update the grid
void processCityLocationFile(const std::string& filename, string** cityGrid) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open city location file." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        // Parse the line to extract coordinates, city ID, and city name
        size_t openBracket = line.find('[');
        size_t comma = line.find(',', openBracket);
        size_t closeBracket = line.find(']', comma);
        size_t dash1 = line.find('-', closeBracket);
        size_t dash2 = line.find('-', dash1 + 1);

        if (openBracket == string::npos || comma == string::npos || closeBracket == string::npos ||
            dash1 == string::npos || dash2 == string::npos) {
            cerr << "Warning: Invalid line format skipped: " << line << endl;
            continue;
        }

        try {
            // Extract coordinates
            int x = stoi(line.substr(openBracket + 1, comma - openBracket - 1));
            int y = stoi(line.substr(comma + 1, closeBracket - comma - 1));

            // Extract city ID and city name
            int cityID = stoi(line.substr(dash1 + 1, dash2 - dash1 - 1));
            string cityName = line.substr(dash2 + 1);
            cityName.erase(cityName.find_last_not_of(" \t") + 1); // Trim trailing whitespace
            cityName.erase(0, cityName.find_first_not_of(" \t")); // Trim leading whitespace

            // Populate the cityGrid with the city ID
            int gridX = x - gridXRange.first;
            int gridY = gridHeight - (y - gridYRange.first) - 1; // Flip y-axis for grid
            if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
                cityGrid[gridY][gridX] = to_string(cityID);
            }

            // Populate the cityNames map
            cityNames[cityID] = cityName;
          //catch error exception
        } catch (const exception& e) {
            cerr << "Error processing line: " << line << " (" << e.what() << ")" << endl;
        }
    }

    file.close();
}
//cloudcover file grid process
void processCloudFile(const string& filename, string** grid, string** rawGrid) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open cloud cover file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t openBracket = line.find('[');
        size_t comma = line.find(',', openBracket);
        size_t closeBracket = line.find(']', comma);
        size_t dash1 = line.find('-', closeBracket);

        if (openBracket == string::npos || comma == string::npos || closeBracket == string::npos || dash1 == string::npos) {
            cerr << "Warning: Invalid line format skipped: " << line << endl;
            continue;
        }
        //try block to store cloudgrid value
        try {
            int x = stoi(line.substr(openBracket + 1, comma - openBracket - 1));
            int y = stoi(line.substr(comma + 1, closeBracket - comma - 1));
            string count = line.substr(dash1 + 1, line.find('-', dash1 + 1) - dash1 - 1);

            int gridX = x - gridXRange.first;
            int gridY = gridHeight - (y - gridYRange.first) - 1;

            if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
                rawGrid[gridY][gridX] = count;  // Store the raw value
                grid[gridY][gridX] = to_string(stoi(count) / 10); // Store the processed value
            }
        } catch (const exception& e) {
            cerr << "Error processing line: " << line << " (" << e.what() << ")" << endl;
        }
    }
    file.close();
}

// function of cloudcover to LMH
void processCloudFileLMH(const string& filename, string** grid) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open cloud cover file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t openBracket = line.find('[');
        size_t comma = line.find(',', openBracket);
        size_t closeBracket = line.find(']', comma);
        size_t dash1 = line.find('-', closeBracket);

        if (openBracket == string::npos || comma == string::npos || closeBracket == string::npos || dash1 == string::npos) {
            cerr << "Warning: Invalid line format skipped: " << line << endl;
            continue;
        }

        try {
            int x = stoi(line.substr(openBracket + 1, comma - openBracket - 1));
            int y = stoi(line.substr(comma + 1, closeBracket - comma - 1));
            int count = stoi(line.substr(dash1 + 1, line.find('-', dash1 + 1) - dash1 - 1));

            int gridX = x - gridXRange.first;
            int gridY = gridHeight - (y - gridYRange.first) - 1;

            if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
                // Convert the cloud cover value into "L", "M", or "H"
                if (count <= 35) {
                    grid[gridY][gridX] = "L";  // Low cloud cover
                } else if (count <= 65) {
                    grid[gridY][gridX] = "M";  // Medium cloud cover
                } else {
                    grid[gridY][gridX] = "H";  // High cloud cover
                }
            }
        } catch (const exception& e) {
            cerr << "Error processing line: " << line << " (" << e.what() << ")" << endl;
        }
    }
    file.close();
}
//process pressure.txt file
void processPressureFile(const string& filename, string** grid, string** rawGrid) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open cloud cover file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t openBracket = line.find('[');
        size_t comma = line.find(',', openBracket);
        size_t closeBracket = line.find(']', comma);
        size_t dash1 = line.find('-', closeBracket);

        if (openBracket == string::npos || comma == string::npos || closeBracket == string::npos || dash1 == string::npos) {
            cerr << "Warning: Invalid line format skipped: " << line << endl;
            continue;
        }

        try {
            int x = stoi(line.substr(openBracket + 1, comma - openBracket - 1));
            int y = stoi(line.substr(comma + 1, closeBracket - comma - 1));
            string count = line.substr(dash1 + 1, line.find('-', dash1 + 1) - dash1 - 1);

            int gridX = x - gridXRange.first;
            int gridY = gridHeight - (y - gridYRange.first) - 1;

            if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
                rawGrid[gridY][gridX] = count;  // Store the raw value
                grid[gridY][gridX] = to_string(stoi(count) / 10); // Store the processed value
            }
        } catch (const exception& e) {
            cerr << "Error processing line: " << line << " (" << e.what() << ")" << endl;
        }
    }
    file.close();
}
// function of pressure to LMH
void processPressureFileLMH(const string& filename, string** grid) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open cloud cover file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t openBracket = line.find('[');
        size_t comma = line.find(',', openBracket);
        size_t closeBracket = line.find(']', comma);
        size_t dash1 = line.find('-', closeBracket);

        if (openBracket == string::npos || comma == string::npos || closeBracket == string::npos || dash1 == string::npos) {
            cerr << "Warning: Invalid line format skipped: " << line << endl;
            continue;
        }

        try {
            int x = stoi(line.substr(openBracket + 1, comma - openBracket - 1));
            int y = stoi(line.substr(comma + 1, closeBracket - comma - 1));
            int count = stoi(line.substr(dash1 + 1, line.find('-', dash1 + 1) - dash1 - 1));

            int gridX = x - gridXRange.first;
            int gridY = gridHeight - (y - gridYRange.first) - 1;

            if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
                // Convert the cloud cover value into "L", "M", or "H"
                if (count <= 35) {
                    grid[gridY][gridX] = "L";  // Low cloud cover
                } else if (count <= 65) {
                    grid[gridY][gridX] = "M";  // Medium cloud cover
                } else {
                    grid[gridY][gridX] = "H";  // High cloud cover
                }
            }
        } catch (const exception& e) {
            cerr << "Error processing line: " << line << " (" << e.what() << ")" << endl;
        }
    }
    file.close();
}

// Function to deallocate the grid memory row by row 
void deleteGrid(string**& grid) {
    // Free the memory used for each row
    if (grid != nullptr) {
        for (int i = 0; i < gridHeight; i++) {
            delete[] grid[i];
        }
        delete[] grid;
        grid = nullptr;
    }
}