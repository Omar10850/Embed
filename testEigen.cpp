
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <iomanip>
#include <functional>

int getValidatedInput(int min, int max);
std::vector<double> calculateYFit(const std::vector<double>& xData, const std::vector<double>& yData);


struct StockData {
    std::string date;
    double open, high, low, close;

    double getValueByColumn(int column) const;
};

double StockData::getValueByColumn(int column) const {
    switch (column) {
    case 1: return open;
    case 2: return high;
    case 3: return low;
    case 4: return close;
    default: return 0.0;
    }
}

std::vector<StockData> btcData = {
    {"11/1/2023", 34651, 38024, 34137, 37589},
    {"10/1/2023", 27072, 35077, 26570, 34651},
    {"9/1/2023", 26002, 27474, 24971, 27072},
    {"8/1/2023", 29180, 30176, 25469, 25996},
    {"7/1/2023", 30379, 31638, 28952, 29180},
    {"6/1/2023", 27120, 31383, 24801, 30379},
    {"5/1/2023", 29348, 29983, 25883, 27120},
    {"4/1/2023", 28392, 30976, 27039, 29348},
    {"3/1/2023", 23142, 29140, 19594, 28392},
    {"2/1/2023", 22950, 25193, 21437, 23159},
    {"1/1/2023", 16549, 23940, 16491, 22950},
    {"12/1/2022", 17102, 18338, 16300, 16549},
    {"11/1/2022", 20407, 21486, 15508, 17102},
    {"10/1/2022", 19425, 21020, 18271, 20407},
    {"9/1/2022", 20207, 22733, 18189, 19420},
    {"8/1/2022", 23804, 25199, 19544, 20207},
    {"7/1/2022", 18731, 24613, 18706, 23804},
    {"6/1/2022", 31757, 31957, 17630, 18731},
    {"5/1/2022", 38332, 39890, 25805, 31757},
    {"4/1/2022", 45760, 47367, 37746, 38332},
    {"3/1/2022", 41650, 48174, 37187, 45760},
    {"2/1/2022", 38444, 45758, 34413, 41650},
    {"1/1/2022", 46338, 47946, 33070, 38444},
    {"12/1/2021", 57144, 59093, 42169, 46322},
    {"11/1/2021", 60968, 68925, 53577, 57144},
};

std::vector<StockData> sp500Data = {
    {"11/1/2023", 4201.27, 4587.64, 4197.74, 4550.58},
    {"10/1/2023", 4284.52, 4393.57, 4103.78, 4193.8},
    {"9/1/2023", 4530.85, 4541.25, 4238.63, 4288.05},
    {"8/1/2023", 4578.83, 4584.62, 4335.31, 4507.66},
    {"7/1/2023", 4449.45, 4607.07, 4385.05, 4588.96},
    {"6/1/2023", 4183.03, 4458.48, 4171.64, 4450.38},
    {"5/1/2023", 4166.79, 4231.1, 4048.28, 4179.83},
    {"4/1/2023", 4102.2, 4170.06, 4049.35, 4169.48},
    {"3/1/2023", 3963.34, 4110.75, 3808.86, 4109.31},
    {"2/1/2023", 4070.07, 4195.44, 3943.08, 3970.15},
    {"1/1/2023", 3853.29, 4094.21, 3794.33, 4076.6},
    {"12/1/2022", 4087.14, 4100.96, 3764.49, 3839.5},
    {"11/1/2022", 3901.79, 4080.11, 3698.15, 4080.11},
    {"10/1/2022", 3609.78, 3905.42, 3491.58, 3871.98},
    {"9/1/2022", 3936.73, 4119.28, 3584.13, 3585.62},
    {"8/1/2022", 4112.38, 4325.28, 3954.53, 3955},
    {"7/1/2022", 3781.08, 4140.15, 3721.56, 4130.29},
    {"6/1/2022", 4149.78, 4177.51, 3636.87, 3785.38},
    {"5/1/2022", 4130.61, 4307.66, 3810.32, 4132.15},
    {"4/1/2022", 4540.32, 4593.45, 4124.28, 4131.93},
    {"3/1/2022", 4363.14, 4637.3, 4157.87, 4530.41},
    {"2/1/2022", 4519.57, 4595.31, 4114.65, 4373.94},
    {"1/1/2022", 4778.14, 4818.62, 4222.62, 4515.55},
    {"12/1/2021", 4602.82, 4808.93, 4495.12, 4766.18},
    {"11/1/2021", 4610.62, 4743.83, 4560, 4567},
};

