#include "../../include/validation/Validator.h"
#include "../../include/models/Supplier.h"
#include "../../include/routes/SupplierRoutes.h"

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

SupplierRoutes::SupplierRoutes(crow::App<crow::CORSHandler>& app) : app(app) {
    setupSupplierRoutes();
}

void SupplierRoutes::setupSupplierRoutes() {
    
    CROW_ROUTE(app, "/supplier/active-suppliers").methods(crow::HTTPMethod::Get)
    ([this] {
        try {
            auto activeSuppliers = Supplier::getTotalSuppliersActive();
            crow::json::wvalue response;
            response["activeSuppliers"] = activeSuppliers;
            return crow::response(200, response);
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving total products sold data: " << e.what();
            return crow::response(500, "Internal server error");
        }
    });

    CROW_ROUTE(app, "/suppliers").methods("GET"_method)
    ([this] {
        try {
            auto suppliers = Supplier::getAllSuppliers();
            auto jsonSuppliers = Supplier::convertSuppliersToJson(suppliers);
            return crow::response(200, jsonSuppliers);
        } catch (const std::exception& e) {
            std::cerr << "Error retriving all suppliers: " << e.what();
            return crow::response(500, e.what());
        }
    });

}