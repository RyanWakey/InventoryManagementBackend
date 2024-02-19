#include "../../include/models/PurchaseOrder.h"
#include "../../include/database/DB_Manager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  


PurchaseOrder::PurchaseOrder() : id(0), orderDate("0000-00-00"), expectedDeliveryDate("0000-00-00"), status(OrderStatus::Pending), totalAmount(0.0), 
                                 receivedDate("0000-00-00"), notes("Empty"), supplierId(0) { }

PurchaseOrder::PurchaseOrder(int id, const std::string& orderDate, const std::string& expectedDeliveryDate,
                             OrderStatus status, double totalAmount, const std::string& receivedDate, 
                             const std::string& notes, int supplierId)
    : id(id), orderDate(orderDate), expectedDeliveryDate(expectedDeliveryDate), status(status),
      totalAmount(totalAmount), receivedDate(receivedDate), notes(notes), supplierId(supplierId) { }

PurchaseOrder PurchaseOrder::createPurchaseOrder(const std::string& orderDate, const std::string& expectedDeliveryDate, OrderStatus status, double totalAmount, const std::string& receivedDate, const std::string& notes, int supplierId) {
    
    int newOrderId = 0; 
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        // Prepare the SQL statement for inserting the new order
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "INSERT INTO PurchaseOrders (OrderDate, ExpectedDeliveryDate, Status, TotalAmount, ReceivedDate, Notes, SupplierID) "
            "VALUES (?, ?, ?, ?, ?, ?, ?)"));

        // Bind values to the prepared statement
        pstmt->setString(1, orderDate);
        pstmt->setString(2, expectedDeliveryDate);
        pstmt->setString(3, statusToString(status)); 
        pstmt->setDouble(4, totalAmount);
        pstmt->setString(5, receivedDate);
        pstmt->setString(6, notes);
        pstmt->setInt(7, supplierId);

        pstmt->executeUpdate();

        // Retrieve the ID of the newly inserted order
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT LAST_INSERT_ID() as last_id"));
        if (res->next()) {
            newOrderId = res->getInt("last_id");
        }

        // Instantiate a new PurchaseOrder object with the retrieved ID and other details
        PurchaseOrder newOrder(newOrderId, orderDate, expectedDeliveryDate, status, totalAmount, receivedDate, notes, supplierId);

        return newOrder;
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in createPurchaseOrder: " << e.what() << std::endl;
    }
    return PurchaseOrder();
}

PurchaseOrder PurchaseOrder::readPurchaseOrder(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT * FROM PurchaseOrders WHERE OrderID = ?"));

        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            PurchaseOrder order(
                res->getInt("OrderID"),
                res->getString("OrderDate"),
                res->getString("ExpectedDeliveryDate"),
                stringToOrderStatus(res->getString("Status")),
                res->getDouble("TotalAmount"),
                res->getString("ReceivedDate"),
                res->getString("Notes"),
                res->getInt("SupplierID")
            );
            return order;
        } else {
            throw std::runtime_error("Purchase order not found");
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in readPurchaseOrder: " << e.what() << std::endl;
        throw;
    }
}

std::vector<PurchaseOrder> PurchaseOrder::getAllPurchaseOrders() {
    std::vector<PurchaseOrder> orders;
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM PurchaseOrders"));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            PurchaseOrder order(
                res->getInt("OrderID"),
                res->getString("OrderDate"),
                res->getString("ExpectedDeliveryDate"),
                stringToOrderStatus(res->getString("Status")),
                res->getDouble("TotalAmount"),
                res->getString("ReceivedDate"),
                res->getString("Notes"),
                res->getInt("SupplierID")
            );
            orders.push_back(order);
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in getAllPurchaseOrders: " << e.what() << std::endl;
    }
    return orders;
}

void PurchaseOrder::updatePurchaseOrder(int id, const std::string& newOrderDate, const std::string& newExpectedDeliveryDate, OrderStatus newStatus, double newTotalAmount, const std::string& newReceivedDate, const std::string& newNotes, int newSupplierId) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE PurchaseOrders SET OrderDate = ?, ExpectedDeliveryDate = ?, Status = ?, TotalAmount = ?, ReceivedDate = ?, Notes = ?, SupplierID = ? WHERE OrderID = ?"));

        pstmt->setString(1, newOrderDate);
        pstmt->setString(2, newExpectedDeliveryDate);
        pstmt->setString(3, PurchaseOrder::statusToString(newStatus)); 
        pstmt->setDouble(4, newTotalAmount);
        pstmt->setString(5, newReceivedDate);
        pstmt->setString(6, newNotes);
        pstmt->setInt(7, newSupplierId);
        pstmt->setInt(8, id);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in updatePurchaseOrder: " << e.what() << std::endl;
    }
}


void PurchaseOrder::deletePurchaseOrder(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM PurchaseOrders WHERE OrderID = ?"));

        pstmt->setInt(1, id);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in deletePurchaseOrder: " << e.what() << std::endl;
    }
}

std::string PurchaseOrder::statusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::Pending: return "Pending";
        case OrderStatus::Completed: return "Completed";
        case OrderStatus::Shipped: return "Shipped";
        case OrderStatus::Cancelled: return "Cancelled";
        default: return "Unknown";
    }
}