struct ManualInputData {
    std::string xColumnName, yColumnName;
    std::vector<double> xValues, yValues, yFitValues; // yFitValues to be calculated later
};

struct SplineSegment {
    double a, b, c, d; // Coefficients
    double x_start, x_end; // Interval bounds
};

enum class DataSource {
    None,
    CSV,
    Manual
};

// Function declarations

void splineInterpolation(const std::vector<double>& xValues, const std::vector<double>& yValues);
void linearInterpolationEquation(const std::vector<double>& xValues, const std::vector<double>& yValues);
void interpolationTool(DataSource source, const std::vector<StockData>& stockData, const ManualInputData& manualData);
void saveCSV(const std::string& filename, const std::vector<StockData>& stockData);
void showCSVTable(const std::vector<StockData>& stockData);
void showManualInputTable(const ManualInputData& data);
void insertManualData(ManualInputData& data);
Eigen::VectorXd polynomialInterpolation(const std::vector<double>& xValues, const std::vector<double>& yValues, int degree);


// New function to get column choices from the user
std::pair<int, int> getColumnChoices() {
    int xChoice, yChoice;
    std::cout << "Select the column for X axis:\n1. Open\n2. High\n3. Low\n4. Close\n";
    xChoice = getValidatedInput(1, 4);

    std::cout << "Select the column for Y axis:\n";
    yChoice = getValidatedInput(1, 4);

    return { xChoice, yChoice };
}

// Modify your existing showCSVTable function to also include the Y-Fit calculation
void showSelectedDataWithYFit(const std::vector<StockData>& stockData, int xColumn, int yColumn) {
    // Extract the selected columns
    std::vector<double> xValues, yValues;
    for (const auto& entry : stockData) {
        xValues.push_back(entry.getValueByColumn(xColumn));
        yValues.push_back(entry.getValueByColumn(yColumn));
    }

    std::vector<double> yFitValues = calculateYFit(xValues, yValues); // You need to implement this function

    // Display the table with X, Y, and Y-Fit values
    std::cout << "X\tY\tY-Fit\n";
    for (size_t i = 0; i < xValues.size(); ++i) {
        std::cout << xValues[i] << "\t" << yValues[i] << "\t" << yFitValues[i] << "\n";
    }
}


double parsePrice(const std::string& priceStr);

std::vector<double> calculateYFit(const std::vector<double>& xData, const std::vector<double>& yData) {
    std::vector<double> yFitValues;

    if (xData.size() < 2 || xData.size() != yData.size()) {
        std::cerr << "Error: Not enough data points or data size mismatch." << std::endl;
        return yFitValues;
    }

    for (size_t i = 0; i < xData.size() - 1; ++i) {
        double x1 = xData[i];
        double y1 = yData[i];
        double x2 = xData[i + 1];
        double y2 = yData[i + 1];
        double yFit = y1 + ((xData[i] - x1) / (x2 - x1)) * (y2 - y1);
        yFitValues.push_back(yFit);
    }

    // Handling the last point
    yFitValues.push_back(yData.back());

    return yFitValues;
}

