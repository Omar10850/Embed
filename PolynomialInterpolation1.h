#include <vector>
#include <Eigen/Dense>
#include <string>

class PolynomialInterpolation {
public:
    static Eigen::VectorXd calculatePolynomialCoefficients(const std::vector<double>& xValues, const std::vector<double>& yValues);
    static void plotPolynomialInterpolation(const Eigen::VectorXd& coefficients, const std::vector<double>& xValues, const std::vector<double>& yValues, const std::string& xAxisLabel, const std::string& yAxisLabel);
};

