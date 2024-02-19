#include "../../include/validation/Validator.h"
#include "../../include/models/PurchaseOrderDetails.h"
#include "../../include/routes/PurchaseOrderDetailsRoutes.h"

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

PurchaseOrderDetailsRoutes::PurchaseOrderDetailsRoutes( crow::App<crow::CORSHandler>& app) : app(app) {
     setupPurchaseOrderDetailsRoutes();
}

void PurchaseOrderDetailsRoutes::setupPurchaseOrderDetailsRoutes() {

     CROW_ROUTE(app, "/suppliers/<int>/purchaseorders/<int>/details").methods("GET"_method)
     ([](int suppleirId, int selectedOrderId){
     try {
          auto orderDetails = PurchaseOrderDetails::readPurchaseOrderDetailByOrderID(selectedOrderId); 
          auto jsonDetails = PurchaseOrderDetails::convertDetailsToJson(orderDetails); 
          return crow::response(200, jsonDetails);
     } catch (const std::exception& e) {
          return crow::response(500, e.what());
     }
     });

     CROW_ROUTE(app, "/purchase-orders/<int>/details").methods("POST"_method)
     ([this](const crow::request& req, int orderID)  {
          auto detailsData = crow::json::load(req.body);
          
          if (!detailsData) {
            return crow::response(400, "Invalid JSON");
          }

          try {
               std::vector<PurchaseOrderDetails> detailsCreated; // To store the created details for response
               for (const auto& detailData : detailsData) {
                    int quantity = detailData["Quantity"].i();
                    double unitPrice = detailData["UnitPrice"].d();
                    double totalPrice = quantity * unitPrice; 
                    int productID = detailData["ProductID"].i();

                    // Create each purchase order detail entry
                    PurchaseOrderDetails newDetail = PurchaseOrderDetails::createPurchaseOrderDetail(
                         quantity, unitPrice, totalPrice, orderID, productID);
                    detailsCreated.push_back(newDetail);
               }
               
               crow::json::wvalue responseJson = PurchaseOrderDetails::convertDetailsToJson(detailsCreated);
               return crow::response(201, responseJson);
          } catch (const std::exception& e) {
               std::cerr << "Exception caught in route: " << e.what() << std::endl;
               return crow::response(500, "Failed to add purchase order details to database: " + std::string(e.what()));
          }
     });
}