#ifndef SALES_H
#define SALES_H

#include <string>
#include <memory>
#include <vector>
#include <iostream>

#include "Customer.h"
#include "Product.h"


struct MonthlyRevenue {
    int year;
    int month;
    double totalRevenue;
};

struct ProductSales {
    std::string name;
    unsigned int quantity;
};


class Sale {


private:

    int id;
    int quantity;
    std::string saleDate; 
    double totalPrice;

    int productId; // Foreign key to Product
    int customerId; // Foreign key to Customer
    // Not even needed
    std::shared_ptr<Product> product; // Associated Product object
    std::shared_ptr<Customer> customer; // Associated Customer object

    std::vector<MonthlyRevenue> calculateMonthlyRevenue();

public:

    Sale();
    Sale(int id, int quantity, const std::string& saleDate, double totalPrice, int productId, int customerId);


    int getId() const noexcept { return id; }
    int getQuantity() const noexcept { return quantity; }
    const std::string& getSaleDate() const noexcept { return saleDate; }
    double getTotalPrice() const noexcept { return totalPrice; }
    int getProductId() const noexcept { return productId; }
    int getCustomerId() const noexcept { return customerId; }

    // Accessors for the associated objects
    std::shared_ptr<Product> getProduct() const noexcept { return product; }
    std::shared_ptr<Customer> getCustomer() const noexcept { return customer; }

    void setQuantity(int newQuantity) noexcept { quantity = newQuantity; }
    void setSaleDate(const std::string& newSaleDate) noexcept { saleDate = newSaleDate; }
    void setTotalPrice(double newTotalPrice) noexcept { totalPrice = newTotalPrice; }
    
    // Mutators for the associated objects
    
    void setProductId(int productId_) noexcept { productId = productId_; }
    void setProduct(const std::shared_ptr<Product>& newProduct) noexcept { product = newProduct; }
    void setCustomerId(int customerId_) noexcept { customerId = customerId_ ; }
    void setCustomer(const std::shared_ptr<Customer>& newCustomer) noexcept { customer = newCustomer; }

    static void createSale(int quantity, const std::string& saleDate, double totalPrice, int productId, int customerId);
    static Sale readSale(int id);
    static std::vector<Sale> getAllSales();
    static void updateSale(int id, int newQuantity, const std::string& newSaleDate, double newTotalPrice, int newProductId, int newCustomerId);
    static void deleteSale(int id);

    static int getTotalSalesMade();
    static double calculateTotalRevenue();
    static std::vector<MonthlyRevenue> getMonthlyRevenue();
    static int calculateTotalProductsSold();
    static std::pair<std::string, int> getMostPopularCategoryAndUnitsSold();
    static std::vector<ProductSales> getTopSellingProductsLastSixMonths();
    static std::map<std::string, double> Sale::getRevenueByCategory();
    static std::map<std::string, int> Sale::getUnitsSoldByCategory();

    static crow::json::wvalue convertMonthlyRevenueToJson(const std::vector<MonthlyRevenue>& monthlyRevenues);
    static crow::json::wvalue convertTopSellingProductsToJson(const std::vector<ProductSales>& topSellingProducts);

};

#endif 
