/*-------------------------------------------------------------------------

THIS CPP FILE CONTAIN THE WEATHER REPORT CALCULATION AND PRINTING OF IT

--------------------------------------------------------------------------*/

//weather report function

void WeatherReport(
    //declare var
    int numCities, 
    string** cityGrid, 
    string** rawCloudGrid, 
    string** rawPressureGrid, 
    const unordered_map<int, string>& cityNames, 
    int gridHeight, 
    int gridWidth
) {
    for (int cityID = 1; cityID <= numCities; ++cityID) {
        // Cloud Coverage
        double avgCloudCover = calculateAverageCloudCover(cityID, cityGrid, rawCloudGrid, gridHeight, gridWidth);

        // Pressure
        double avgPressure = calculateAveragePressure(cityID, cityGrid, rawPressureGrid, gridHeight, gridWidth);

        // City Name
        string cityName = cityNames.count(cityID) > 0 ? cityNames.at(cityID) : "Unknown";

        if (avgCloudCover >= 0 && avgCloudCover <= 100 && avgPressure >= 0 && avgPressure <= 100) {
            // Determine LMH classification for cloud cover
            string cloudCoverLMH = (avgCloudCover <= 34) ? "L" : (avgCloudCover <= 64) ? "M" : "H";

            // Determine LMH classification for pressure
            string pressureLMH = (avgPressure <= 34) ? "L" : (avgPressure <= 64) ? "M" : "H";

            // Determine probability of rain using chart
            int rainProbability = 0;
            string asciiRain;

            if (pressureLMH == "L" && cloudCoverLMH == "H") {
                rainProbability = 90;
                asciiRain = "~~~~\n~~~~~\n\\\\\\\\\\";
            } else if (pressureLMH == "L" && cloudCoverLMH == "M") {
                rainProbability = 80;
                asciiRain = "~~~~\n~~~~~\n\\\\\\\\";
            } else if (pressureLMH == "L" && cloudCoverLMH == "L") {
                rainProbability = 70;
                asciiRain = "~~~~\n~~~~~\n\\\\\\";
            } else if (pressureLMH == "M" && cloudCoverLMH == "H") {
                rainProbability = 60;
                asciiRain = "~~~~\n~~~~~\n  \\\\";
            } else if (pressureLMH == "M" && cloudCoverLMH == "M") {
                rainProbability = 50;
                asciiRain = "~~~~\n~~~~~\n    \\";
            } else if (pressureLMH == "M" && cloudCoverLMH == "L") {
                rainProbability = 40;
                asciiRain = "~~~~\n~~~~~";
            } else if (pressureLMH == "H" && cloudCoverLMH == "H") {
                rainProbability = 30;
                asciiRain = "~~~\n~~~~~";
            } else if (pressureLMH == "H" && cloudCoverLMH == "M") {
                rainProbability = 20;
                asciiRain = "~~\n~~~~";
            } else if (pressureLMH == "H" && cloudCoverLMH == "L") {
                rainProbability = 10;
                asciiRain = "~\n~~";
            }

            // Output
            cout << "City Name: " << cityName << endl;
            cout << "City ID: " << cityID << endl;
            cout << "Average Cloud Coverage: " << fixed << setprecision(2) << avgCloudCover 
                 << " (" << cloudCoverLMH << ")" << endl;
            cout << "Average Pressure: " << fixed << setprecision(2) << avgPressure 
                 << " (" << pressureLMH << ")" << endl;
            cout << "Estimated Probability of Rain (%): " << rainProbability << endl;
            cout << asciiRain << endl;
            cout << endl;
        }
    }
}
