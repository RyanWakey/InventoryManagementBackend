#ifndef PURCHASE_ORDER_H
#define PURCHASE_ORDER_H

#include <boost/asio.hpp>
#include <crow.h>

#include "Supplier.h"
#include <string>
#include <memory>
#include <vector>

enum class OrderStatus {
    Pending,
    Completed,
    Shipped,
    Cancelled
};

class PurchaseOrder {

private:

    int id;
    std::string orderDate;
    std::string expectedDeliveryDate;
    OrderStatus status;
    double totalAmount;
    std::string receivedDate;
    std::string notes;
    int supplierId;
    std::shared_ptr<Supplier> supplier;

public:

     struct PurchaseOrderInfo {
        int id;
        std::string orderDate;
        std::string expectedDeliveryDate;
        OrderStatus status;
        double totalAmount;
        std::string receivedDate;
        std::string notes;
        std::string supplierName; 
    };


    PurchaseOrder();
    PurchaseOrder(int id, const std::string& orderDate, const std::string& expectedDeliveryDate,
                  OrderStatus status, double totalAmount, const std::string& receivedDate, 
                  const std::string& notes, int supplierId);

    int getId() const noexcept { return id; }
    const std::string& getOrderDate() const noexcept { return orderDate; }
    const std::string& getExpectedDeliveryDate() const noexcept { return expectedDeliveryDate; }
    OrderStatus getStatus() const noexcept { return status; }
    double getTotalAmount() const noexcept { return totalAmount; }
    const std::string& getReceivedDate() const noexcept { return receivedDate; }
    const std::string& getNotes() const noexcept { return notes; }
    int getSupplierId() const noexcept { return supplierId; }
    std::shared_ptr<Supplier> getSupplier() const noexcept { return supplier; }

    void setId(int newId) noexcept { id = newId; }
    void setOrderDate(const std::string& newOrderDate) noexcept { orderDate = newOrderDate; }
    void setExpectedDeliveryDate(const std::string& newExpectedDeliveryDate) noexcept { expectedDeliveryDate = newExpectedDeliveryDate; }
    void setStatus(OrderStatus newStatus) noexcept { status = newStatus; }
    void setTotalAmount(double newTotalAmount) noexcept { totalAmount = newTotalAmount; }
    void setReceivedDate(const std::string& newReceivedDate) noexcept { receivedDate = newReceivedDate; }
    void setNotes(const std::string& newNotes) noexcept { notes = newNotes; }
    void setSupplierId(int newSupplierId) noexcept { supplierId = newSupplierId; }
    void setSupplier(const std::shared_ptr<Supplier>& newSupplier) noexcept { supplier = newSupplier; }

    // CRUD Operations
    static PurchaseOrder createPurchaseOrder(const std::string& orderDate, const std::string& expectedDeliveryDate, OrderStatus status, double totalAmount, const std::string& receivedDate, const std::string& notes, int supplierId);
    static PurchaseOrder readPurchaseOrder(int id);
    static std::vector<PurchaseOrder> getAllPurchaseOrders();
    static std::vector<PurchaseOrder> getAllPurchaseOrdersForSupplierID(int supplierId);
    static void updatePurchaseOrder(int id, const std::string& newOrderDate, const std::string& newExpectedDeliveryDate, OrderStatus newStatus, double newTotalAmount, const std::string& newReceivedDate, const std::string& newNotes, int newSupplierId);
    static void deletePurchaseOrder(int id);    

    static std::string statusToString(OrderStatus status);
    static OrderStatus stringToOrderStatus(const std::string& statusStr);
    
    static std::vector<PurchaseOrderInfo> getRecentPurchaseOrders();

    static crow::json::wvalue PurchaseOrder::convertPurchaseOrderToJson(const PurchaseOrder& purchaseOrder);    
    static crow::json::wvalue convertRecentPurchaseOrdersToJson(const std::vector<PurchaseOrderInfo>& purchaseOrders);
    static crow::json::wvalue convertPurchaseOrdersBySupplierIDToJson(const std::vector<PurchaseOrder>& purchaseOrders);
};

#endif 
