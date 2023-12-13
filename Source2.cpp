#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    // Create a file to store sine wave data
    std::ofstream dataFile("sine_wave_data.txt");
    if (!dataFile.is_open()) {
        std::cerr << "Error: Unable to open file for writing.\n";
        return 1;
    }

    // Generate sine wave data
    for (double x = 0; x <= 2 * M_PI; x += 0.1) {
        double y = std::sin(x);
        dataFile << x << " " << y << std::endl;
    }

    dataFile.close();

    // Use Gnuplot to plot the data
    std::string command = "gnuplot -p -e \"plot 'sine_wave_data.txt' with lines\"";
    system(command.c_str());

    return 0;
}
