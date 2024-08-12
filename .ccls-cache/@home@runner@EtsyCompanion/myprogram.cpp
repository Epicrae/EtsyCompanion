#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

// function prototypes
void menu();                                      // menu display
void analyzeCountries(const string &file_name);   // get the data for the
                                                  // countries
void analyzeStates(const std::string &file_name); // get the data for the states
void analyzeCities(const std::string &file_name); // get the data for the cities
void displayTopLocations(const string &file_name,
                         int topN); // display the top N locations
void analyzeLocations(const string &file_name,
                      vector<pair<string, int>> &sortedLocations,
                      const string &locationType);

int main() {
  string file_name;
  cout << "Enter the name of your CSV file (including .csv extension): ";
  getline(cin, file_name);

  int choice;
  string input;

  do {
    menu();
    getline(cin, input);

    // Check if the input is a valid integer
    if (stringstream(input) >> choice) {
      switch (choice) {
      case 1: {
        analyzeCountries(file_name);
        break;
      }
      case 2: {
        analyzeStates(file_name);
        break;
      }
      case 3: {
        analyzeCities(file_name);
        break;
      }
      case 4: {
        int topN = 5; // Number of top locations to display
        displayTopLocations(file_name, topN);
        break;
      }
      case 5: {
        cout << "Exiting the program." << endl;
        break;
      }
      default:
        cout << "Invalid choice. Please enter a number between 1 and 5."
             << endl;
        break;
      }
    } else {
      cout << "Invalid input. Please enter a valid integer choice." << endl;
    }

    cout << endl;
  } while (choice != 5);

  return 0;
}

void menu() {
  cout << "+---------------------------------+\n";
  cout << "|     EtsyOrderAnalyzer           |\n";
  cout << "+---------------------------------+\n";
  cout << "| 1. View Shipping Countries      |\n";
  cout << "| 2. View Shipping States         |\n";
  cout << "| 3. View Shipping Cities         |\n";
  cout << "| 4. Top Shipping Locations       |\n";
  cout << "| 5. Exit                         |\n";
  cout << "+---------------------------------+\n";
  cout << "Select an option: ";
}

void analyzeCountries(const string &file_name) {
  vector<pair<string, int>> sortedCountries;
  analyzeLocations(file_name, sortedCountries, "Country");

  cout << "\nCOUNTRIES SHIPPED to and number of orders:\n" << endl;
  int totalOrders = 0;
  for (const auto &entry : sortedCountries) {
    cout << entry.first << ": " << entry.second << " order"
         << (entry.second != 1 ? "s" : "") << endl;
    totalOrders += entry.second;
  }

  cout << "\nTotal COUNTRIES: " << sortedCountries.size() << endl;
  cout << "Total ORDERS: " << totalOrders << endl;
}

void analyzeStates(const string &file_name) {
  vector<pair<string, int>> sortedStates;
  analyzeLocations(file_name, sortedStates, "State");

  cout << "\nSTATES SHIPPED to and number of orders:\n" << endl;
  for (const auto &entry : sortedStates) {
    cout << entry.first << ": " << entry.second
         << (entry.second == 1 ? " order" : " orders") << endl;
  }
}

void analyzeCities(const string &file_name) {
  vector<pair<string, int>> sortedCities;
  analyzeLocations(file_name, sortedCities, "City");

  cout << "\nCITIES SHIPPED to and number of orders:\n" << endl;
  for (const auto &entry : sortedCities) {
    cout << entry.first << ": " << entry.second
         << (entry.second == 1 ? " order" : " orders") << endl;
  }
  cout << "\nTotal CITIES: " << sortedCities.size() << endl;
}

void displayTopLocations(const string &file_name, int topN) {
  // Analyze countries, states, and cities
  vector<pair<string, int>> sortedCountries, sortedStates, sortedCities;
  analyzeLocations(file_name, sortedCountries, "Country");
  analyzeLocations(file_name, sortedStates, "State");
  analyzeLocations(file_name, sortedCities, "City");

  auto displayTop = [topN](const vector<pair<string, int>> &sorted,
                           const string &locationType) {
    cout << "\n" << locationType << ":\n";
    int totalOrders = 0;
    for (int i = 0; i < topN && i < sorted.size(); ++i) {
      cout << i + 1 << ". " << sorted[i].first << ": " << sorted[i].second
           << " order" << (sorted[i].second != 1 ? "s" : "") << "\n";
      totalOrders += sorted[i].second;
    }
    cout << "Total orders for top " << min(topN, (int)sorted.size()) << " "
         << locationType << ": " << totalOrders << "\n";
  };

  // Display top-selling locations
  cout << "\nTop " << topN << " selling locations:\n";
  displayTop(sortedCountries, "Countries");
  displayTop(sortedStates, "States");
  displayTop(sortedCities, "Cities");
}

// Helper function to convert string to lowercase
string toLower(string s) {
  transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return std::tolower(c); });
  return s;
}

// update to make parsing more flexible to find correct column based on header
void analyzeLocations(const string &file_name,
                      vector<pair<string, int>> &sortedLocations,
                      const string &locationType) {
  ifstream file(file_name);
  if (!file) {
    cout << "Error opening file: " << file_name << endl;
    return;
  }

  string line;
  unordered_map<string, int> locationOrderCount;
  int columnIndex = -1;
  int countryColumnIndex =
      -1; // New: to find the country column for state filtering

  // Read the header line to find the correct column
  getline(file, line);
  istringstream headerStream(line);
  string header;
  int currentColumn = 0;
  string columnToFind = "Ship " + locationType;

  while (getline(headerStream, header, '\t')) {
    if (header == columnToFind) {
      columnIndex = currentColumn;
    }
    if (header == "Ship Country") {
      countryColumnIndex = currentColumn;
    }
    currentColumn++;
  }

  if (columnIndex == -1) {
    cout << "Error: Could not find column for " << columnToFind << endl;
    return;
  }

  unordered_set<string> countries;
  if (locationType == "State") {
    while (getline(file, line)) {
      istringstream ss(line);
      vector<string> fields;
      string field;
      while (getline(ss, field, '\t')) {
        fields.push_back(field);
      }
      if (fields.size() > countryColumnIndex) {
        countries.insert(fields[countryColumnIndex]);
      }
    }
    file.clear();
    file.seekg(0);
    getline(file, line); // Skip header
  }

  // Process the data
  while (getline(file, line)) {
    istringstream ss(line);
    string field;
    string country;
    vector<string> fields;
    while (getline(ss, field, '\t')) {
      fields.push_back(field);
    }
    if (fields.size() > max(columnIndex, countryColumnIndex)) {
      country = fields[countryColumnIndex];
      if (!fields[columnIndex].empty()) {
        if (locationType == "City") {
          // Normalize city names to uppercase
          transform(fields[columnIndex].begin(), fields[columnIndex].end(),
                    fields[columnIndex].begin(), ::toupper);
        }
        if (locationType != "State" ||
            countries.find(country) != countries.end()) {
          locationOrderCount[fields[columnIndex]]++;
        }
      }
    }
  }
  file.close();

  // Sort the locationOrderCount map in descending order of order counts
  sortedLocations.assign(locationOrderCount.begin(), locationOrderCount.end());
  sort(sortedLocations.begin(), sortedLocations.end(),
       [](const auto &a, const auto &b) { return a.second > b.second; });
}

