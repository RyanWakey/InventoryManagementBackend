#include "../../include/validation/Validator.h"
#include "../../include/models/Transaction.h"
#include "../../include/routes/TransactionRoutes.h"

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

TransactionRoutes::TransactionRoutes(crow::App<crow::CORSHandler>& app) : app(app) {
    setupTransactionRoutes();
}

void TransactionRoutes::setupTransactionRoutes() {
    
    CROW_ROUTE(app, "/transactions/count").methods(crow::HTTPMethod::Get)
    ([this] {
        try {
            int TransactionCount = Transaction::getTransactionCount();
            crow::json::wvalue jsonResponse;
            jsonResponse["TransactionCount"] = TransactionCount;
            return crow::response(200, jsonResponse);
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving sales transaction count: " << e.what() << std::endl;
            return crow::response(500, "Internal server error");
        }
    });

    CROW_ROUTE(app, "/transactions/count-by-type").methods(crow::HTTPMethod::Get)
    ([this] {
        try {
            std::map<TransactionType, int> transactionCountsByType = Transaction::getTransactionCountsByType();
            crow::json::wvalue jsonResponse;

            for (const auto& pair : transactionCountsByType) {
                std::string typeStr = Transaction::transactionTypeToString(pair.first);
                jsonResponse[typeStr] = pair.second;
            }

            return crow::response(200, jsonResponse);
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving transaction counts by type: " << e.what() << std::endl;
            return crow::response(500, "Internal server error");
        }
    });


}