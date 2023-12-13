#pragma once#pragma once
#include <vector>

class LinearInterpolation {
public:
    static std::pair<double, double> calculateCoefficients(const std::vector<double>& xValues, const std::vector<double>& yValues);
    static void plotLinearInterpolation(const std::vector<double>& xValues, const std::vector<double>& yValues, const std::pair<double, double>& coefficients, const std::string& xAxisLabel, const std::string& yAxisLabel);
};

