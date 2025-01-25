#include <iostream>
#include <string>
#include <sstream>

//include grid.cpp
#include "grid.cpp"

using namespace std;

/*-------------------------------------------------------------

THIS FILE CONTAINS INT MAIN AND MENU SELECTION FUNCTION

---------------------------------------------------------------*/

//hold the selected till enter is press
void waitForEnter() {
    cout << "Press Enter to return to the selection menu..." << endl;
    cin.ignore(); 
    cin.get();     // Wait for the user to press Enter
}

//menu selection
void selection() {
    int choice;
    bool in = false;
    string** cityGrid= nullptr;
    string** cloudGrid=nullptr;
    string** pressureGrid=nullptr;
    string** rawCloudGrid = nullptr;
    string** rawPressureGrid=nullptr;



    while (true) {
        cout << "Student ID: 8750762" << endl;
        cout << "Student Name: Kelvin Yap" << endl;
        cout << "------------------------------------" << endl;
        cout << "Welcome to Weather Information Processing System" << endl;
        cout << "1) Read in and process a configuration file" << endl;
        if (in) {
            cout << "2) Display city map" << endl;
            cout << "3) Display cloud coverage map (cloudiness index)" << endl;
            cout << "4) Display cloud coverage map (LMH symbols)" << endl;
            cout << "5) Display atmospheric pressure map (pressure index)" << endl;
            cout << "6) Display atmospheric pressure map (LMH symbols)" << endl;
            cout << "7) Show weather forecast summary report" << endl;
            cout << "8) Exit" << endl;
        }
        cout << "Please enter your choice: ";
        cin >> choice;
        // choice 1 process file
        if (choice == 1) {
            in = processConfigFile();
            if(in){
                initializeGrid(cityGrid);
                processCityLocationFile(cityLocationFile,cityGrid);
                initializeGrid(cloudGrid);
                initializeGrid(rawCloudGrid);
                processCloudFile(cloudCoverFile,cloudGrid,rawCloudGrid);
                initializeGrid(rawPressureGrid);
                initializeGrid(pressureGrid);
                processPressureFile(pressureFile,pressureGrid,rawPressureGrid);
            }
        // choice 2 display city map grid
        } else if (choice == 2 && in) {
            cout << "Displaying city map..." << endl;
            cout << endl;
            displayGrid(cityGrid);
            cout << endl;
            waitForEnter();
        // choice 3 display cloud coverage
        } else if (choice == 3 && in){
            cout << "Displaying cloud coverage map (cloudiness index) " << endl;
            cout << endl;
            processCloudFile(cloudCoverFile,cloudGrid,rawCloudGrid);
            displayGrid(cloudGrid);
            waitForEnter();
        }
        //choice 4 display cloud lmh
        else if (choice == 4 && in){
            cout << "Displaying cloud coverage map (LMH symbols)" << endl;
            cout << endl;
            processCloudFileLMH(cloudCoverFile,cloudGrid);
            displayGrid(cloudGrid);
            waitForEnter();
        }
        //choice 5 display atmosphere pressure index
        else if (choice == 5 && in){
            cout << "Displaying atmospheric pressure map (pressure index) " <<endl;
            cout << endl;
            processPressureFile(pressureFile,pressureGrid,rawPressureGrid);
            displayGrid(pressureGrid);
            waitForEnter();
        }
        //choice 6 display atmosphere LMH
        else if (choice == 6 && in){
            cout << "Displaying atmospheric pressure map (LMH symbols) " << endl;
            cout << endl;
            processPressureFileLMH(pressureFile,pressureGrid);
            displayGrid(pressureGrid);
            waitForEnter();
        }
        //choice 7 display weather report 
        else if (choice == 7 && in){
            cout << "Weather Forecast Summary Report" <<  endl;
            cout << "----------------------------------" << endl;
            int numCities = getNumCities(cityGrid, gridHeight, gridWidth);
            WeatherReport(numCities,cityGrid,rawCloudGrid,rawPressureGrid,cityNames,gridHeight,gridWidth);
            cout << endl;
            waitForEnter();
        }
        // quit program and delete grid
        else if (choice == 8) {
            cout << "Exiting program..." << endl;
            deleteGrid(cityGrid);
            deleteGrid(cloudGrid);
            deleteGrid(rawCloudGrid);
            deleteGrid(pressureGrid);
            deleteGrid(rawPressureGrid);
            break;
        // invalid choice
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

// int main
int main() {
    selection();    
    return 0;
}