void insertManualData(ManualInputData& data) {
    // Get column names
    std::cout << "Enter column name for X values: ";
    std::cin >> data.xColumnName;
    std::cout << "Enter column name for Y values: ";
    std::cin >> data.yColumnName;

    // Get X values
    std::cout << "Enter X values (end with a non-numeric value): ";
    double valueX;
    while (std::cin >> valueX) {
        data.xValues.push_back(valueX);
    }
    // Clear error state and buffer
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Get Y values
    std::cout << "Enter Y values (end with a non-numeric value): ";
    double valueY;
    while (std::cin >> valueY) {
        data.yValues.push_back(valueY);
    }
    // Clear error state and buffer
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    data.yFitValues = calculateYFit(data.xValues, data.yValues);

}

void showManualInputTable(const ManualInputData& data) {
    const int columnWidth = 15; // Adjust the width as needed

    // Display the headers
    std::cout << std::left << std::setw(columnWidth) << data.xColumnName
        << std::setw(columnWidth) << data.yColumnName
        << std::setw(columnWidth) << "Y-Fit" << std::endl;

    // Display the data
    for (size_t i = 0; i < data.xValues.size(); ++i) {
        std::cout << std::left
            << std::setw(columnWidth) << std::fixed << std::setprecision(2) << data.xValues[i]
            << std::setw(columnWidth) << std::fixed << std::setprecision(2) << data.yValues[i];

        if (i < data.yFitValues.size()) {
            std::cout << std::setw(columnWidth) << std::fixed << std::setprecision(2) << data.yFitValues[i];
        }
        else {
            std::cout << std::setw(columnWidth) << "N/A";
        }

        std::cout << std::endl;
    }
}

void showCSVTable(const std::vector<StockData>& stockData) {
    std::cout << std::left
        << std::setw(12) << "DATE"
        << std::setw(12) << "OPEN"
        << std::setw(12) << "HIGH"
        << std::setw(12) << "LOW"
        << std::setw(12) << "CLOSE"
        << std::endl;

    for (const auto& entry : stockData) {
        std::cout << std::left
            << std::setw(12) << entry.date
            << std::setw(12) << entry.open
            << std::setw(12) << entry.high
            << std::setw(12) << entry.low
            << std::setw(12) << entry.close
            << std::endl;
    }
}

double parsePrice(const std::string& priceStr) {
    std::string number = priceStr;
    number.erase(std::remove(number.begin(), number.end(), ','), number.end());
    number.erase(std::remove(number.begin(), number.end(), '$'), number.end());
    try {
        return std::stod(number);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid number format: " << number << std::endl;
        return 0.0;
    }
}

std::function<double(double)> createSplineFunction(const std::vector<SplineSegment>& splineSegments) {
    return [&splineSegments](double x) {
        for (const auto& segment : splineSegments) {
            if (x >= segment.x_start && x <= segment.x_end) {
                double dx = x - segment.x_start;
                return segment.a + segment.b * dx + segment.c * dx * dx + segment.d * dx * dx * dx;
            }
        }
        throw std::runtime_error("x value is out of the bounds of the spline segments.");
        };
}


// Function to load data from a CSV file
void loadCSV(const std::string& filename, std::vector<StockData>& stockData) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Skip the header line

    while (std::getline(file, line)) {
        std::stringstream linestream(line);
        std::string date, open, high, low, close;

        std::getline(linestream, date, ',');
        std::getline(linestream, open, ',');
        std::getline(linestream, high, ',');
        std::getline(linestream, low, ',');
        std::getline(linestream, close, ',');

        StockData data;
        data.date = date;
        data.open = parsePrice(open);
        data.high = parsePrice(high);
        data.low = parsePrice(low);
        data.close = parsePrice(close);

        stockData.push_back(data);
    }

    file.close();
    std::cout << "CSV file loaded successfully.\n";
}

int getValidatedInput(int min, int max) {
    int input;
    while (true) {
        std::cin >> input;
        if (std::cin.fail() || input < min || input > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        }
        else {
            return input;
        }
    }
}

