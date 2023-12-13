#include "LinearInterpolation1.h"
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

std::pair<double, double> LinearInterpolation::calculateCoefficients(const std::vector<double>& xValues, const std::vector<double>& yValues) {
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    int n = xValues.size();

    for (int i = 0; i < n; ++i) {
        sumX += xValues[i];
        sumY += yValues[i];
        sumXY += xValues[i] * yValues[i];
        sumX2 += xValues[i] * xValues[i];
    }

    double m = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double c = (sumY - m * sumX) / n;

    return { m, c };
}

void LinearInterpolation::plotLinearInterpolation(const std::vector<double>& xValues, const std::vector<double>& yValues, const std::pair<double, double>& coefficients, const std::string& xAxisLabel, const std::string& yAxisLabel) {
    std::ofstream outFile("Linear_Interpolation_Data.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file for writing.\n";
        return;
    }

    for (size_t i = 0; i < xValues.size(); i++) {
        outFile << xValues[i] << " " << yValues[i] << " " << (coefficients.first * xValues[i] + coefficients.second) << std::endl;
    }
    outFile.close();

    std::string command = "gnuplot -p -e \"";
    command += "set xlabel '" + xAxisLabel + "'; ";
    command += "set ylabel '" + yAxisLabel + "'; ";
    command += "set title 'Linear Interpolation'; ";
    command += "plot 'Linear_Interpolation_Data.txt' using 1:2 with points title 'Original Data', ";
    command += "'Linear_Interpolation_Data.txt' using 1:3 with lines title 'Linear Fit'\"";
    system(command.c_str());
}
