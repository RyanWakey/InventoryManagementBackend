#include "../../include/validation/Validator.h"
#include "../../include/models/Customer.h"
#include "../../include/routes/CustomerRoutes.h"


#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>


CustomerRoutes::CustomerRoutes(crow::App<crow::CORSHandler>& app) : app(app) {
    setupCustomerRoutes();
}

void CustomerRoutes::setupCustomerRoutes() {
    
    CROW_ROUTE(app, "/profile/customer/<int>").methods("GET"_method)
    ([](int customerId){
        try{
            auto customer = Customer::readCustomer(customerId);
            auto jsonCustomer = Customer::convertCustomerToJSON(customer);
            return crow::response(200, jsonCustomer);
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });

    CROW_ROUTE(app, "/profile/customer/<int>/update").methods("PATCH"_method)
    ([](const crow::request& req, int customerId) {
        auto updatedProfileData = crow::json::load(req.body);
        if (!updatedProfileData) return crow::response(400, "Invalid JSON format");

        try {
            std::string name = updatedProfileData["Name"].s();
            std::string contactDetails = updatedProfileData["ContactDetails"].s();
            std::string address = updatedProfileData["Address"].s();
            std::string city = updatedProfileData["City"].s();
            std::string country = updatedProfileData["Country"].s();

            bool updateSuccessful = Customer::updateCustomer(customerId, name, contactDetails, address, city, country);
            if (updateSuccessful) {
                return crow::response(200, "Customer updated successfully");
            } else {
                return crow::response(404, "Customer not found");
            }
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });

    
}
