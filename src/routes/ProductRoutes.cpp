#include "../../include/validation/Validator.h"
#include "../../include/models/Product.h"
#include "../../include/routes/ProductRoutes.h"

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

ProductRoutes::ProductRoutes(crow::App<crow::CORSHandler>& app) : app(app) {
    setupProductRoutes();
}

void ProductRoutes::setupProductRoutes() {
    
    CROW_ROUTE(app, "/products").methods("GET"_method)
    ([this] {
        try {
            auto products = Product::getProductsFromDatabase();
            auto jsonProducts = Product::convertProductsToJson(products);
            return  crow::response{jsonProducts};
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });

    CROW_ROUTE(app, "/products").methods("POST"_method)
    ([this](const crow::request& req){
        auto productData = crow::json::load(req.body);
        if(!productData){
            return crow::response(400, "Invalid JSON");
        }

        if(!Validator::validateName(productData["Name"].s())){
            return crow::response(400, "Invalid Name");
        }

        if(!Validator::validateDescription(productData["Description"].s())){
            return crow::response(400, "Invalid Description");
        }

        if (!Validator::validateDouble(productData["Price"].d())) {
            return crow::response(400, "Invalid Price");
        }

        if (!Validator::validateDouble(productData["Cost"].d())) {
            return crow::response(400, "Invalid Cost");
        }

        if (!Validator::validateDouble(productData["Weight"].d())) {
            return crow::response(400, "Invalid Weight");
        }

        if (!Validator::validateDouble(productData["Length"].d())) {
            return crow::response(400, "Invalid Length");
        }
        
        if (!Validator::validateDouble(productData["Width"].d())) {
            return crow::response(400, "Invalid Width");
        }
        
        if (!Validator::validateDouble(productData["Height"].d())) {
            return crow::response(400, "Invalid Height");
        }

        if (!Validator::validateStockQuantity(productData["StockQuantity"].i())) {
            return crow::response(400, "Invalid StockQuantity");
        }

        if(!(productData["isActive"].t() == crow::json::type::True || productData["IsActive"].t() == crow::json::type::False)){
           return crow::response(400, "Invalid Active Status");
        }

        if (!Validator::validateImageURL(productData["ImageURL"].s())){
            return crow::response(400, "Invalid Image URL");
        }

        if (!Validator::validateDate(productData["DateAdded"].s())) {
            return crow::response(400, "Invalid Date Added");
        }

        if (!Validator::validateDate(productData["DateModified"].s())) {
            return crow::response(400, "Invalid Date Modified");
        }

        try {

            Product newProduct = Product::createProduct(
            productData["Name"].s(),
            productData["Description"].s(),
            productData["Price"].d(),
            productData["Cost"].d(),
            productData["Weight"].d(),
            productData["Length"].d(),
            productData["Width"].d(),
            productData["Height"].d(),
            productData["StockQuantity"].i(),
            productData["isActive"].b(),
            productData["ImageURL"].s(),
            productData["DateAdded"].s(),
            productData["DateModified"].s(),
            productData["CategoriesID"].i()
            );

            crow::json::wvalue newProductJson = Product::convertProductToJson(newProduct);
            return  crow::response{201, newProductJson};

        } catch (const std::exception& e) {
            return crow::response(500, "Failed to add product to database: " + std::string(e.what()));
        }
    });

    CROW_ROUTE(app, "/products/<int>").methods("DELETE"_method)
    ([this](int productID) {
        try{
            Product::deleteProduct(productID);
            return crow::response(200, "Product deleted successfully");
        } catch (const std::exception& e){
            return crow::response(500, "Failed to delete product: " + std::string(e.what()));
        }
    });

    CROW_ROUTE(app, "/products/<int>").methods("PUT"_method)
    ([this](const crow::request& req, int productId) {
        auto updatedData = crow::json::load(req.body);
        if (!updatedData) {
            std::cerr << "THIS ONE 1" << std::endl;
            return crow::response(400, "Invalid JSON");
        }
        // Print the entire JSON object to the console
        std::cerr << "Received data: " << updatedData << std::endl;

        if(!Validator::validateName(updatedData["Name"].s())){
            std::cerr << "THIS ONE 2" << std::endl;
            return crow::response(400, "Invalid Name");
        }

        if(!Validator::validateDescription(updatedData["Description"].s())){
            std::cerr << "THIS ONE 3" << std::endl;
            return crow::response(400, "Invalid Description");
        }

        if (!Validator::validateDouble(updatedData["Price"].d())) {
            std::cerr << "THIS ONE 4" << std::endl;
            return crow::response(400, "Invalid Price");
        }

        if (!Validator::validateDouble(updatedData["Cost"].d())) {
            std::cerr << "THIS ONE 5" << std::endl;
            return crow::response(400, "Invalid Cost");
        }

        if (!Validator::validateDouble(updatedData["Weight"].d())) {
            std::cerr << "THIS ONE 5" << std::endl;
            return crow::response(400, "Invalid Weight");
        }

        if (!Validator::validateDouble(updatedData["Length"].d())) {
            std::cerr << "THIS ONE 6" << std::endl;
            return crow::response(400, "Invalid Length");
        }
        
        if (!Validator::validateDouble(updatedData["Width"].d())) {
            std::cerr << "THIS ONE 7" << std::endl;
            return crow::response(400, "Invalid Width");
        }
        
        if (!Validator::validateDouble(updatedData["Height"].d())) {
            std::cerr << "THIS ONE 8" << std::endl;
            return crow::response(400, "Invalid Height");
        }

        if (!Validator::validateStockQuantity(updatedData["StockQuantity"].i())) {
            std::cerr << "THIS ONE 9" << std::endl;
            return crow::response(400, "Invalid StockQuantity");
        }

        if(!(updatedData["isActive"].t() == crow::json::type::True || updatedData["IsActive"].t() == crow::json::type::False)){
            std::cerr << "THIS ONE 10" << std::endl;
           return crow::response(400, "Invalid Active Status");
        }

        if (!Validator::validateImageURL(updatedData["ImageURL"].s())){
            std::cerr << "THIS ONE 11" << std::endl;
            return crow::response(400, "Invalid Image URL");
        }

        if (!Validator::validateDate(updatedData["DateAdded"].s())) {
            std::cerr << "THIS ONE 12" << std::endl;
            return crow::response(400, "Invalid Date Added");
        }

        if (!Validator::validateDate(updatedData["DateModified"].s())) {
            std::cerr << "THIS ONE 13" << std::endl;
            return crow::response(400, "Invalid Date Modified");
        }

        try {
            Product::updateProduct(
                productId,
                updatedData["Name"].s(),
                updatedData["Description"].s(),
                updatedData["Price"].d(),
                updatedData["Cost"].d(),
                updatedData["Weight"].d(),
                updatedData["Length"].d(),
                updatedData["Width"].d(),
                updatedData["Height"].d(),
                updatedData["StockQuantity"].i(),
                updatedData["isActive"].b(), 
                updatedData["ImageURL"].s(),
                updatedData["DateAdded"].s(), 
                updatedData["DateModified"].s(),
                updatedData["CategoriesID"].i() 
            );
            return crow::response(200, "Product updated successfully");
        } catch (const std::exception& e){
            return crow::response(500, "Failed to update product: " + std::string(e.what()));
        }
    });

    CROW_ROUTE(app, "/products/low-stock").methods("GET"_method)
    ([this] {
        try {
            auto lowStockProducts = Product::getLowStockProducts();
            auto jsonResponse = Product::convertLowStockProductsToJson(lowStockProducts); 
            return crow::response(200, jsonResponse);
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving low stock product data: " << e.what();
            return crow::response(500, "Internal server error");
        }
    });

    CROW_ROUTE(app, "/products/table-info").methods("GET"_method)
    ([this] {
        try {
            auto productsInfo = Product::getProductTableInfo();
            auto jsonResponse = Product::convertProductsTableInfoToJson(productsInfo); 
            return crow::response(200, jsonResponse);
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving product table information: " << e.what();
            return crow::response(500, "Internal server error");
        }
    });

    CROW_ROUTE(app, "/products/<int>").methods("GET"_method)
    ([this] (int productID) {
        try {
            auto product = Product::getProductByID(productID);
            auto jsonProduct = Product::convertProductToJson(product.value());
            return  crow::response{jsonProduct};
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving product data: " << e.what();
            return crow::response(500, e.what());
        }
    });


    CROW_ROUTE(app, "/products/<int>/update-stock").methods("PATCH"_method)
    ([this](const crow::request& req, int productId) {
        auto reqBody = crow::json::load(req.body);
        if (!reqBody) {
            return crow::response(400, "Invalid JSON");
        }
        int quantityToAdd = reqBody["StockQuantity"].i();

        try {
            Product::updateProductStock(productId, quantityToAdd);
            return crow::response(200, "Stock updated successfully");
        } catch (const std::exception& e) {
            std::cerr << "Error updating product stock: " << e.what();
            return crow::response(500, "Internal Server Error");
        }
    });



}
