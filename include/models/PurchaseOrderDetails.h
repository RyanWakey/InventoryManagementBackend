#ifndef PURCHASE_ORDER_DETAILS_H
#define PURCHASE_ORDER_DETAILS_H

#include <boost/asio.hpp>
#include <crow.h>

#include <string>
#include <memory>
#include <vector>


class PurchaseOrderDetails {

private:
    int id;
    int quantity;
    double unitPrice;
    double totalPrice;
    int orderID;
    int productID;
    

public:
    PurchaseOrderDetails();
    PurchaseOrderDetails(int id, int quantity, double unitPrice, int orderID, int productID);
    PurchaseOrderDetails(int id, int quantity, double unitPrice, double totalPrice, int orderID, int productID);

    // Getters
    int getID() const { return id; }
    int getOrderID() const { return orderID; }
    int getProductID() const { return productID; }
    int getQuantity() const { return quantity; }
    double getUnitPrice() const { return unitPrice; }
    double getTotalPrice() const { return totalPrice; }

    // Setters
    void setID(int newId) { id = newId; }
    void setOrderID(int newOrderID) { orderID = newOrderID; }
    void setProductID(int newProductID) { productID = newProductID; }
    void setQuantity(int newQuantity) {
        quantity = newQuantity;
    }   

    void setUnitPrice(double newUnitPrice) {
        unitPrice = newUnitPrice;
    }

    void updateTotalPrice() {
        totalPrice = quantity * unitPrice;
    }  

    static PurchaseOrderDetails createPurchaseOrderDetail(int quantity, double unitPrice, double totalPrice, int orderID, int productID);
    static std::vector<PurchaseOrderDetails> readPurchaseOrderDetailByOrderID(int id);  

    static crow::json::wvalue convertDetailsToJson(const std::vector<PurchaseOrderDetails>& detailsList);
};

#endif