void splineInterpolation(const std::vector<double>& xValues, const std::vector<double>& yValues, std::vector<SplineSegment>& splineSegments) {
    int n = xValues.size() - 1;

    // Initialize vectors
    Eigen::VectorXd h(n), alpha(n), l(n + 1), mu(n), z(n + 1), c(n + 1), b(n), d(n);
    Eigen::VectorXd a = Eigen::Map<const Eigen::VectorXd, Eigen::Unaligned>(yValues.data(), yValues.size());

    // Compute h and alpha
    for (int i = 0; i < n; ++i) {
        h(i) = xValues[i + 1] - xValues[i];
        if (i > 0) {
            alpha(i) = (3.0 / h(i)) * (a(i + 1) - a(i)) - (3.0 / h(i - 1)) * (a(i) - a(i - 1));
        }
    }

    // Compute l, mu, and z
    l(0) = 1.0;
    mu(0) = z(0) = 0.0;
    for (int i = 1; i < n; ++i) {
        l(i) = 2.0 * (xValues[i + 1] - xValues[i - 1]) - h(i - 1) * mu(i - 1);
        mu(i) = h(i) / l(i);
        z(i) = (alpha(i) - h(i - 1) * z(i - 1)) / l(i);
    }

    // Set the last values of l, z, and c
    l(n) = 1.0;
    z(n) = c(n) = 0.0;

    // Back substitution loop for the c, b, and d vectors
    for (int j = n - 1; j >= 0; --j) {
        c(j) = z(j) - mu(j) * c(j + 1);
        b(j) = (a(j + 1) - a(j)) / h(j) - h(j) * (c(j + 1) + 2.0 * c(j)) / 3.0;
        d(j) = (c(j + 1) - c(j)) / (3.0 * h(j));
    }

    // Store coefficients and interval bounds
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

double evaluateUnifiedSpline(double x, const std::vector<SplineSegment>& splineSegments) {
    for (const auto& segment : splineSegments) {
        if (x >= segment.x_start && x <= segment.x_end) {
            double dx = x - segment.x_start;
            return segment.a + segment.b * dx + segment.c * dx * dx + segment.d * dx * dx * dx;
        }
    }
    throw std::runtime_error("x value is out of the bounds of the spline segments.");
}


Eigen::VectorXd polynomialInterpolation(const std::vector<double>& xValues, const std::vector<double>& yValues) {
    int n = xValues.size();
    Eigen::MatrixXd V(n, n);
    Eigen::VectorXd y(n);

    // Create the Vandermonde matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            V(i, j) = std::pow(xValues[i], j);
        }
        y(i) = yValues[i];
    }

    // Solve for coefficients
    Eigen::VectorXd coefficients = V.colPivHouseholderQr().solve(y);
    return coefficients;
}

double evaluatePolynomial(const Eigen::VectorXd& coefficients, double x) {
    double result = 0.0;
    for (int i = 0; i < coefficients.size(); ++i) {
        result += coefficients[i] * std::pow(x, i);
    }
    return result;
}

void performPolynomialInterpolation(const std::vector<double>& xValues, const std::vector<double>& yValues) {
    if (xValues.size() != yValues.size() || xValues.size() < 2) {
        std::cerr << "Insufficient data points or mismatch in sizes of x and y values." << std::endl;
        return;
    }

    Eigen::VectorXd coefficients = polynomialInterpolation(xValues, yValues);

    std::cout << "Polynomial coefficients:" << std::endl;
    for (int i = 0; i < coefficients.size(); ++i) {
        std::cout << "Coefficient of x^" << i << ": " << coefficients[i] << std::endl;
    }

    std::cout << "Polynomial Interpolation Equation:" << std::endl;
    std::cout << "P(x) = ";
    for (int i = 0; i < coefficients.size(); ++i) {
        if (i != 0) {
            std::cout << " + ";
        }
        std::cout << std::fixed << std::setprecision(4) << coefficients[i];
        if (i == 1) {
            std::cout << "x";
        }
        else if (i > 1) {
            std::cout << "x^" << i;
        }
    }
    std::cout << std::endl;
}

