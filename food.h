#ifndef FOOD_H
#define FOOD_H

#include <string>
#include <vector>

class Food {
private:
    std::string identifier;
    std::vector<std::string> keywords;
    int caloriesPerServing;
    bool isComposite;
    std::vector<Food> components;
    
public:
    // Constructors
    Food();
    Food(const std::string& id, const std::vector<std::string>& kw, int calories);
    // New constructor for composite foods
    Food(const std::string& id, const std::vector<Food>& components);
    
    // Getters
    std::string getIdentifier() const;
    std::vector<std::string> getKeywords() const;
    int getCaloriesPerServing() const;
    bool getIsComposite() const;
    const std::vector<Food>& getComponents() const;
    
    // Setters
    void setIdentifier(const std::string& id);
    void addKeyword(const std::string& keyword);
    void setKeywords(const std::vector<std::string>& kw);
    void setCaloriesPerServing(int calories);
    void addComponent(const Food& component);
    
    // Other methods
    bool matchKeyword(const std::string& keyword) const;
    bool matchAllKeywords(const std::vector<std::string>& searchKeywords) const;
    bool matchAnyKeyword(const std::vector<std::string>& searchKeywords) const;
    
    // For file operations
    std::string toString() const;
    static Food fromString(const std::string& str);
};

#endif // FOOD_H