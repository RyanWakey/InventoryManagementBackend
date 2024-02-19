#include "../../include/models/Product.h"
#include "../../include/database/DB_Manager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  


Product::Product()  : id(0), price(0.0), cost(0.0), weight(0.0), length(0.0), width(0.0), 
                      height(0.0), stockQuantity(0), isActive(false), categoryId(0) { }

Product::Product(int id, const std::string& name, const std::string& description, double price, 
                 double cost, double weight, double length, double width, double height, 
                 int stockQuantity, bool isActive, const std::string& imageURL, 
                 const std::string& dateAdded, const std::string& dateModified, int categoryId)
    : id(id), name(name), description(description), price(price), cost(cost), 
      weight(weight), length(length), width(width), height(height), 
      stockQuantity(stockQuantity), isActive(isActive), imageURL(imageURL), 
      dateAdded(dateAdded), dateModified(dateModified), categoryId(categoryId) { }



Product Product::createProduct(const std::string& name, const std::string& description, double price, double cost, double weight, double length, double width, double height, int stockQuantity, bool isActive, const std::string& imageURL, const std::string& dateAdded, const std::string& dateModified, int categoryId) {
    int newProductId = 0;
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "INSERT INTO Products (Name, Description, Price, Cost, Weight, Length, Width, Height, StockQuantity, IsActive, ImageURL, DateAdded, DateModified, CategoriesID) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));

        // Bind data to the prepared statement
        pstmt->setString(1, name);
        pstmt->setString(2, description);
        pstmt->setDouble(3, price);
        pstmt->setDouble(4, cost);
        pstmt->setDouble(5, weight);
        pstmt->setDouble(6, length);
        pstmt->setDouble(7, width);
        pstmt->setDouble(8, height);
        pstmt->setInt(9, stockQuantity);
        pstmt->setBoolean(10, isActive);
        pstmt->setString(11, imageURL);
        pstmt->setString(12, dateAdded);
        pstmt->setString(13, dateModified);
        pstmt->setInt(14, categoryId);

        pstmt->executeUpdate();

        // Get the last inserted ID
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT LAST_INSERT_ID() as last_id"));
        if (res->next()) {
            newProductId = res->getInt("last_id");
        }

        Product newProduct(newProductId, name, description, price, cost, weight, length, width, height, stockQuantity, isActive, imageURL, dateAdded, dateModified, categoryId);

        return newProduct;
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in createProduct: " << e.what() << std::endl;
    }
    return Product();
}


void Product::deleteProduct(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM Products WHERE ProductID = ?"));

        pstmt->setInt(1, id);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in deleteProduct: " << e.what() << std::endl;
    }
}


void Product::updateProduct(int id, const std::string& name, const std::string& description, double price, double cost, double weight,
                            double length, double width, double height, int stockQuantity, bool isActive, const std::string& imageURL, 
                            const std::string& dateAdded, const std::string& dateModified, int categoryId) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE Products SET Name = ?, Description = ?, Price = ?, Cost = ?, Weight = ?, Length = ?, Width = ?, Height = ?, StockQuantity = ?, IsActive = ?, ImageURL = ?, DateAdded = ?, DateModified = ?, CategoriesID = ? WHERE ProductID = ?"));

        pstmt->setString(1, name);
        pstmt->setString(2, description);
        pstmt->setDouble(3, price);
        pstmt->setDouble(4, cost);
        pstmt->setDouble(5, weight);
        pstmt->setDouble(6, length);
        pstmt->setDouble(7, width);
        pstmt->setDouble(8, height);
        pstmt->setInt(9, stockQuantity);
        pstmt->setBoolean(10, isActive);
        pstmt->setString(11, imageURL);
        pstmt->setString(12, dateAdded);
        pstmt->setString(13, dateModified);
        pstmt->setInt(14, categoryId);
        pstmt->setInt(15, id); // The product ID is at the end in the WHERE clause

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in updateProduct: " << e.what() << std::endl;
    }
}

Product Product::readProduct(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM Products WHERE ProductID = ?"));

        pstmt->setInt(1, id);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            Product product(
                res->getInt("ProductID"),
                res->getString("Name"),
                res->getString("Description"),
                res->getDouble("Price"),
                res->getDouble("Cost"),
                res->getDouble("Weight"),
                res->getDouble("Length"),
                res->getDouble("Width"),
                res->getDouble("Height"),
                res->getInt("StockQuantity"),
                res->getBoolean("isActive"),
                res->getString("ImageURL"),
                res->getString("DateAdded"),
                res->getString("DateModified"),
                res->getInt("CategoriesID")
            );
            return product;
        } else {
            throw std::runtime_error("Product not found");
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in readProduct: " << e.what() << std::endl;
        throw;
    }
}