void polynomialInterpolationTool(const std::vector<double>& xValues, const std::vector<double>& yValues) {
    std::cout << "Debug: Starting Polynomial Interpolation Tool" << std::endl;
    std::cout << "Debug: Number of data points = " << xValues.size() << std::endl;

    if (xValues.size() < 2 || xValues.size() != yValues.size()) {
        std::cerr << "Error: Not enough data points or data size mismatch for polynomial interpolation." << std::endl;
        return;
    }

    Eigen::VectorXd coefficients = polynomialInterpolation(xValues, yValues);
    std::cout << "Polynomial interpolation coefficients calculated." << std::endl;

    std::cout << "Polynomial Interpolation Equation:" << std::endl;
    std::cout << "P(x) = ";
    for (int i = 0; i < coefficients.size(); ++i) {
        if (i != 0) {
            std::cout << " + ";
        }
        std::cout << "(" << coefficients[i] << ")";
        if (i > 0) {
            std::cout << " * x";
            if (i > 1) {
                std::cout << "^" << i;
            }
        }
    }
    std::cout << std::endl;

    double x;
    std::cout << "Enter an x value to evaluate the polynomial: ";
    std::cin >> x;

    double interpolatedValue = evaluatePolynomial(coefficients, x);
    std::cout << "The value of P(" << x << ") = " << interpolatedValue << std::endl;
    std::cout << "Debug: Polynomial Interpolation Tool finished" << std::endl;
}

