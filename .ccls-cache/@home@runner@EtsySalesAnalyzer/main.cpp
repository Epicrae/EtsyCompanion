#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
// function prototypes
void menu();
void analyzeCountries(const string &file_name);
void analyzeStates(const std::string &file_name);
void analyzeCities(const std::string &file_name);
void displayTopLocations(const string &file_name, int topN);
void analyzeLocations(const string &file_name, int columnIndex, vector<pair<string, int>> &sortedLocations);

int main() {
  std::string file_name = "EtsySoldOrders2023.csv";

  int choice; // get users choice
  do {
    menu();
    cin >> choice;

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
      cout << "Invalid choice. Please try again." << endl;
      break;
    }

    cout << endl;
  } while (choice != 5);

  return 0;
}

void menu() {

  cout << "+---------------------------------+\n";
  cout << "|    <ETSY SHIPMENTS APP>         |\n";
  cout << "+---------------------------------+\n";
  cout << "| 1. View Countries               |\n";
  cout << "| 2. View States                  |\n";
  cout << "| 3. View Cities                  |\n";
  cout << "| 4. Top Locations                |\n";
  cout << "| 5. Exit                         |\n";
  cout << "+---------------------------------+\n";
  cout << "Select an option: ";
}

void analyzeCountries(const string &file_name) {
  ifstream file(file_name);
  if (!file) {
    cout << "Error opening file" << endl;
    return;
  }

  string line;
  unordered_map<std::string, int> countryOrderCount;
  vector<std::string> countriesList; // Create a vector to store country names

  // Skip the header line
  getline(file, line);

  while (getline(file, line)) {
    istringstream ss(line);
    string field;
    string country;

    // Extract the country from the appropriate column (column 14, index 13)
    for (int i = 0; i < 15; ++i) {
      getline(ss, field, '\t');
      if (i == 14) {
        country = field;
        countryOrderCount[country]++;
        countriesList.push_back(
            country); // Store the country name in the vector
        break;
      }
    }
  }

  file.close();

  // Sort the countryOrderCount map in descending order of order counts
  vector<pair<string, int>> sortedCountries(countryOrderCount.begin(),
                                            countryOrderCount.end());
  sort(sortedCountries.begin(), sortedCountries.end(),
       [](const auto &a, const auto &b) { return a.second > b.second; });

  // Output the countries and their corresponding order counts in descending
  // order
  cout << "\nCOUNTRIES SHIPPED to and number of orders:\n"
       << endl;
  for (const auto &entry : sortedCountries) {
    cout << entry.first << ": " << entry.second << " orders" << std::endl;
  }

  // Output total orders
  int totalOrders = 0;
  for (const auto &entry : countryOrderCount) {
    totalOrders += entry.second;
  }
  cout << "Total ORDERS: " << totalOrders << std::endl;
}
void analyzeStates(const std::string &file_name) {

  ifstream file(file_name);
  if (!file) {
    cout << "Error opening file" << endl;
  }
  string line;
  unordered_map<std::string, int> statesOrderCount;

  while (getline(file, line)) {
    istringstream iss(line);
    string state;
    string field;
    for (int i = 0; i < 13; ++i) {
      getline(iss, field, '\t');
      if (i == 12) {
        state = field;
        statesOrderCount[state]++;
      }
    }
  }
  file.close();

  // Sort the statesOrderCount map in descending order of order counts
  vector<pair<string, int>> sortedStates(statesOrderCount.begin(),
                                         statesOrderCount.end());
  sort(sortedStates.begin(), sortedStates.end(),
       [](const auto &a, const auto &b) { return a.second > b.second; });

  // Output the states and their corresponding order counts in descending order
  cout << "\nSTATES SHIPPED to and number of orders:\n"
       << endl;
  for (const auto &entry : sortedStates) {
    // Check if the number of orders is less than 2
    if (entry.second < 2) {
      cout << entry.first << ": " << entry.second << " order\n";
    } else {
      cout << entry.first << ": " << entry.second << " orders\n";
    }
  }
}
void analyzeCities(const std::string &file_name) {
    ifstream file(file_name);
    if (!file) {
        cout << "Error opening file" << endl;
    }

    string line;
    unordered_map<std::string, int> citiesOrderCount;

    while (getline(file, line)) {
        istringstream iss(line);
        string city;
        string field;
        for (int i = 0; i < 12; ++i) {
            getline(iss, field, '\t');
            if (i == 11) {
                city = field;
                citiesOrderCount[city]++;
            }
        }
    }

    // Sort the citiesOrderCount map in descending order of order counts
    vector<pair<string, int>> sortedCities(citiesOrderCount.begin(), citiesOrderCount.end());
    sort(sortedCities.begin(), sortedCities.end(), [](const auto &a, const auto &b) {
        return a.second > b.second;
    });

    // Output the cities and their corresponding order counts in descending order
    cout << "\nCITIES SHIPPED to and number of orders: \n" << endl;
    for (const auto &entry : sortedCities) {
        if (entry.second < 2) {
            cout << entry.first << ": " << entry.second << " order\n";
        } else {
            cout << entry.first << ": " << entry.second << " orders\n";
        }
    }
   cout<<"\n Total CITIES: "<<citiesOrderCount.size()<<endl;

    file.close();
}
void displayTopLocations(const string &file_name, int topN) {
    // Analyze countries
    vector<pair<string, int>> sortedCountries;
    analyzeLocations(file_name, 14, sortedCountries); //column 14

    // Analyze states
    vector<pair<string, int>> sortedStates;
    analyzeLocations(file_name, 12, sortedStates); //column 12

    // Analyze cities
    vector<pair<string, int>> sortedCities;
    analyzeLocations(file_name, 11, sortedCities); //column 11

    // Display top-selling locations
    cout << "\nTop " << topN << " selling locations:\n";

    cout << "\nCountries:\n";
    for (int i = 0; i < topN && i < sortedCountries.size(); ++i) {
        cout << i + 1 << ". " << sortedCountries[i].first << ": " << sortedCountries[i].second << " orders\n";
    }

    cout << "\nStates:\n";
    for (int i = 0; i < topN && i < sortedStates.size(); ++i) {
        cout << i + 1 << ". " << sortedStates[i].first << ": " << sortedStates[i].second << " orders\n";
    }

    cout << "\nCities:\n";
    for (int i = 0; i < topN && i < sortedCities.size(); ++i) {
        cout << i + 1 << ". " << sortedCities[i].first << ": " << sortedCities[i].second << " orders\n";
    }
}
void analyzeLocations(const string &file_name, int columnIndex, vector<pair<string, int>> &sortedLocations) {
    ifstream file(file_name);
    if (!file) {
        cout << "Error opening file" << endl;
        return;
    }

    string line;
    unordered_map<string, int> locationOrderCount;

    // Skip the header line
    getline(file, line);

    while (getline(file, line)) {
        istringstream ss(line);
        string field;
        string location;

        // Extract the location from the appropriate column
        for (int i = 0; i <= columnIndex; ++i) {
            getline(ss, field, '\t');
            if (i == columnIndex) {
                location = field;
                locationOrderCount[location]++;
                break;
            }
        }
    }

    file.close();

    // Sort the locationOrderCount map in descending order of order counts
    sortedLocations.assign(locationOrderCount.begin(), locationOrderCount.end());
    sort(sortedLocations.begin(), sortedLocations.end(),
         [](const auto &a, const auto &b) { return a.second > b.second; });
}