#ifndef PRODUCT_H
#define PRODUCT_H

#include <boost/asio.hpp>
#include <crow.h>

#include <string>
#include <vector>
#include <memory>
#include "Category.h"

class Product {

private:

    int id;
    std::string name;
    std::string description;
    double price;
    double cost;
    double weight;
    double length;
    double width;
    double height;
    int stockQuantity;
    bool isActive;
    std::string imageURL;
    std::string dateAdded;
    std::string dateModified;
    int categoryId;
    std::unique_ptr<Category> category;

public:

    struct LowStockProductInfo {
        int id;
        std::string name;
        int stockQuantity;
    };

    struct ProductTableInfo {
        int id;
        std::string name;
        double price;
        double cost;
        int quantity;
    };

    Product(); 
    Product(int id, const std::string& name, const std::string& description, double price, double cost, double weight,
            double length, double width, double height, int stockQuantity, bool isActive, const std::string& imageURL, 
            const std::string& dateAdded, const std::string& dateModified, int categoryId);   

    inline int getId() const noexcept { return id; } 
    inline const std::string& getName() const noexcept { return name; }
    inline const std::string& getDescription() const noexcept { return description; }  
    inline double getPrice() const noexcept { return price; }
    inline double getCost() const noexcept { return cost; }
    inline double getWeight() const noexcept { return weight; }
    inline double getLength() const noexcept { return length; }
    inline double getWidth() const noexcept { return width; }
    inline double getHeight() const noexcept { return height; }
    inline int getStockQuantity() const noexcept { return stockQuantity; }
    inline bool getIsActive() const noexcept { return isActive; }
    inline const std::string& getImageURL() const noexcept { return imageURL; }
    inline const std::string& getDateAdded() const noexcept { return dateAdded; }
    inline const std::string& getDateModified() const noexcept { return dateModified; }
    inline int getCategoryId() const noexcept { return categoryId; }
    inline const Category* getCategory() const { return category.get(); }

    inline void setId(int id_) noexcept { id = id_; }
    inline void setName(const std::string& name_) noexcept { name = name_; }
    inline void setDescription(const std::string& description_) noexcept { description = description_; }
    inline void setPrice(double price_) noexcept { price = price_; }
    inline void setCost(double cost_) noexcept { cost = cost_; }
    inline void setWeight(double weight_) noexcept { weight = weight_; }
    inline void setLength(double length_) noexcept { length = length_; }
    inline void setWidth(double width_) noexcept { width = width_; }
    inline void setHeight(double height_) noexcept { height = height_; }
    inline void setStockQuantity(int stockQuantity_) noexcept { stockQuantity = stockQuantity_; }
    inline void setIsActive(bool isActive_) noexcept { isActive = isActive_; }
    inline void setImageURL(const std::string& imageURL_) noexcept { imageURL = imageURL_; }
    inline void setDateAdded(const std::string& dateAdded_) noexcept { dateAdded = dateAdded_; }
    inline void setDateModified(const std::string& dateModified_) noexcept { dateModified = dateModified_; }
    inline void setCategoryId(int categoryId_) noexcept { categoryId = categoryId_; }
    inline void setCategory(std::unique_ptr<Category> newCategory) { category = std::move(newCategory); }

    static Product createProduct(const std::string& name, const std::string& description, double price, double cost, double weight,
                                    double length, double width, double height, int stockQuantity, bool isActive, const std::string& imageURL, 
                                    const std::string& dateAdded, const std::string& dateModified, int categoryId);
    static void deleteProduct(int id);                                
    static void updateProduct(int id, const std::string& name, const std::string& description, double price, double cost, double weight,
                                     double length, double width, double height, int stockQuantity, bool isActive, const std::string& imageURL, 
                                     const std::string& dateAdded, const std::string& dateModified, int categoryId);
    static Product readProduct(int id);
    
    static std::vector<Product> getProductsFromDatabase();
    static std::vector<LowStockProductInfo> getLowStockProducts();
    static std::vector<ProductTableInfo> getProductTableInfo();
    static std::optional<Product> Product::getProductByID(int productID);
    static void updateProductStock(int productId, int quantityToAdd);    

    static crow::json::wvalue convertProductsToJson(const std::vector<Product>& products);
    static crow::json::wvalue convertLowStockProductsToJson(const std::vector<LowStockProductInfo>& lowStockProducts);
    static crow::json::wvalue convertProductsTableInfoToJson(const std::vector<ProductTableInfo>& products);
    static crow::json::wvalue Product::convertProductToJson(const Product& product);
    // Move constructor
    Product(Product&& other) noexcept;

    // Move assignment operator
    Product& operator=(Product&& other) noexcept;

    // Delete copy constructor and copy assignment operator
    Product(const Product&) = delete;
    Product& operator=(const Product&) = delete;
};

#endif