std::vector<Product> Product::getProductsFromDatabase() {
    std::vector<Product> products;

    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        // Typically want to explicit the column names in query (better practice - if schema changes etc.)
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM Products"));

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
             Product product(
                res->getInt("ProductID"),
                res->getString("Name"),
                res->getString("Description"),
                res->getDouble("Price"),
                res->getDouble("Cost"),
                res->getDouble("Weight"),
                res->getDouble("Length"),
                res->getDouble("Width"),
                res->getDouble("Height"),
                res->getInt("StockQuantity"),
                res->getBoolean("isActive"),
                res->getString("ImageURL"),
                res->getString("DateAdded"),
                res->getString("DateModified"),
                res->getInt("CategoriesID")
            );
            products.push_back(std::move(product));
        }

    } catch (sql::SQLException& e){
         std::cerr << "SQL Error in getProductsFromDatabase: " << e.what() << std::endl;
    }

    return products;
}



std::vector<Product::LowStockProductInfo> Product::getLowStockProducts() {
    std::vector<LowStockProductInfo> lowStockProducts;

    try {
        auto conn = DB_Manager::getConnection();
        auto pstmt = conn->prepareStatement(
            "SELECT ProductID, Name, StockQuantity "
            "FROM Products "
            "WHERE StockQuantity < ? "
            "ORDER BY StockQuantity ASC "
            "LIMIT 3"
        );
        
        pstmt->setInt(1, 10); // Assuming 10 is the threshold for low stock

        auto res = pstmt->executeQuery();
        
        while (res->next()) {
            LowStockProductInfo productInfo;
            productInfo.id = res->getInt("ProductID");
            productInfo.name = res->getString("Name");
            productInfo.stockQuantity = res->getInt("StockQuantity");
            
            lowStockProducts.push_back(productInfo);
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQLException in getLowStockProducts: " << e.what();
        // Handle better
    }

    return lowStockProducts;
}





std::vector<Product::ProductTableInfo> Product::getProductTableInfo() {
    
    std::vector<ProductTableInfo> productTableInfo;

    try {
        
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT ProductID, Name, Price, Cost, StockQuantity FROM Products"));

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        
        while (res->next()) {
            ProductTableInfo info;
            info.id = res->getInt("ProductID");
            info.name = res->getString("Name");
            info.price = res->getDouble("Price");
            info.cost = res->getDouble("Cost");
            info.quantity = res->getInt("StockQuantity");
            productTableInfo.push_back(info);
        }

    } catch (const sql::SQLException &e) {
        std::cerr << "SQLException in getProductTableInfo: " << e.what();
    }

    return productTableInfo;   
   
}  

std::optional<Product> Product::getProductByID(int productID) {

    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM Products WHERE ProductID = ?"));
        pstmt->setInt(1, productID);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
             Product foundProduct(
                res->getInt("ProductID"),
                res->getString("Name"),
                res->getString("Description"),
                res->getDouble("Price"),
                res->getDouble("Cost"),
                res->getDouble("Weight"),
                res->getDouble("Length"),
                res->getDouble("Width"),
                res->getDouble("Height"),
                res->getInt("StockQuantity"),
                res->getBoolean("isActive"),
                res->getString("ImageURL"),
                res->getString("DateAdded"),
                res->getString("DateModified"),
                res->getInt("CategoriesID")
            );
            return foundProduct;
        }

    } catch (sql::SQLException& e){
         std::cerr << "SQL Error in getProductsFromDatabase: " << e.what() << std::endl;
    }

    return std::nullopt;
}




void Product::updateProductStock(int productId, int quantityToAdd) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE Products SET StockQuantity = StockQuantity + ? WHERE ProductID = ?"));

        pstmt->setInt(1, quantityToAdd);
        pstmt->setInt(2, productId);

        pstmt->executeUpdate();
    } catch (const sql::SQLException& e) {
        std::cerr << "SQL Error in updateProductStock: " << e.what() << std::endl;
    }
}




crow::json::wvalue Product::convertProductToJson(const Product& product) {
    crow::json::wvalue jsonProduct;
    jsonProduct["ProductID"] = product.getId();
    jsonProduct["Name"] = product.getName();
    jsonProduct["Description"] = product.getDescription();
    jsonProduct["Price"] = product.getPrice();
    jsonProduct["Cost"] = product.getCost();
    jsonProduct["Weight"] = product.getWeight();
    jsonProduct["Length"] = product.getLength();
    jsonProduct["Width"] = product.getWidth();
    jsonProduct["Height"] = product.getHeight();
    jsonProduct["StockQuantity"] = product.getStockQuantity();
    jsonProduct["isActive"] = product.getIsActive();
    jsonProduct["ImageURL"] = product.getImageURL();
    jsonProduct["DateAdded"] = product.getDateAdded();
    jsonProduct["DateModified"] = product.getDateModified();
    jsonProduct["CategoriesID"] = product.getCategoryId();
    
    return jsonProduct;
}


