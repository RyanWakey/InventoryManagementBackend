#include "../../include/validation/Validator.h"
#include "../../include/auth/AuthenticationService.h"
#include "../../include/routes/AuthenticationServiceRoutes.h"
#include "../../include/models/Customer.h"
#include "../../include/models/EmployeeRoles.h"

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

#include <jwt-cpp/jwt.h>

AuthenticationServiceRoutes::AuthenticationServiceRoutes(crow::App<crow::CORSHandler>& app) : app(app) {
    setupAuthenticationServiceRoutes();
}

void AuthenticationServiceRoutes::setupAuthenticationServiceRoutes() {
    
    CROW_ROUTE(app, "/register").methods("POST"_method)
    ([](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400, "Invalid JSON");

        std::string email = x["email"].s();
        std::string password = x["password"].s();

        if (AuthenticationService::emailExists(email)) {
            return crow::response(409, "Email already exists");
        }

        std::string hashedPassword = AuthenticationService::hashPassword(password);
        
        // createCustomer returns the new customer's ID or -1 on failure
        int newCustomerId = Customer::createCustomer(email, hashedPassword);

        if (newCustomerId > 0) {
            // Generate a token with the new customer's ID
            std::string token = AuthenticationService::generateToken(newCustomerId, "customer");

            crow::json::wvalue response;
            response["token"] = token;
            response["userId"] = newCustomerId; 
            response["userType"] = "customer";
            return crow::response(201, response);
        } else {
            return crow::response(500, "Account creation failed");
        }
    });


    CROW_ROUTE(app, "/login").methods("POST"_method)
    ([](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x || !x.has("userType") || !x.has("email") || !x.has("password")) {
            return crow::response(400, "Invalid JSON or missing fields");
        }

        std::string userType = x["userType"].s();
        std::string email = x["email"].s();
        std::string password = x["password"].s();

        if (AuthenticationService::validateCredentials(userType, email, password)) {
            int employeeId = AuthenticationService::fetchSpecificId(userType, email);
            std::string token = AuthenticationService::generateToken(employeeId, userType);

            auto roleIds = EmployeeRoles::readRolesForEmployee(employeeId);
            
            // Manually construct the JSON array for roles
            crow::json::wvalue rolesJson(crow::json::type::List);
            int index = 0;
            for (int roleId : roleIds) {
                Roles role = Roles::readRole(roleId); // Fetch each role by ID
                crow::json::wvalue roleJson;
                roleJson["RoleID"] = roleId;
                roleJson["RoleName"] = role.getName(); 
                rolesJson[index++] = std::move(roleJson); // Assign each role to the JSON array
            }

            crow::json::wvalue response;
            response["token"] = token;
            response["userType"] = userType;
            response["userId"] = employeeId;
            response["roles"] = std::move(rolesJson); // Include the roles JSON array in the response

            return crow::response(200, response);
        } else {
            return crow::response(401, "Invalid credentials");
        }
    });

}
