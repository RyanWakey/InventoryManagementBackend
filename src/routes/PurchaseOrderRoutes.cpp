#include "../../include/validation/Validator.h"
#include "../../include/models/PurchaseOrder.h"
#include "../../include/routes/PurchaseOrderRoutes.h"

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

PurchaseOrderRoutes::PurchaseOrderRoutes(crow::App<crow::CORSHandler>& app) : app(app) {
    setupPurchaseOrderRoutes();
}

void PurchaseOrderRoutes::setupPurchaseOrderRoutes() {

    CROW_ROUTE(app, "/purchase-order/recent-purchase-order-info").methods("GET"_method)
    ([this] {
        try {
            auto recentOrders = PurchaseOrder::getRecentPurchaseOrders();
            auto jsonResponse = PurchaseOrder::convertRecentPurchaseOrdersToJson(recentOrders);
            return crow::response(200, jsonResponse);
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving recent purchase orders: " << e.what() << std::endl;
            return crow::response(500, "Internal server error");
        }
    });

    CROW_ROUTE(app, "/suppliers/<int>/purchaseorders").methods("GET"_method)
    ([this](int supplierId) {
        try {
            auto purchaseOrders = PurchaseOrder::getAllPurchaseOrdersForSupplierID(supplierId);
            auto jsonPurchaseOrders = PurchaseOrder::convertPurchaseOrdersBySupplierIDToJson(purchaseOrders);
            return crow::response(200, jsonPurchaseOrders);
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });     

    CROW_ROUTE(app, "/suppliers/<int>/create-purchase-order").methods("POST"_method)
    ([this](const crow::request& req, int supplierID) {
        auto purchaseOrderData = crow::json::load(req.body);
        if (!purchaseOrderData) {
            return crow::response(400, "Invalid JSON");
        }
        try {
            PurchaseOrder newOrder = PurchaseOrder::createPurchaseOrder(
                purchaseOrderData["OrderDate"].s(),
                purchaseOrderData["ExpectedDeliveryDate"].s(),
                PurchaseOrder::stringToOrderStatus(purchaseOrderData["Status"].s()), 
                purchaseOrderData["TotalAmount"].d(),
                purchaseOrderData["ReceivedDate"].s(),
                purchaseOrderData["Notes"].s(),
                supplierID
            );
   
            crow::json::wvalue newPurchaseOrderJSON = PurchaseOrder::convertPurchaseOrderToJson(newOrder);
            return crow::response(200, newPurchaseOrderJSON);
        } catch (const std::exception& e) {
            std::cerr << "Exception caught in route: " << e.what() << std::endl;
            return crow::response(500, "Failed to add purchase order to database: " + std::string(e.what()));
        }
    });
}