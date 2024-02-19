#include "../../include/models/PurchaseOrderDetails.h"
#include "../../include/database/DB_Manager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  

PurchaseOrderDetails::PurchaseOrderDetails() : id(0), quantity(0), unitPrice(0.0), orderID(0), productID(0), totalPrice(0.0) {}

PurchaseOrderDetails::PurchaseOrderDetails(int id, int quantity, double unitPrice, int orderID, int productID) : 
                                           id(id), quantity(quantity), unitPrice(unitPrice), orderID(orderID), productID(productID), totalPrice(quantity*unitPrice){}


PurchaseOrderDetails::PurchaseOrderDetails(int id, int quantity, double unitPrice, double totalPrice, int orderID, int productID) : 
                                           id(id), quantity(quantity), unitPrice(unitPrice), totalPrice(totalPrice), orderID(orderID), productID(productID){}


PurchaseOrderDetails PurchaseOrderDetails::createPurchaseOrderDetail(int quantity, double unitPrice, double totalPrice, int orderID, int productID) {
    int newDetailId = 0;
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        // Prepare the SQL statement for inserting the new purchase order detail
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "INSERT INTO PurchaseOrderDetails (Quantity, UnitPrice, TotalPrice, OrderID, ProductID) "
            "VALUES (?, ?, ?, ?, ?)"));

        // Bind values to the prepared statement
        pstmt->setInt(1, quantity);
        pstmt->setDouble(2, unitPrice);
        pstmt->setDouble(3, totalPrice);
        pstmt->setInt(4, orderID);
        pstmt->setInt(5, productID);

        pstmt->executeUpdate();

        // Retrieve the ID of the newly inserted purchase order detail
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT LAST_INSERT_ID()"));
        if (res->next()) {
            newDetailId = res->getInt("LAST_INSERT_ID()");
        }

        // Instantiate a new PurchaseOrderDetails object with the retrieved ID and other details
        return PurchaseOrderDetails(newDetailId, quantity, unitPrice, totalPrice, orderID, productID);
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in createPurchaseOrderDetail: " << e.what() << std::endl;
    }
    return PurchaseOrderDetails();
} 


std::vector<PurchaseOrderDetails> PurchaseOrderDetails::readPurchaseOrderDetailByOrderID(int orderId) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT PurchaseOrderDetailsID, Quantity, UnitPrice, TotalPrice, OrderID, ProductID "
            "FROM PurchaseOrderDetails "
            "WHERE OrderID = ?"));
        pstmt->setInt(1, orderId);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        std::vector<PurchaseOrderDetails> detailsList;
        
        while (res->next()) {
            PurchaseOrderDetails detail(
                res->getInt("PurchaseOrderDetailsID"),
                res->getInt("Quantity"),
                res->getDouble("UnitPrice"),
                res->getDouble("TotalPrice"),
                res->getInt("OrderID"),
                res->getInt("ProductID")
            );
            detailsList.push_back(detail);
        }
        
        if (detailsList.empty()) {
            throw std::runtime_error("No PurchaseOrderDetails found for OrderID " + std::to_string(orderId));
        }

        return detailsList; 
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in readPurchaseOrderDetailByOrderID: " << e.what() << std::endl;
        throw;
    }
}


crow::json::wvalue PurchaseOrderDetails::convertDetailsToJson(const std::vector<PurchaseOrderDetails>& detailsList) {
    crow::json::wvalue jsonDetails(crow::json::type::List); // Initialize as a JSON array

    int index = 0;
    for (const auto& detail : detailsList) {
        crow::json::wvalue jsonDetail;
        jsonDetail["PurchaseOrderDetailsID"] = detail.getID();
        jsonDetail["Quantity"] = detail.getQuantity();
        jsonDetail["UnitPrice"] = detail.getUnitPrice();
        jsonDetail["TotalPrice"] = detail.getTotalPrice();
        jsonDetail["OrderID"] = detail.getOrderID();
        jsonDetail["ProductID"] = detail.getProductID();

        // Add the detail to the JSON array
        jsonDetails[index++] = std::move(jsonDetail);
    }

    return jsonDetails;
}