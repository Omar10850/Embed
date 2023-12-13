#include "SplineInterpolation1.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <cmath>

void SplineInterpolation::calculateSplineSegments(const std::vector<double>& xValues, const std::vector<double>& yValues, std::vector<SplineSegment>& splineSegments) {
    int n = xValues.size() - 1;
    Eigen::VectorXd h(n), alpha(n), l(n + 1), mu(n), z(n + 1), c(n + 1), b(n), d(n);
    Eigen::VectorXd a = Eigen::Map<const Eigen::VectorXd, Eigen::Unaligned>(yValues.data(), yValues.size());

    for (int i = 0; i < n; ++i) {
        h(i) = xValues[i + 1] - xValues[i];
        if (i > 0) {
            alpha(i) = (3.0 / h(i)) * (a(i + 1) - a(i)) - (3.0 / h(i - 1)) * (a(i) - a(i - 1));
        }
    }

    l(0) = 1.0;
    mu(0) = z(0) = 0.0;
    for (int i = 1; i < n; ++i) {
        l(i) = 2.0 * (xValues[i + 1] - xValues[i - 1]) - h(i - 1) * mu(i - 1);
        mu(i) = h(i) / l(i);
        z(i) = (alpha(i) - h(i - 1) * z(i - 1)) / l(i);
    }

    l(n) = 1.0;
    z(n) = c(n) = 0.0;

    for (int j = n - 1; j >= 0; --j) {
        c(j) = z(j) - mu(j) * c(j + 1);
        b(j) = (a(j + 1) - a(j)) / h(j) - h(j) * (c(j + 1) + 2.0 * c(j)) / 3.0;
        d(j) = (c(j + 1) - c(j)) / (3.0 * h(j));
    }

    splineSegments.clear();
    for (int i = 0; i < n; ++i) {
        SplineSegment segment;
        segment.a = a(i);
        segment.b = b(i);
        segment.c = c(i);
        segment.d = d(i);
        segment.x_start = xValues[i];
        segment.x_end = xValues[i + 1];
        splineSegments.push_back(segment);
    }
}

void SplineInterpolation::plotSplineInterpolation(const std::vector<double>& xValues, const std::vector<double>& yValues, const std::vector<SplineSegment>& splineSegments, const std::string& xAxisLabel, const std::string& yAxisLabel) {
    std::ofstream outFile("Spline_Interpolation_Data.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file for writing.\n";
        return;
    }

    for (size_t i = 0; i < xValues.size(); ++i) {
        outFile << xValues[i] << " " << yValues[i] << std::endl;
    }

    for (const auto& segment : splineSegments) {
        for (double x = segment.x_start; x <= segment.x_end; x += (segment.x_end - segment.x_start) / 100.0) {
            double dx = x - segment.x_start;
            double y = segment.a + segment.b * dx + segment.c * dx * dx + segment.d * dx * dx * dx;
            outFile << x << " " << y << std::endl;
        }
    }
    outFile.close();

    std::string command = "gnuplot -p -e \"";
    command += "set xlabel '" + xAxisLabel + "'; ";
    command += "set ylabel '" + yAxisLabel + "'; ";
    command += "set title 'Spline Interpolation'; ";
    command += "plot 'Spline_Interpolation_Data.txt' using 1:2 with points title 'Original Data', ";
    command += "'Spline_Interpolation_Data.txt' using 1:2 with lines title 'Spline Fit'\"";
    system(command.c_str());
}