OrderStatus PurchaseOrder::stringToOrderStatus(const std::string& statusStr) {
    if (statusStr == "Pending") return OrderStatus::Pending;
    if (statusStr == "Completed") return OrderStatus::Completed;
    if (statusStr == "Shipped") return OrderStatus::Shipped;
    if (statusStr == "Cancelled") return OrderStatus::Cancelled;
    return OrderStatus::Pending; // Default value 
}


std::vector<PurchaseOrder::PurchaseOrderInfo> PurchaseOrder::getRecentPurchaseOrders() {
    std::vector<PurchaseOrder::PurchaseOrderInfo> recentOrders;
    try {
        auto conn = DB_Manager::getConnection();
        auto pstmt = conn->prepareStatement(
            "SELECT po.OrderID, po.OrderDate, po.ExpectedDeliveryDate, po.Status, s.Name "
            "FROM PurchaseOrders po "
            "JOIN Suppliers s ON po.SupplierID = s.SupplierID "
            "ORDER BY po.OrderDate DESC "
            "LIMIT 4" 
        );
        auto res = pstmt->executeQuery();

        while (res->next()) {
            PurchaseOrder::PurchaseOrderInfo info;
            info.id = res->getInt("OrderID");
            info.orderDate = res->getString("OrderDate");
            info.expectedDeliveryDate = res->getString("ExpectedDeliveryDate");
            info.status = stringToOrderStatus(res->getString("Status"));
            info.supplierName = res->getString("Name");
            recentOrders.push_back(info);
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQLException in getRecentPurchaseOrders: " << e.what();
        // Handle exceptions as appropriate for your application
        throw std::runtime_error("Database query failed");
    }
    return recentOrders;
}


std::vector<PurchaseOrder> PurchaseOrder::getAllPurchaseOrdersForSupplierID(int supplierId) {
    std::vector<PurchaseOrder> orders;
    
    try {
        auto conn = DB_Manager::getConnection(); 
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT * FROM PurchaseOrders WHERE SupplierID = ?"
        ));
        pstmt->setInt(1, supplierId);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            PurchaseOrder order(
                res->getInt("OrderID"),
                res->getString("OrderDate"),
                res->getString("ExpectedDeliveryDate"),
                stringToOrderStatus(res->getString("Status")),
                res->getDouble("TotalAmount"),
                res->getString("ReceivedDate"),
                res->getString("Notes"),
                res->getInt("SupplierID")  
            );
            orders.push_back(order);
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "SQLException in getAllPurchaseOrdersForSupplierID: " << e.what() << std::endl;
    }
    
    return orders;
}

crow::json::wvalue PurchaseOrder::convertPurchaseOrderToJson(const PurchaseOrder& purchaseOrder) {
    crow::json::wvalue jsonOrder;
    jsonOrder["orderId"] = purchaseOrder.id;
    jsonOrder["orderDate"] = purchaseOrder.orderDate;
    jsonOrder["expectedDeliveryDate"] = purchaseOrder.expectedDeliveryDate;
    jsonOrder["status"] = PurchaseOrder::statusToString(purchaseOrder.status); // Keeping it as "status" for simplicity
    jsonOrder["totalAmount"] = purchaseOrder.totalAmount;
    jsonOrder["receivedDate"] = purchaseOrder.receivedDate;
    jsonOrder["notes"] = purchaseOrder.notes;
    jsonOrder["supplierId"] = purchaseOrder.supplierId;

    return jsonOrder;
}


crow::json::wvalue PurchaseOrder::convertRecentPurchaseOrdersToJson(const std::vector<PurchaseOrder::PurchaseOrderInfo>& purchaseOrders) {
    crow::json::wvalue jsonPurchaseOrders(crow::json::type::List); // Initialize as a JSON array

    int index = 0;
    for (const auto& order : purchaseOrders) {
        crow::json::wvalue jsonOrder;
        jsonOrder["orderId"] = order.id;  // Changed from "id" to "orderId"
        jsonOrder["orderDate"] = order.orderDate;
        jsonOrder["expectedDeliveryDate"] = order.expectedDeliveryDate;
        jsonOrder["orderStatus"] = PurchaseOrder::statusToString(order.status); // Changed from "status" to "orderStatus"
        jsonOrder["supplierName"] = order.supplierName;
        jsonPurchaseOrders[index++] = std::move(jsonOrder);
    }

    return jsonPurchaseOrders;
}

crow::json::wvalue PurchaseOrder::convertPurchaseOrdersBySupplierIDToJson(const std::vector<PurchaseOrder>& orders) {
    crow::json::wvalue jsonOrders(crow::json::type::List); // Initialize as a JSON array

    int index = 0;
    for (const auto& order : orders) {
        crow::json::wvalue jsonOrder;
        jsonOrder["OrderID"] = order.getId();
        jsonOrder["OrderDate"] = order.getOrderDate();
        jsonOrder["ExpectedDeliveryDate"] = order.getExpectedDeliveryDate();
        jsonOrder["Status"] = PurchaseOrder::statusToString(order.status);
        jsonOrder["TotalAmount"] = order.getTotalAmount();
        jsonOrder["ReceivedDate"] = order.getReceivedDate();
        jsonOrder["Notes"] = order.getNotes();
        jsonOrder["SupplierID"] = order.getSupplierId();

        // Add the order to the JSON array
        jsonOrders[index++] = std::move(jsonOrder);
    }

    return jsonOrders;
}