int main() {
    std::vector<StockData> stockData;
    ManualInputData manualData;
    DataSource lastDataSource = DataSource::None;
    std::string filename;
    int choice = 0, fileChoice = 0;
    bool exitProgram = false;
    std::vector<SplineSegment> splineSegments; // Assume this is filled with your spline data
    auto splineFunction = createSplineFunction(splineSegments);

    try {
        double value = splineFunction(1.5); // Test value
        std::cout << "Spline value at 1.5 is " << value << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    while (!exitProgram) {
        std::cout << "+---------------------------------+\n";
        std::cout << "|          Main Menu              |\n";
        std::cout << "+---------------------------------+\n";
        std::cout << "| 1. Load CSV file                |\n";
        std::cout << "| 2. Enter Manual Data            |\n";
        std::cout << "| 3. Interpolation Tool           |\n";
        std::cout << "| 4. Save CSV file                |\n";
        std::cout << "| 5. Show CSV file table          |\n";
        std::cout << "| 6. Exit                         |\n";
        std::cout << "+---------------------------------+\n";
        std::cout << "| Enter your choice:              |\n";
        std::cout << "+---------------------------------+\n";
        std::cout << "  ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Choose a dataset to load:\n";
            std::cout << "1. BTC\n";
            std::cout << "2. SP500\n";
            std::cin >> fileChoice;
            if (fileChoice == 1) {
                stockData = btcData;  // Use the hardcoded BTC data
                std::cout << "BTC data loaded successfully.\n";
            }
            else if (fileChoice == 2) {
                stockData = sp500Data;  // Use the hardcoded SP500 data
                std::cout << "SP500 data loaded successfully.\n";
            }
            else {
                std::cout << "Invalid choice. Please try again.\n";
                continue;
            }
            lastDataSource = DataSource::CSV;
            break;

        case 2:
            insertManualData(manualData);
            lastDataSource = DataSource::Manual;
            break;
        case 3:
            interpolationTool(lastDataSource, stockData, manualData);
            break;
        case 4: {
            std::string saveFilename;
            std::cout << "Enter the filename to save as: ";
            std::cin >> saveFilename;
            saveCSV(saveFilename, stockData);
            break;
        }
              break;
        case 5:
            if (lastDataSource == DataSource::CSV) {
                showCSVTable(stockData); // Show the full table first

                auto [xColumn, yColumn] = getColumnChoices(); // Get user choices for columns
                showSelectedDataWithYFit(stockData, xColumn, yColumn); // Show table with Y-Fit values
            }
            else if (lastDataSource == DataSource::Manual) {
                // If manual data, show the manual input table
                showManualInputTable(manualData);
            }
            else {
                std::cout << "No data available to display." << std::endl;
            }
            break;
        case 6:
            exitProgram = true;
            std::cout << "Exiting program." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
    return 0;
}

void saveCSV(const std::string& filename, const std::vector<StockData>& stockData) {
    std::ofstream file(filename + ".csv");
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << ".csv" << std::endl;
        return;
    }

    // Write header
    file << "Date,Open,High,Low,Close\n";

    // Write data
    for (const auto& entry : stockData) {
        file << entry.date << "," << entry.open << "," << entry.high << ","
            << entry.low << "," << entry.close << "\n";
    }

    file.close();
    std::cout << filename << ".csv saved successfully.\n";
}

void linearInterpolation(const std::vector<double>& xValues, const std::vector<double>& yValues) {
    std::cout << "Performing linear interpolation..." << std::endl;
    // Implement linear interpolation logic here
}

void linearInterpolationEquation(const std::vector<double>& xValues, const std::vector<double>& yValues) {
    if (xValues.size() != yValues.size() || xValues.size() < 2) {
        std::cerr << "Error: Invalid data for interpolation. Ensure equal number of x and y values and at least 2 points." << std::endl;
        return;
    }

    double xPoint;
    std::cout << "Enter the x value for which you want the linear equation: ";
    std::cin >> xPoint;

    for (size_t i = 0; i < xValues.size() - 1; ++i) {
        if ((xValues[i] - xPoint) * (xValues[i + 1] - xPoint) <= 0) {
            double x1 = xValues[i];
            double y1 = yValues[i];
            double x2 = xValues[i + 1];
            double y2 = yValues[i + 1];

            double m = (y2 - y1) / (x2 - x1);
            double c = y1 - m * x1;

            std::cout << "Linear equation: y = " << m << "x + " << c << std::endl;
            return;
        }
    }

    std::cout << "Given x value is outside the range of the input data." << std::endl;
}

void interpolationTool(DataSource source, const std::vector<StockData>& stockData, const ManualInputData& manualData) {
    if (source == DataSource::None) {
        std::cout << "No data selected for interpolation." << std::endl;
        return;
    }

    std::vector<double> xValues, yValues;

    if (source == DataSource::CSV) {
        auto [xColumn, yColumn] = getColumnChoices();
        for (const auto& entry : stockData) {
            xValues.push_back(entry.getValueByColumn(xColumn));
            yValues.push_back(entry.getValueByColumn(yColumn));
        }
    }
    else if (source == DataSource::Manual) {
        xValues = manualData.xValues;
        yValues = manualData.yValues;
    }

    int interpolationChoice;
    std::cout << "Interpolation Tool:\n1. Linear Interpolation\n2. Spline Interpolation\n3. Polynomial Interpolation\n4. Go Back\nEnter your choice: ";
    std::cin >> interpolationChoice;

    switch (interpolationChoice) {
    case 1:
        linearInterpolationEquation(xValues, yValues);
        break;
    case 2:
        std::cout << "Spline interpolation selected" << std::endl;
        splineInterpolation(xValues, yValues);
        break;
    case 3:

        polynomialInterpolationTool(xValues, yValues);
        break;
    case 4:
        // This case is for going back to the main menu
        std::cout << "Returning to main menu." << std::endl;
        return; // Exit the function, returning to the main menu
    default:
        std::cout << "Invalid choice. Please try again." << std::endl;
    }
}

    







