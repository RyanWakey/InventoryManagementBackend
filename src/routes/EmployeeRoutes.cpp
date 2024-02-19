#include "../../include/validation/Validator.h"
#include "../../include/models/Employee.h"
#include "../../include/routes/EmployeeRoutes.h"

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

EmployeeRoutes::EmployeeRoutes(crow::App<crow::CORSHandler>& app) : app(app){
    setupEmployeeRoutes();
}

void EmployeeRoutes::setupEmployeeRoutes(){

    CROW_ROUTE(app, "/employees").methods("GET"_method)
    ([this] {
        try {
            auto employees = Employee::getEmployeesFromDatabase();
            auto jsonEmployees = Employee::convertEmployeesToJSON(employees);
            return crow::response{jsonEmployees};
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });


    CROW_ROUTE(app, "/employees").methods("POST"_method)
    ([this](const crow::request& req) {
        auto employeeData = crow::json::load(req.body);
        if (!employeeData) {
            return crow::response(400, "Invalid JSON");
        }

        if (!Validator::validateName(employeeData["FirstName"].s())) {
            return crow::response(400, "Invalid First Name");
        }

        if (!Validator::validateName(employeeData["LastName"].s())) {
            return crow::response(400, "Invalid Lirst Name");
        }

        if (!Validator::validateDate(employeeData["DOB"].s())) {
            return crow::response(400, "Invalid Date of Birth");
        }
        
        std::string genderStr = employeeData["Gender"].s();
        if (!Validator::validateGender(genderStr)) {
            return crow::response(400, "Invalid Gender");
        }

        if (!Validator::validateAddress(employeeData["Address"].s())) {
            return crow::response(400, "Invalid Address");
        }

        if (!Validator::validateCity(employeeData["City"].s())) {
            return crow::response(400, "Invalid City");
        }

        if (!Validator::validatePostalCode(employeeData["PostalCode"].s())) {
            return crow::response(400, "Invalid Postal Code");
        }

        if (!Validator::validatePhoneNumber(employeeData["Phone"].s())) {
            return crow::response(400, "Invalid Phone Number");
        }

        if (!Validator::validateProfilePicture(employeeData["ProfilePicture"].s())) {
            return crow::response(400, "Invalid Profile Picture");
        }

        if (!Validator::validateDate(employeeData["HireDate"].s())) {
            return crow::response(400, "Invalid Hire Date");
        }

        if (!Validator::validateEmail(employeeData["Email"].s())) {
            return crow::response(400, "Invalid Email");
        }

        try {
            
            Employee::createEmployee(
                employeeData["FirstName"].s(),
                employeeData["LastName"].s(),
                employeeData["DOB"].s(),
                genderStr[0], 
                employeeData["Address"].s(),
                employeeData["City"].s(),
                employeeData["PostalCode"].s(),
                employeeData["Phone"].s(),
                employeeData["ProfilePicture"].s(),
                employeeData["HireDate"].s(),
                employeeData["Email"].s(),
                employeeData["PasswordHash"].s()
            );

            return crow::response(201, "Employee created successfully");
        } catch (const std::exception& e) {
            return crow::response(500, "Failed to create employee: " + std::string(e.what()));
        }
    });
        
    CROW_ROUTE(app, "/employees/<int>").methods("DELETE"_method)
    ([this](int employeeID) {
        try {
            // Attempt to delete the employee by their ID
            Employee::deleteEmployee(employeeID);
            // If no exception was thrown, assume successful deletion
            return crow::response(200, "Employee deleted successfully");
        } catch (const std::exception& e) {
            std::cerr << "Error in deleting employee: " << e.what() << std::endl;
            return crow::response(500, "Failed to delete employee");
        }
    });
        
    CROW_ROUTE(app, "/employees/<int>").methods("PUT"_method)
    ([this](const crow::request& req, int employeeID) {
        auto employeeData = crow::json::load(req.body);
        if (!employeeData) {
            return crow::response(400, "Invalid JSON");
        }
        if (!Validator::validateName(employeeData["FirstName"].s())) {
            return crow::response(400, "Invalid First Name");
        }

        if (!Validator::validateName(employeeData["LastName"].s())) {
            return crow::response(400, "Invalid Lirst Name");
        }

        if (!Validator::validateDate(employeeData["DOB"].s())) {
            return crow::response(400, "Invalid Date of Birth");
        }
        
        std::string genderStr = employeeData["Gender"].s();
        if (!Validator::validateGender(genderStr)) {
            return crow::response(400, "Invalid Gender");
        }

        if (!Validator::validateAddress(employeeData["Address"].s())) {
            return crow::response(400, "Invalid Address");
        }

        if (!Validator::validateCity(employeeData["City"].s())) {
            return crow::response(400, "Invalid City");
        }

        if (!Validator::validatePostalCode(employeeData["PostalCode"].s())) {
            return crow::response(400, "Invalid Postal Code");
        }

        if (!Validator::validatePhoneNumber(employeeData["Phone"].s())) {
            return crow::response(400, "Invalid Phone Number");
        }

        if (!Validator::validateProfilePicture(employeeData["ProfilePicture"].s())) {
            return crow::response(400, "Invalid Profile Picture");
        }

        if (!Validator::validateDate(employeeData["HireDate"].s())) {
            return crow::response(400, "Invalid Hire Date");
        }

        if (!Validator::validateEmail(employeeData["Email"].s())) {
            return crow::response(400, "Invalid Email");
        }


        try {
            Employee::updateEmployee(
                employeeID, // Use the employeeID from the route parameter
                employeeData["FirstName"].s(),
                employeeData["LastName"].s(),
                employeeData["DOB"].s(),
                genderStr[0],
                employeeData["Address"].s(),
                employeeData["City"].s(),
                employeeData["PostalCode"].s(),
                employeeData["Phone"].s(),
                employeeData["ProfilePicture"].s(),
                employeeData["HireDate"].s(),
                employeeData["Email"].s()
            );

            return crow::response(200, "Employee updated successfully");
        } catch (const std::exception& e) {
            return crow::response(500, "Failed to update employee: " + std::string(e.what()));
        }
    });

    CROW_ROUTE(app, "/profile/employee/<int>").methods("GET"_method)
    ([](int employeeId){
        try{
            auto employee = Employee::readEmployee(employeeId);
            auto jsonEmployee = Employee::convertEmployeeToJSON(employee);
            return crow::response(200, jsonEmployee);
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });


}       
