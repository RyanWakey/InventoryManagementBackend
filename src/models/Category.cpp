#include "../../include/models/Category.h"
#include "../../include/database/DB_Manager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  

#include <boost/asio.hpp>
#include <crow.h>




Category::Category() : id(0), name(""), description(""), isActive(false) { }

Category::Category(int id, const std::string& name, const std::string& description, bool isActive) 
                     : id(id), name(name), description(description), isActive(isActive) { }

void Category::createCategory(const std::string& name, const std::string& description, bool isActive) {
    try {

        // Get DB connection from DB Manager
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        // Prepare SQL statement
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "INSERT INTO Categories (Name,Description,IsActive) VALUES (?, ?, ?)"));

        // Bind the data to the statement
        pstmt->setString(1, name);
        pstmt->setString(2, description);
        pstmt->setBoolean(3, isActive);    

        // Execute the statement
        pstmt->executeUpdate();

    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in createCategory: " << e.what() << std::endl;
        // Should handle this more appropriately.
    }
    
}

void Category::deleteCategory(int id){
    try {   
        // Get the database connection from DB_Manager
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
            
        // Prepare the SQL statement for deleting the category    
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM Categories WHERE CategoriesID = ?"));

        // Bind the id to the statement
        pstmt->setInt(1,id);

        // Execute the statement
        pstmt->executeUpdate();

    } catch (const sql::SQLException& e) {
        std::cerr << "SQL Error in deleteCategory: " << e.what() << std::endl;
    }
}

void Category::updateCategory(int id, const std::string& name, const std::string& description, bool isActive) {
    
    try{
        // Get the db connection from DB_Manager
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        // Prepare the SQL statement for updating the category
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("UPDATE Categories SET Name = ?, Description = ?, IsActive = ? WHERE CategoriesID = ?"));
        
        // Bind the data to the statement
        pstmt->setString(1, name);
        pstmt->setString(2, description);
        pstmt->setBoolean(3, isActive);
        pstmt->setInt(4, id);

        // Execute the statement
        pstmt->executeUpdate();

    } catch (const sql::SQLException& e) {
        std::cerr << "SQL Error in updateCategory: " << e.what() << std::endl;
    }
}

Category Category::readCategory(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT CategoriesID, Name, Description, IsActive FROM Categories WHERE CategoriesID = ?"));

        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            // Create a Category object and fill it with data from the database
            Category category(
                res->getInt("CategoriesID"),
                res->getString("Name"),
                res->getString("Description"),
                res->getBoolean("IsActive")
            );
            return category;
        } else {
            throw std::runtime_error("Category not found");
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in readCategory: " << e.what() << std::endl;
        throw; // Re-throw the exception for the caller to handle
    }
}

std::vector<Category> Category::getCategoriesFromDatabase() {
    std::vector<Category> categories;

    try {
        auto conn = DB_Manager::getConnection(); 

        auto stmt = conn->createStatement();
        auto res = stmt->executeQuery("SELECT * FROM Categories");

        while (res->next()) {
            Category category(
                res->getInt("CategoriesID"),
                res->getString("Name"),
                res->getString("Description"),
                res->getBoolean("IsActive")
        );
        categories.push_back(std::move(category));
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "SQL Error in getCategoriesFromDatabase: " << e.what() << std::endl;
    }

        return categories;
    }

crow::json::wvalue Category::convertCategoryToJSON(const std::vector<Category>& categories) {
    crow::json::wvalue jsonCategories(crow::json::type::List); // Initialize as a JSON array

    int index = 0;
    for (const auto& category : categories) {
        crow::json::wvalue jsonCategory;
        jsonCategory["CategoriesID"] = category.getId();             
        jsonCategory["Name"] = category.getName();         
        jsonCategory["Description"] = category.getDescription(); 
        jsonCategory["IsActive"] = category.getIsActive(); 

        // Add the category to the array
        jsonCategories[index++] = std::move(jsonCategory);
    }
    return jsonCategories;
}
