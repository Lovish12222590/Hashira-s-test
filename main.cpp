#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

long long baseToDecimal(const string& valueStr, int base) {
    long long decimalValue = 0;
    for (char ch : valueStr) {
        int digit;
        if (isdigit(ch)) digit = ch - '0';
        else digit = tolower(ch) - 'a' + 10;
        decimalValue = decimalValue * base + digit;
    }
    return decimalValue;
}

long double reconstructSecret(const vector<pair<int, long long>>& sharePoints) {
    long double secret = 0.0;
    int totalPoints = sharePoints.size();

    for (int i = 0; i < totalPoints; ++i) {
        long double basisTerm = static_cast<long double>(sharePoints[i].second);
        for (int j = 0; j < totalPoints; ++j) {
            if (i == j) continue;
            basisTerm *= static_cast<long double>(-sharePoints[j].first) / (sharePoints[i].first - sharePoints[j].first);
        }
        secret += basisTerm;
    }

    return secret;
}

vector<pair<int, long long>> loadPointsFromJson(const string& filename) {
    ifstream inputFile(filename);
    json jsonData;
    inputFile >> jsonData;

    vector<pair<int, long long>> points;
    for (auto& point : jsonData["points"]) {
        int xCoord = point["x"];
        string yValueStr = point["y"];
        int base = point["base"];
        long long yValue = baseToDecimal(yValueStr, base);
        points.emplace_back(xCoord, yValue);
    }
    return points;
}

int main() {
    vector<pair<int, long long>> testCasePoints1 = loadPointsFromJson("input1.json");
    vector<pair<int, long long>> testCasePoints2 = loadPointsFromJson("input2.json");

    cout << "Secret for Test Case 1: " << reconstructSecret(testCasePoints1) << endl;
    cout << "Secret for Test Case 2: " << reconstructSecret(testCasePoints2) << endl;

    return 0;
}
