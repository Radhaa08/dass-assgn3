#include "food.h"
#include "food_database.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>

// Function to clear the input buffer
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Function to add a new food to the database
void addNewFood(FoodDatabase& db) {
    std::string identifier;
    std::string keywordsStr;
    std::vector<std::string> keywords;
    int calories;
    
    std::cout << "\n=== Add New Food ===\n";
    
    std::cout << "Enter food name: ";
    std::getline(std::cin, identifier);
    
    std::cout << "Enter keywords (comma-separated): ";
    std::getline(std::cin, keywordsStr);
    
    // Parse keywords
    size_t pos = 0;
    std::string keyword;
    while ((pos = keywordsStr.find(',')) != std::string::npos) {
        keyword = keywordsStr.substr(0, pos);
        keywordsStr.erase(0, pos + 1);
        // Trim whitespace
        keyword.erase(0, keyword.find_first_not_of(" \t\n\r\f\v"));
        keyword.erase(keyword.find_last_not_of(" \t\n\r\f\v") + 1);
        if (!keyword.empty()) {
            keywords.push_back(keyword);
        }
    }
    // Add the last keyword
    keywordsStr.erase(0, keywordsStr.find_first_not_of(" \t\n\r\f\v"));
    keywordsStr.erase(keywordsStr.find_last_not_of(" \t\n\r\f\v") + 1);
    if (!keywordsStr.empty()) {
        keywords.push_back(keywordsStr);
    }
    
    std::cout << "Enter calories per serving: ";
    while (!(std::cin >> calories) || calories < 0) {
        std::cout << "Invalid input. Please enter a positive number: ";
        clearInputBuffer();
    }
    clearInputBuffer(); // Clear the newline character
    
    // Create the food and add it to the database
    Food newFood(identifier, keywords, calories);
    db.addFood(newFood);
    
    std::cout << "\nFood \"" << identifier << "\" added successfully!\n";
}

