#include "PolynomialInterpolation1.h"
#include <fstream>
#include <iostream>
#include <cmath>

Eigen::VectorXd PolynomialInterpolation::calculatePolynomialCoefficients(const std::vector<double>& xValues, const std::vector<double>& yValues) {
    int n = xValues.size();
    Eigen::MatrixXd V(n, n);
    Eigen::VectorXd y(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            V(i, j) = std::pow(xValues[i], j);
        }
        y(i) = yValues[i];
    }

    return V.colPivHouseholderQr().solve(y);
}

void PolynomialInterpolation::plotPolynomialInterpolation(const Eigen::VectorXd& coefficients, const std::vector<double>& xValues, const std::vector<double>& yValues, const std::string& xAxisLabel, const std::string& yAxisLabel) {
    std::ofstream outFile("Polynomial_Interpolation_Data.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file for writing.\n";
        return;
    }

    for (size_t i = 0; i < xValues.size(); ++i) {
        outFile << xValues[i] << " " << yValues[i] << std::endl;
    }

    double xMin = *std::min_element(xValues.begin(), xValues.end());
    double xMax = *std::max_element(xValues.begin(), xValues.end());
    for (double x = xMin; x <= xMax; x += (xMax - xMin) / 1000.0) {
        double y = 0.0;
        for (int i = 0; i < coefficients.size(); ++i) {
            y += coefficients[i] * std::pow(x, i);
        }
        outFile << x << " " << y << std::endl;
    }
    outFile.close();

    std::string command = "gnuplot -p -e \"";
    command += "set xlabel '" + xAxisLabel + "'; ";
    command += "set ylabel '" + yAxisLabel + "'; ";
    command += "set title 'Polynomial Interpolation'; ";
    command += "plot 'Polynomial_Interpolation_Data.txt' using 1:2 with points title 'Original Data', ";
    command += "'Polynomial_Interpolation_Data.txt' using 1:2 with lines title 'Polynomial Fit'\"";
    system(command.c_str());
}
