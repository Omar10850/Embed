#pragma once
#include <vector>
#include <Eigen/Dense>

struct SplineSegment {
    double a, b, c, d; // Coefficients
    double x_start, x_end; // Interval bounds
};

class SplineInterpolation {
public:
    static void calculateSplineSegments(const std::vector<double>& xValues, const std::vector<double>& yValues, std::vector<SplineSegment>& splineSegments);
    static void plotSplineInterpolation(const std::vector<double>& xValues, const std::vector<double>& yValues, const std::vector<SplineSegment>& splineSegments, const std::string& xAxisLabel, const std::string& yAxisLabel);
};

