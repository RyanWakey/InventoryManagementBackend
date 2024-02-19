#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <crow.h>

class Category {
private:

    int id;
    std::string name;
    std::string description;
    bool isActive;

public:

    Category(); 
    Category(int id, const std::string& name, const std::string& description, bool isActive);

    // Accessors
    int getId() const noexcept { return id; }
    const std::string& getName() const noexcept { return name; }
    const std::string& getDescription() const noexcept { return description; }
    bool getIsActive() const noexcept { return isActive; }

    // Mutators
    void setName(const std::string& newName) noexcept { name = newName; }
    void setDescription(const std::string& newDescription) noexcept { description = newDescription; }
    void setIsActive(bool newIsActive) noexcept { isActive = newIsActive; }

    static void createCategory(const std::string& name, const std::string& description, bool isActive);
    static void deleteCategory(int id);
    static void updateCategory(int id, const std::string& name, const std::string& description, bool isActive);
    static Category readCategory(int id);
    
    static std::vector<Category> getCategoriesFromDatabase();
    static crow::json::wvalue convertCategoryToJSON(const std::vector<Category>& categories);
};

#endif 