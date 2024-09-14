#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <limits>
#include <iomanip>  // For formatting output

using namespace std;

// Helper function to convert a string to lowercase
string toLowerCase(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Function to perform binary search for ingredients based on name (case-insensitive)
int binarySearchIngredientsByName(const vector<pair<string, int>>& inventory, const string& ingredient) {
    int low = 0, high = inventory.size() - 1;
    string ingredientLower = toLowerCase(ingredient);  // Convert the input to lowercase
    while (low <= high) {
        int mid = low + (high - low) / 2;
        string midIngredientLower = toLowerCase(inventory[mid].first);  // Compare in lowercase
        if (midIngredientLower == ingredientLower)
            return mid;
        else if (midIngredientLower < ingredientLower)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

// Merge function for merge sort
void merge(vector<pair<string, int>>& orders, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<pair<string, int>> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = orders[l + i];
    for (int i = 0; i < n2; i++) R[i] = orders[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].second <= R[j].second)
            orders[k++] = L[i++];
        else
            orders[k++] = R[j++];
    }

    while (i < n1)
        orders[k++] = L[i++];
    while (j < n2)
        orders[k++] = R[j++];
}

// Merge sort for sorting orders
void mergeSort(vector<pair<string, int>>& orders, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(orders, l, m);
        mergeSort(orders, m + 1, r);
        merge(orders, l, m, r);
    }
}

// Coin change for calculating tips
int coinChangeHelper(const vector<int>& coins, int amount,vector<int>& memo) {
    // Base cases
    if (amount == 0) return 0;
    if (amount < 0) return -1;

    if (memo[amount] != -1) return memo[amount];

    int minCoins = INT_MAX;
    for (int coin : coins) {
        int res = coinChangeHelper(coins, amount - coin, memo);
        if (res != -1 && res + 1 < minCoins) {
            minCoins = res + 1;
        }
    }

    memo[amount] = (minCoins == INT_MAX) ? -1 : minCoins;
    return memo[amount];
}

int coinChange(const vector<int>& coins, int amount) {
    vector<int> memo(amount + 1, -1);
    return coinChangeHelper(coins, amount, memo);
}

// Fractional knapsack for menu design (profit maximization)
double fractionalKnapsack(vector<pair<double, double>>& dishes, double maxTime) {
    sort(dishes.begin(), dishes.end(), [](const pair<double, double>& a, const pair<double, double>& b) {
        return (a.first / a.second) > (b.first / b.second);
    });

    double totalProfit = 0.0;
    for (auto& dish : dishes) {
        if (maxTime >= dish.second) {
            maxTime -= dish.second;
            totalProfit += dish.first;
        } else {
            totalProfit += (dish.first / dish.second) * maxTime;
            break;
        }
    }
    return totalProfit;
}

// Activity scheduling for table reservations
int activitySelection(vector<pair<int, int>>& reservations) {
    sort(reservations.begin(), reservations.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.second < b.second;
    });

    int count = 1, lastEnd = reservations[0].second;
    for (int i = 1; i < reservations.size(); ++i) {
        if (reservations[i].first >= lastEnd) {
            count++;
            lastEnd = reservations[i].second;
        }
    }
    return count;
}

// Function to display the names, prices, and expiry dates of the ingredients
void displayIngredientDetails(const vector<pair<string, int>>& inventory) {
    cout << "\nAvailable ingredients and their details:\n\n";
    for (size_t i = 0; i < inventory.size(); ++i) {
        cout << setw(3) << setfill('0') << (102 + i) << ". " << inventory[i].first
             << "-----Price: $" << (inventory[i].second + 20)
             << "-----Expiry Day: " << inventory[i].second << " days"<< endl;
    }
}

// Function to display the names of the ingredients
void displayIngredientNames(const vector<pair<string, int>>& inventory) {
    cout << "\nAvailable ingredients:\n";
    for (size_t i = 0; i < inventory.size(); ++i) {
        cout << setw(3) << setfill('0') << (102 + i) << ". " << inventory[i].first << endl;
    }
}

