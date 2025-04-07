#include "food_database.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Constructor
FoodDatabase::FoodDatabase(const std::string& filename) : databaseFilename(filename) {
    loadFromFile();
}

// Database operations
bool FoodDatabase::loadFromFile() {
    std::ifstream file(databaseFilename);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open database file '" << databaseFilename << "' for reading." << std::endl;
        return false;
    }
    
    foods.clear();
    std::string line;
    std::vector<CompositeRelation> compositeRelations;
    
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Split the line to extract composite food information
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ';')) {
            tokens.push_back(token);
        }
        
        // Check if this is a composite food entry (has at least 5 tokens)
        if (tokens.size() >= 5 && tokens[3] == "1") {
            CompositeRelation relation;
            relation.foodId = tokens[0];
            
            // Extract component IDs
            std::stringstream compStream(tokens[4]);
            std::string compId;
            while (std::getline(compStream, compId, ',')) {
                relation.componentIds.push_back(compId);
            }
            
            compositeRelations.push_back(relation);
        }
        
        // Load the food (initially as basic food)
        Food food = Food::fromString(line);
        if (!food.getIdentifier().empty()) {
            foods.push_back(food);
        }
    }
    
    file.close();
    
    // Process composite relations after all foods are loaded
    if (!compositeRelations.empty()) {
        processCompositeRelations(compositeRelations);
    }
    
    return true;
}

void FoodDatabase::processCompositeRelations(const std::vector<CompositeRelation>& relations) {
    for (const auto& relation : relations) {
        // Find the composite food
        Food* compositeFood = findFoodByIdentifier(relation.foodId);
        if (!compositeFood) {
            continue;
        }
        
        // Find and add all components
        std::vector<Food> components;
        bool allComponentsFound = true;
        
        for (const auto& compId : relation.componentIds) {
            Food* component = findFoodByIdentifier(compId);
            if (component) {
                components.push_back(*component);
            } else {
                allComponentsFound = false;
                std::cerr << "Warning: Component '" << compId << "' not found for composite food '" 
                          << relation.foodId << "'" << std::endl;
                break;
            }
        }
        
        if (allComponentsFound && !components.empty()) {
            // Rebuild the composite food
            Food newCompositeFood(relation.foodId, components);
            // Replace the existing entry
            *compositeFood = newCompositeFood;
        }
    }
}

bool FoodDatabase::saveToFile() const {
    std::ofstream file(databaseFilename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open database file '" << databaseFilename << "' for writing." << std::endl;
        return false;
    }
    
    // Add a header comment
    file << "# Food Database Format: identifier;keyword1,keyword2,...;calories;isComposite;componentId1,componentId2,..." << std::endl;
    
    for (const auto& food : foods) {
        file << food.toString() << std::endl;
    }
    
    file.close();
    return true;
}

// Composite food operations
bool FoodDatabase::createCompositeFood(const std::string& name, const std::vector<std::string>& componentIds) {
    // Check if food with this name already exists
    if (findFoodByIdentifier(name) != nullptr) {
        std::cerr << "Error: Food with name '" << name << "' already exists." << std::endl;
        return false;
    }
    
    // Find all component foods
    std::vector<Food> components;
    for (const auto& compId : componentIds) {
        Food* component = findFoodByIdentifier(compId);
        if (component) {
            components.push_back(*component);
        } else {
            std::cerr << "Error: Component '" << compId << "' not found." << std::endl;
            return false;
        }
    }
    
    if (components.empty()) {
        std::cerr << "Error: Cannot create composite food with no components." << std::endl;
        return false;
    }
    
    // Create and add the composite food
    Food compositeFood(name, components);
    addFood(compositeFood);
    
    return true;
}

// Food operations
void FoodDatabase::addFood(const Food& food) {
    // Check if food with same identifier already exists
    for (auto& existingFood : foods) {
        if (existingFood.getIdentifier() == food.getIdentifier()) {
            // Replace existing food
            existingFood = food;
            return;
        }
    }
    
    // Add new food
    foods.push_back(food);
}

bool FoodDatabase::removeFood(const std::string& identifier) {
    for (auto it = foods.begin(); it != foods.end(); ++it) {
        if (it->getIdentifier() == identifier) {
            foods.erase(it);
            return true;
        }
    }
    return false;
}

Food* FoodDatabase::findFoodByIdentifier(const std::string& identifier) {
    for (auto& food : foods) {
        if (food.getIdentifier() == identifier) {
            return &food;
        }
    }
    return nullptr;
}

std::vector<Food> FoodDatabase::findFoodsByKeyword(const std::string& keyword) const {
    std::vector<Food> result;
    for (const auto& food : foods) {
        if (food.matchKeyword(keyword)) {
            result.push_back(food);
        }
    }
    return result;
}

std::vector<Food> FoodDatabase::findFoodsByAllKeywords(const std::vector<std::string>& keywords) const {
    std::vector<Food> result;
    for (const auto& food : foods) {
        if (food.matchAllKeywords(keywords)) {
            result.push_back(food);
        }
    }
    return result;
}

std::vector<Food> FoodDatabase::findFoodsByAnyKeyword(const std::vector<std::string>& keywords) const {
    std::vector<Food> result;
    for (const auto& food : foods) {
        if (food.matchAnyKeyword(keywords)) {
            result.push_back(food);
        }
    }
    return result;
}

// Other operations
size_t FoodDatabase::size() const {
    return foods.size();
}

const std::vector<Food>& FoodDatabase::getAllFoods() const {
    return foods;
}