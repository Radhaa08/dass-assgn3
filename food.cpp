#include "food.h"
#include <sstream>
#include <algorithm>

// Default constructor
Food::Food() : identifier(""), caloriesPerServing(0), isComposite(false) {}

// Parameterized constructor
Food::Food(const std::string& id, const std::vector<std::string>& kw, int calories) 
    : identifier(id), keywords(kw), caloriesPerServing(calories), isComposite(false) {}

// New constructor for composite foods
Food::Food(const std::string& id, const std::vector<Food>& comps) 
    : identifier(id), isComposite(true), components(comps) {
    
    // Calculate total calories from components
    int totalCalories = 0;
    std::vector<std::string> allKeywords;
    
    for (const auto& component : components) {
        totalCalories += component.getCaloriesPerServing();
        
        // Collect all unique keywords from components
        const auto& compKeywords = component.getKeywords();
        for (const auto& keyword : compKeywords) {
            // Add keyword only if it doesn't exist already
            if (std::find(allKeywords.begin(), allKeywords.end(), keyword) == allKeywords.end()) {
                allKeywords.push_back(keyword);
            }
        }
    }
    
    caloriesPerServing = totalCalories;
    keywords = allKeywords;
}

// Getters
std::string Food::getIdentifier() const {
    return identifier;
}

std::vector<std::string> Food::getKeywords() const {
    return keywords;
}

int Food::getCaloriesPerServing() const {
    return caloriesPerServing;
}

bool Food::getIsComposite() const {
    return isComposite;
}

const std::vector<Food>& Food::getComponents() const {
    return components;
}

// Setters
void Food::setIdentifier(const std::string& id) {
    identifier = id;
}

void Food::addKeyword(const std::string& keyword) {
    keywords.push_back(keyword);
}

void Food::setKeywords(const std::vector<std::string>& kw) {
    keywords = kw;
}

void Food::setCaloriesPerServing(int calories) {
    caloriesPerServing = calories;
}

void Food::addComponent(const Food& component) {
    components.push_back(component);
    caloriesPerServing += component.getCaloriesPerServing();
    
    // Add unique keywords from the component
    const auto& compKeywords = component.getKeywords();
    for (const auto& keyword : compKeywords) {
        if (std::find(keywords.begin(), keywords.end(), keyword) == keywords.end()) {
            keywords.push_back(keyword);
        }
    }
    
    // Set to composite if this is the first component
    if (!isComposite && components.size() == 1) {
        isComposite = true;
    }
}

// Other methods remain the same...

// Format: identifier;keyword1,keyword2,...;calories;isComposite;component1ID,component2ID,...
std::string Food::toString() const {
    std::stringstream ss;
    ss << identifier << ";";
    
    // Add keywords separated by commas
    for (size_t i = 0; i < keywords.size(); ++i) {
        ss << keywords[i];
        if (i < keywords.size() - 1) {
            ss << ",";
        }
    }
    
    ss << ";" << caloriesPerServing << ";" << (isComposite ? "1" : "0");
    
    // Add component IDs if this is a composite food
    if (isComposite) {
        ss << ";";
        for (size_t i = 0; i < components.size(); ++i) {
            ss << components[i].getIdentifier();
            if (i < components.size() - 1) {
                ss << ",";
            }
        }
    }
    
    return ss.str();
}
// Add these implementations to your Food.cpp file

// Check if this food matches a single keyword
bool Food::matchKeyword(const std::string& keyword) const {
    for (const auto& kw : keywords) {
        if (kw == keyword) {
            return true;
        }
    }
    return false;
}

// Check if this food matches all the provided keywords
bool Food::matchAllKeywords(const std::vector<std::string>& searchKeywords) const {
    for (const auto& keyword : searchKeywords) {
        bool found = false;
        for (const auto& kw : keywords) {
            if (kw == keyword) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

// Check if this food matches any of the provided keywords
bool Food::matchAnyKeyword(const std::vector<std::string>& searchKeywords) const {
    for (const auto& keyword : searchKeywords) {
        for (const auto& kw : keywords) {
            if (kw == keyword) {
                return true;
            }
        }
    }
    return searchKeywords.empty(); // Return true if no keywords to search
}
Food Food::fromString(const std::string& str) {
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> tokens;
    
    // Split the string by semicolons
    while (std::getline(ss, token, ';')) {
        tokens.push_back(token);
    }
    
    // Check minimum required tokens
    if (tokens.size() < 3) {
        // Invalid format, return empty food
        return Food();
    }
    
    std::string id = tokens[0];
    
    // Parse keywords (split by commas)
    std::vector<std::string> kw;
    std::stringstream kwStream(tokens[1]);
    std::string keyword;
    while (std::getline(kwStream, keyword, ',')) {
        kw.push_back(keyword);
    }
    
    // Parse calories
    int calories = std::stoi(tokens[2]);
    
    // Basic food with 3 tokens
    if (tokens.size() == 3) {
        return Food(id, kw, calories);
    }
    
    // Check if it's a composite food
    bool isComp = (tokens.size() > 3 && tokens[3] == "1");
    
    // For now, create as basic food - components will need to be loaded separately
    Food food(id, kw, calories);
    
    // Mark as composite but don't add components here
    // Components need to be added after all foods are loaded from file
    
    return food;
}