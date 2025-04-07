#ifndef FOOD_DATABASE_H
#define FOOD_DATABASE_H

#include "food.h"
#include <vector>
#include <string>
#include <map>

class FoodDatabase {
private:
    std::vector<Food> foods;
    std::string databaseFilename;
    
    // Helper structure for loading composite foods
    struct CompositeRelation {
        std::string foodId;
        std::vector<std::string> componentIds;
    };
    
    // Helper methods for composite foods
    void processCompositeRelations(const std::vector<CompositeRelation>& relations);
    
public:
    // Constructor
    FoodDatabase(const std::string& filename = "foods.txt");
    
    // Database operations
    bool loadFromFile();
    bool saveToFile() const;
    
    // Food operations
    void addFood(const Food& food);
    bool removeFood(const std::string& identifier);
    Food* findFoodByIdentifier(const std::string& identifier);
    std::vector<Food> findFoodsByKeyword(const std::string& keyword) const;
    std::vector<Food> findFoodsByAllKeywords(const std::vector<std::string>& keywords) const;
    std::vector<Food> findFoodsByAnyKeyword(const std::vector<std::string>& keywords) const;
    
    // Composite food operations
    bool createCompositeFood(const std::string& name, const std::vector<std::string>& componentIds);
    
    // Other operations
    size_t size() const;
    const std::vector<Food>& getAllFoods() const;
};

#endif // FOOD_DATABASE_H