#include <iostream>
#include <vector>
#include <string>
#include "LinearInterpolation1.h"
#include "SplineInterpolation1.h"
#include "PolynomialInterpolation1.h"
#include <Eigen/Dense>

enum class DataSource {
    None,
    CSV,
    Manual
};

struct StockData {
    std::string date;
    double open, high, low, close;

    double getValueByColumn(int column) const {
        switch (column) {
        case 1: return open;
        case 2: return high;
        case 3: return low;
        case 4: return close;
        default: return 0.0;
        }
    }
};

std::pair<int, int> getColumnChoices() {
    int xChoice, yChoice;
    std::cout << "Select the column for X axis:\n1. Open\n2. High\n3. Low\n4. Close\n";
    std::cin >> xChoice;
    std::cout << "Select the column for Y axis:\n";
    std::cin >> yChoice;
    return { xChoice, yChoice };
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

void interpolationTool(DataSource source, const std::vector<StockData>& stockData) {
    std::vector<double> xValues, yValues;

    if (source == DataSource::CSV) {
        auto [xColumn, yColumn] = getColumnChoices();
        for (const auto& entry : stockData) {
            xValues.push_back(entry.getValueByColumn(xColumn));
            yValues.push_back(entry.getValueByColumn(yColumn));
        }
    }
    else if (source == DataSource::Manual) {
        // Handle manual data input (not shown for brevity)
    }

    int interpolationChoice;
    std::cout << "Interpolation Tool:\n1. Linear Interpolation\n2. Spline Interpolation\n3. Polynomial Interpolation\nEnter your choice: ";
    std::cin >> interpolationChoice;

    switch (interpolationChoice) {
    case 1:
        LinearInterpolation::plotLinearInterpolation(xValues, yValues, "X-Axis Label", "Y-Axis Label");
        break;
    case 2:
        SplineInterpolation::plotSplineInterpolation(xValues, yValues, "X-Axis Label", "Y-Axis Label");
        break;
    case 3:
        auto coefficients = PolynomialInterpolation::calculatePolynomialCoefficients(xValues, yValues);
        PolynomialInterpolation::plotPolynomialInterpolation(coefficients, xValues, yValues, "X-Axis Label", "Y-Axis Label");
        break;
    default:
        std::cout << "Invalid choice. Please try again." << std::endl;
    }
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
