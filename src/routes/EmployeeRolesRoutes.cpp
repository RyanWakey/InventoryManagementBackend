#include "../../include/validation/Validator.h"
#include "../../include/models/EmployeeRoles.h"
#include "../../include/routes/EmployeeRolesRoutes.h"


#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

EmployeeRolesRoutes::EmployeeRolesRoutes(crow::App<crow::CORSHandler>& app) : app(app) {
    setupEmployeeRolesRoutes();
}

void EmployeeRolesRoutes::setupEmployeeRolesRoutes() {
    
    CROW_ROUTE(app, "/employee-roles/<int>").methods("GET"_method)
    ([this](int employeeId) {
        try {
            // Retrieve the roles for the given employee from the database
            auto roles = EmployeeRoles::readRolesForEmployee(employeeId);
            // Convert the roles to JSON format
            auto jsonRoles = EmployeeRoles::convertRolesToJSON(roles);
            return crow::response{jsonRoles};
        } catch (const std::exception& e) {
            return crow::response(500, "Failed to retrieve roles: " + std::string(e.what()));
        }
    });
    
    
    // Route for assigning a role to an employee
    CROW_ROUTE(app, "/employee-roles/assign").methods("POST"_method)
    ([this](const crow::request& req) {
        auto requestData = crow::json::load(req.body);
        if (!requestData) {
            return crow::response(400, "Invalid JSON");
        }

        int employeeId = requestData["EmployeeID"].i();
        int roleId = requestData["RoleID"].i();
        try {
            EmployeeRoles::createRoleToEmployee(employeeId, roleId);
            return crow::response(201, "Role assigned to employee successfully");
        } catch (const std::exception& e) {
            return crow::response(500, "Failed to assign role: " + std::string(e.what()));
        }
    });
    
     // Route for removing a role from an employee
    CROW_ROUTE(app, "/employee-roles/remove").methods("POST"_method)
    ([this](const crow::request& req) {
        auto requestData = crow::json::load(req.body);
        if (!requestData) {
            return crow::response(400, "Invalid JSON");
        }

        int employeeId = requestData["EmployeeID"].i();
        int roleId = requestData["RoleID"].i();
        try {
            EmployeeRoles::deleteRoleFromEmployee(employeeId, roleId);
            return crow::response(200, "Role removed from employee successfully");
        } catch (const std::exception& e) {
            return crow::response(500, "Failed to remove role: " + std::string(e.what()));
        }
    });  

     
}