void itemSelectionAndDisplay(vector<pair<string, int>>& inventory) {
    displayIngredientDetails(inventory);

    int numItems;
    cout << "\nHow many food item you want :";
    cin >> numItems;

    vector<int> selectedIndices(numItems);
    cout << "Enter the serial number from menu card : ";
    for (int i = 0; i < numItems; ++i) {
        int serialNumber;
        cin >> serialNumber;
        selectedIndices[i] = serialNumber - 102; // Convert to 0-based index
    }

    double totalCost = 0.0;
    cout << "\nSelected items and their details:\n";
    for (int index : selectedIndices) {
        if (index >= 0 && index < inventory.size()) {
            cout << inventory[index].first << "-----Price: $" << (inventory[index].second + 20) << "-----Expiry Day: " << inventory[index].second << endl;
            totalCost += inventory[index].second + 20;
        } else {
            cout << "Invalid item number.\n";
        }
    }

    cout << "\nTotal cost of selected items: $" << totalCost << endl;

    char orderChoice;
    cout << "\nDo you want to place an order for these items? (y/n): ";
    cin >> orderChoice;

    if (orderChoice == 'y' || orderChoice == 'Y') {
        cout << "\nOrder placed successfully!\n";

        // Now handle the tip calculation
        //tipCalculation();

        // Sort selected items based on their price for ordering
        vector<pair<string, int>> selectedItems;
        for (int index : selectedIndices) {
            if (index >= 0 && index < inventory.size()) {
                selectedItems.emplace_back(inventory[index].first, inventory[index].second);
            }
        }
        mergeSort(selectedItems, 0, selectedItems.size() - 1);
        cout << "\nSorted selected items based on price:\n";
        for (const auto& item : selectedItems) {
            cout << item.first << ": $" << (item.second + 20) << endl;
        }
    } else {
        cout << "Order not placed.\n";
    }
}

void tipCalculation() {
    int numCoins;
    cout << "\nEnter the number of coin types for paying the bill: ";
    cin >> numCoins;
    vector<int> coins(numCoins);
    cout << "Enter the coin denominations:" << endl;
    for (int i = 0; i < numCoins; ++i) {
        cin >> coins[i];
    }
    int tipAmount;
    cout << "Enter the tip amount: ";
    cin >> tipAmount;
    int result = coinChange(coins, tipAmount);
    if (result != -1)
        cout << "\nMinimum coins required for tip: " << result << endl;
    else
        cout << "\nIt's not possible to provide the exact tip amount with given coins.\n";
}

void menuDesign() {
    int numDishes;
    cout << "Enter the number of dishes: ";
    cin >> numDishes;
    vector<pair<double, double>> dishes(numDishes);
    cout << "Enter the profit and time for each dish:" << endl;
    for (int i = 0; i < numDishes; ++i) {
        cin >> dishes[i].first >> dishes[i].second;
    }
    double maxTime;
    cout << "Enter the maximum available time: ";
    cin >> maxTime;
    double maxProfit = fractionalKnapsack(dishes, maxTime);
    cout << "\nMaximum profit for menu design: " << maxProfit << endl;
}

void tableReservation() {
    int numReservations;
    cout << "Enter the number of reservations: ";
    cin >> numReservations;
    vector<pair<int, int>> reservations(numReservations);
    cout << "Enter the start and end time for each reservation:" << endl;
    for (int i = 0; i < numReservations; ++i) {
        cin >> reservations[i].first >> reservations[i].second;
    }
    int maxActivities = activitySelection(reservations);
    cout << "\nMaximum number of non-overlapping reservations: " << maxActivities << endl;
}

int main() {
    vector<pair<string, int>> inventory = {
        {"Pulao & Korma", 25}, {"Biryani------", 30}, {"Rogan Josh---", 28},
        {"Khichuri-----", 22}, {"Prawn Curry--", 35}, {"Beef Bhuna---", 26},
        {"Mutton Kebab-", 32}, {"Kacchi Birani", 33}, {"Cingri Bhorta", 20},
        {"Reshmi Kebab-", 29}, {"Hilsa Fish---", 27}, {"Pulao & Raita", 21},
        {"Nihari-------", 24}, {"Prawn Tempura", 31}, {"Paneer Tikka-", 23},
        {"Mutton Korma-", 30}, {"Chicken Tikka", 28}, {"Tandori Ciken", 27}
    };
    int choice;

    do {
        cout << "\n----------------------------------------------------" << endl ;
        cout << "                 : What do you want : \n"                     ;
        cout << "----------------------------------------------------\n" <<endl;
        cout << "1. View Menu Card \n";
        cout << "2. Select Items and Place Order\n";
        cout << "3. Menu Design\n";
        cout << "4. Table Reservation\n";
        cout << "0. Exit\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayIngredientDetails(inventory);
                break;
            case 2:
                itemSelectionAndDisplay(inventory);
                break;
            case 3:
                menuDesign();
                break;
            case 4:
                tableReservation();
                break;
            case 0:
                cout << "Exiting the program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}