crow::json::wvalue Product::convertLowStockProductsToJson(const std::vector<LowStockProductInfo>& lowStockProducts) {
    crow::json::wvalue jsonLowStockProducts(crow::json::type::List); // Initialize as a JSON array
    
    if (lowStockProducts.empty()) {
        jsonLowStockProducts["message"] = "All products are sufficiently stocked at this time.";
    } else {
        int index = 0;
        for (const auto& productInfo : lowStockProducts) {
            crow::json::wvalue jsonProduct;
            jsonProduct["id"] = productInfo.id;
            jsonProduct["name"] = productInfo.name;
            jsonProduct["stockQuantity"] = productInfo.stockQuantity;

            // Add the product to the array
            jsonLowStockProducts[index++] = std::move(jsonProduct);
        }
    }
    
    return jsonLowStockProducts;
}




crow::json::wvalue Product::convertProductsToJson(const std::vector<Product>& products) {
    crow::json::wvalue jsonProducts(crow::json::type::List); // Initialize as a JSON array

    int index = 0;
    for (const auto& product : products) {
        crow::json::wvalue jsonProduct;
        jsonProduct["ProductID"] = product.getId();
        jsonProduct["Name"] = product.getName();
        jsonProduct["Description"] = product.getDescription();
        jsonProduct["Price"] = product.getPrice();
        jsonProduct["Cost"] = product.getCost();
        jsonProduct["Weight"] = product.getWeight();
        jsonProduct["Length"] = product.getLength();
        jsonProduct["Width"] = product.getWidth();
        jsonProduct["Height"] = product.getHeight();
        jsonProduct["StockQuantity"] = product.getStockQuantity();
        jsonProduct["isActive"] = product.getIsActive();
        jsonProduct["ImageURL"] = product.getImageURL();
        jsonProduct["DateAdded"] = product.getDateAdded();
        jsonProduct["DateModified"] = product.getDateModified();
        jsonProduct["CategoriesID"] = product.getCategoryId();

        // Add the product to the array
        jsonProducts[index++] = std::move(jsonProduct);
    }

    return jsonProducts;
}


crow::json::wvalue Product::convertProductsTableInfoToJson(const std::vector<Product::ProductTableInfo>& products) {
    crow::json::wvalue jsonProducts(crow::json::type::List); // Initialize as a JSON array

    for (unsigned int i = 0; i < static_cast<unsigned int>(products.size()); i++) {
        crow::json::wvalue jsonProduct;
        jsonProduct["ProductID"] = products[i].id;
        jsonProduct["Name"] = products[i].name;
        jsonProduct["Price"] = products[i].price;
        jsonProduct["Cost"] = products[i].cost;
        jsonProduct["StockQuantity"] = products[i].quantity;
        
        jsonProducts[static_cast<unsigned int>(i)] = std::move(jsonProduct);
    }
    return jsonProducts;
}


Product::Product(Product&& other) noexcept
    : id(other.id),
      name(std::move(other.name)),
      description(std::move(other.description)),
      price(other.price),
      cost(other.cost),
      weight(other.weight),
      length(other.length),
      width(other.width),
      height(other.height),
      stockQuantity(other.stockQuantity),
      isActive(other.isActive),
      imageURL(std::move(other.imageURL)),
      dateAdded(std::move(other.dateAdded)),
      dateModified(std::move(other.dateModified)),
      categoryId(other.categoryId),
      category(std::move(other.category)) {
    
    // Set 'other' members to default values
    other.id = 0;
    other.price = 0.0;
    other.cost = 0.0;
    other.weight = 0.0;
    other.length = 0.0;
    other.width = 0.0;
    other.height = 0.0;
    other.stockQuantity = 0;
    other.isActive = false;
    other.categoryId = 0;
    // 'other.category' is already null after std::move
}


Product& Product::operator=(Product&& other) noexcept {
    if (this != &other) {
        id = other.id;
        name = std::move(other.name);
        description = std::move(other.description);
        price = other.price;
        cost = other.cost;
        weight = other.weight;
        length = other.length;
        width = other.width;
        height = other.height;
        stockQuantity = other.stockQuantity;
        isActive = other.isActive;
        imageURL = std::move(other.imageURL);
        dateAdded = std::move(other.dateAdded);
        dateModified = std::move(other.dateModified);
        categoryId = other.categoryId;
        category = std::move(other.category);

        // Set 'other' members to default values
        other.id = 0;
        other.price = 0.0;
        other.cost = 0.0;
        other.weight = 0.0;
        other.length = 0.0;
        other.width = 0.0;
        other.height = 0.0;
        other.stockQuantity = 0;
        other.isActive = false;
        other.categoryId = 0;
        // 'other.category' is already null after std::move
    }
    return *this;
}