// Function to display all foods
void displayAllFoods(const FoodDatabase& db) {
    std::cout << "\n=== All Foods ===\n";
    
    const auto& foods = db.getAllFoods();
    if (foods.empty()) {
        std::cout << "No foods in the database.\n";
        return;
    }
    
    std::cout << "ID\tCalories\tType\t\tKeywords\n";
    std::cout << "-------------------------------------------------------\n";
    
    for (const auto& food : foods) {
        std::cout << food.getIdentifier() << "\t";
        std::cout << food.getCaloriesPerServing() << "\t\t";
        std::cout << (food.getIsComposite() ? "Composite\t" : "Basic\t\t");
        
        const auto& keywords = food.getKeywords();
        for (size_t i = 0; i < keywords.size(); ++i) {
            std::cout << keywords[i];
            if (i < keywords.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "\n";
        
        // If it's a composite food, display its components
        if (food.getIsComposite()) {
            std::cout << "  Components: ";
            const auto& components = food.getComponents();
            for (size_t i = 0; i < components.size(); ++i) {
                std::cout << components[i].getIdentifier();
                if (i < components.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "\n";
        }
    }
}

// Function to create a composite food
void createCompositeFood(FoodDatabase& db) {
    std::string compositeName;
    std::vector<std::string> componentIds;
    std::string componentId;
    char continueAdding = 'y';
    
    std::cout << "\n=== Create Composite Food ===\n";
    
    std::cout << "Enter name for the composite food: ";
    std::getline(std::cin, compositeName);
    
    // Check if food with this name already exists
    if (db.findFoodByIdentifier(compositeName) != nullptr) {
        std::cout << "Error: Food with name '" << compositeName << "' already exists.\n";
        return;
    }
    
    // Display all available foods for reference
    displayAllFoods(db);
    
    // Add components to the composite food
    while (tolower(continueAdding) == 'y') {
        std::cout << "Enter ID of food to add as component: ";
        std::getline(std::cin, componentId);
        
        // Check if the food exists
        if (db.findFoodByIdentifier(componentId) == nullptr) {
            std::cout << "Food with ID \"" << componentId << "\" not found in database.\n";
        } else {
            componentIds.push_back(componentId);
            std::cout << "Added \"" << componentId << "\" to " << compositeName << "\n";
        }
        
        std::cout << "Add another component? (y/n): ";
        std::cin >> continueAdding;
        clearInputBuffer();
    }
    
    if (componentIds.empty()) {
        std::cout << "No components added. Cannot create empty composite food.\n";
        return;
    }
    
    // Create the composite food using the database helper
    if (db.createCompositeFood(compositeName, componentIds)) {
        std::cout << "\nComposite food \"" << compositeName << "\" created with components:\n";
        for (const auto& id : componentIds) {
            const Food* comp = db.findFoodByIdentifier(id);
            if (comp) {
                std::cout << "- " << comp->getIdentifier() << " (" << comp->getCaloriesPerServing() << " calories)\n";
            }
        }
        std::cout << "Composite food added successfully!\n";
    } else {
        std::cout << "Failed to create composite food.\n";
    }
}

// Function to search foods by keyword
void searchFoodsByKeyword(const FoodDatabase& db) {
    std::string searchType;
    std::string keywordsStr;
    std::vector<std::string> keywords;
    
    std::cout << "\n=== Search Foods ===\n";
    std::cout << "Search for foods matching (A)ny or (A)ll keywords? (A/L): ";
    std::getline(std::cin, searchType);
    
    std::cout << "Enter keywords to search (comma-separated): ";
    std::getline(std::cin, keywordsStr);
    
    // Parse keywords
    size_t pos = 0;
    std::string keyword;
    while ((pos = keywordsStr.find(',')) != std::string::npos) {
        keyword = keywordsStr.substr(0, pos);
        keywordsStr.erase(0, pos + 1);
        // Trim whitespace
        keyword.erase(0, keyword.find_first_not_of(" \t\n\r\f\v"));
        keyword.erase(keyword.find_last_not_of(" \t\n\r\f\v") + 1);
        if (!keyword.empty()) {
            keywords.push_back(keyword);
        }
    }
    // Add the last keyword
    keywordsStr.erase(0, keywordsStr.find_first_not_of(" \t\n\r\f\v"));
    keywordsStr.erase(keywordsStr.find_last_not_of(" \t\n\r\f\v") + 1);
    if (!keywordsStr.empty()) {
        keywords.push_back(keywordsStr);
    }
    
    if (keywords.empty()) {
        std::cout << "No keywords entered. Please try again.\n";
        return;
    }
    
    std::vector<Food> results;
    if (searchType == "A" || searchType == "a") {
        results = db.findFoodsByAnyKeyword(keywords);
        std::cout << "\nFoods matching ANY of the keywords: ";
    } else {
        results = db.findFoodsByAllKeywords(keywords);
        std::cout << "\nFoods matching ALL of the keywords: ";
    }
    
    // Display the keywords being searched for
    for (size_t i = 0; i < keywords.size(); ++i) {
        std::cout << "'" << keywords[i] << "'";
        if (i < keywords.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "\n";
    
    if (results.empty()) {
        std::cout << "No matching foods found.\n";
        return;
    }
    
    std::cout << "ID\tCalories\tType\t\tKeywords\n";
    std::cout << "-------------------------------------------------------\n";
    
    for (const auto& food : results) {
        std::cout << food.getIdentifier() << "\t";
        std::cout << food.getCaloriesPerServing() << "\t\t";
        std::cout << (food.getIsComposite() ? "Composite\t" : "Basic\t\t");
        
        const auto& foodKeywords = food.getKeywords();
        for (size_t i = 0; i < foodKeywords.size(); ++i) {
            std::cout << foodKeywords[i];
            if (i < foodKeywords.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "\n";
        
        if (food.getIsComposite()) {
            std::cout << "  Components: ";
            const auto& components = food.getComponents();
            for (size_t i = 0; i < components.size(); ++i) {
                std::cout << components[i].getIdentifier();
                if (i < components.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "\n";
        }
    }
}

// Main menu function
void displayMenu() {
    std::cout << "\n=== YADA Food Database ===\n";
    std::cout << "1. Add a new food\n";
    std::cout << "2. Display all foods\n";
    std::cout << "3. Search foods by keyword\n";
    std::cout << "4. Create composite food\n";
    std::cout << "5. Save database\n";
    std::cout << "6. Exit\n";
    std::cout << "Enter your choice (1-6): ";
}

int main() {
    std::cout << "YADA (Yet Another Diet Assistant) - Food Database\n";
    std::cout << "===============================================\n";
    
    // Create a food database
    FoodDatabase db("foods.txt");
    
    int choice;
    bool exitProgram = false;
    
    while (!exitProgram) {
        displayMenu();
        
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
            continue;
        }
        clearInputBuffer(); // Clear the newline character
        
        switch (choice) {
            case 1:
                addNewFood(db);
                break;
            case 2:
                displayAllFoods(db);
                break;
            case 3:
                searchFoodsByKeyword(db);
                break;
            case 4:
                createCompositeFood(db);
                break;
            case 5:
                if (db.saveToFile()) {
                    std::cout << "\nDatabase saved successfully!\n";
                } else {
                    std::cout << "\nError saving database.\n";
                }
                break;
            case 6:
                std::cout << "\nSaving database before exit...\n";
                db.saveToFile();
                std::cout << "Thank you for using YADA. Goodbye!\n";
                exitProgram = true;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    
    return 0;
}