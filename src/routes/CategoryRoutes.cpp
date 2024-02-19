#include "../../include/validation/Validator.h"
#include "../../include/models/Category.h"
#include "../../include/routes/CategoryRoutes.h"


#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

CategoryRoutes::CategoryRoutes(crow::App<crow::CORSHandler>& app) : app(app) {
    setupCategoryRoutes();
}

void CategoryRoutes::setupCategoryRoutes(){
   
   CROW_ROUTE(app, "/categories").methods("GET"_method)
    ([this] {
        try{
            auto categories = Category::getCategoriesFromDatabase();
            auto jsonCategories = Category::convertCategoryToJSON(categories);
            return crow::response{jsonCategories};
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });
    
    CROW_ROUTE(app, "/categories").methods("POST"_method)
    ([this](const crow::request& req) {
        auto categoryData = crow::json::load(req.body);
        if (!categoryData) {
            return crow::response(400, "Invalid JSON");
        }

        if (!Validator::validateName(categoryData["Name"].s())) {
            return crow::response(400, "Invalid Name");
        }

        if (!Validator::validateDescription(categoryData["Description"].s())) {
            return crow::response(400, "Invalid Description");
        }

        if (!(categoryData["IsActive"].t() == crow::json::type::True || categoryData["IsActive"].t() == crow::json::type::False)) {
            return crow::response(400, "Invalid Active Status");
        }

        try {
            Category::createCategory(
                categoryData["Name"].s(),
                categoryData["Description"].s(),
                categoryData["IsActive"].b()
            );
            
            return crow::response(201, "Category created successfully");
        } catch (const std::exception& e) {
            return crow::response(500, "Failed to add category to database: " + std::string(e.what()));
        }
    });


    CROW_ROUTE(app, "/categories/<int>").methods("DELETE"_method)
    ([this](int categoryID) {
        try {
            // Attempt to delete the category by its ID
            Category::deleteCategory(categoryID);
            // If no exception was thrown, assume successful deletion
            return crow::response(200, "Category deleted successfully");
        } catch (const std::exception& e) {
            std::cerr << "Error in deleting category: " << e.what() << std::endl;
            return crow::response(500, "Failed to delete category");
        }
    });


    CROW_ROUTE(app, "/categories/<int>").methods("PUT"_method)
   ([this](const crow::request& req, int categoryId) {
        auto updatedData = crow::json::load(req.body);
        if (!updatedData) {
            return crow::response(400, "Invalid JSON");
        }

        if (!Validator::validateName(updatedData["Name"].s())) {
            return crow::response(400, "Invalid Name");
        }

        if (!Validator::validateDescription(updatedData["Description"].s())) {
            return crow::response(400, "Invalid Description");
        }

        if (!(updatedData["IsActive"].t() == crow::json::type::True || updatedData["IsActive"].t() == crow::json::type::False)) {
            return crow::response(400, "Invalid Active Status");
        }

        try {
            Category::updateCategory(
                categoryId,
                updatedData["Name"].s(),
                updatedData["Description"].s(),
                updatedData["IsActive"].b()
            );

        return crow::response(200, "Category updated successfully");
        } catch (const std::exception& e) {
            return crow::response(500, "Failed to update category: " + std::string(e.what()));
        }
    });


}