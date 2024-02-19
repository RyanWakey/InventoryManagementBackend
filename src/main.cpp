// Make sure this is placed before definition of crow.h as they define this there to
// whatever first takes precedence.
#define CROW_STATIC_DIRECTORY "C:/Users/Ryan/Documents/Projects/Inventory Based Managment Code/InvProject/public/images/"
#define CROW_STATIC_ENDPOINT "/images/<path>"

#include "../include/database/DB_Manager.h"
#include "../include/routes/AuthenticationServiceRoutes.h"
#include "../include/routes/CategoryRoutes.h" 
#include "../include/routes/CustomerRoutes.h"
#include "../include/routes/EmployeeRoutes.h" 
#include "../include/routes/ProductRoutes.h" 
#include "../include/routes/EmployeeRolesRoutes.h" 
#include "../include/routes/SaleRoutes.h" 
#include "../include/routes/TransactionRoutes.h" 
#include "../include/routes/PurchaseOrderRoutes.h"
#include "../include/routes/PurchaseOrderDetailsRoutes.h"
#include "../include/routes/SupplierRoutes.h"

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_set>

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  // For Connection
#include <jdbc/cppconn/prepared_statement.h>  // For PreparedStatement

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

// #include <../external/libbcrypt-master/include/bcrypt/bcrypt.h>
// #include <../external/libbcrypt-master/include/bcrypt/BCrypt.hpp>




int main() {   
    
    // std::string password;
    // std::cout << "Enter password: ";
    // std::cin >> password;

    // std::string hashed = BCrypt::generateHash(password);
    // std::cout << "Hashed password: " << hashed << std::endl;
    
    
    DB_Manager::initialize();  

    crow::App<crow::CORSHandler> app;

    auto& cors = app.get_middleware<crow::CORSHandler>();

    
    cors.global().headers("Content-Type", "Accept").methods("POST"_method, "GET"_method, "DELETE"_method, "PUT"_method, "PATCH"_method)
                .prefix("/cors").origin("http://localhost:8080").allow_credentials();
       
    AuthenticationServiceRoutes authenticationServiceRoutes(app);
    CategoryRoutes categoryRotues(app);
    CustomerRoutes customerRoutes(app);
    ProductRoutes productRoutes(app);
    EmployeeRoutes employeeRoutes(app);
    EmployeeRolesRoutes employeeRolesRoutes(app);
    SaleRoutes SaleRoutes(app);
    TransactionRoutes TransactionRoutes(app);
    PurchaseOrderRoutes PurchaseOrderRoutes(app);
    PurchaseOrderDetailsRoutes PurchaseOrderDetailsRoutes(app);
    SupplierRoutes SupplierRoutes(app);

    CROW_ROUTE(app, "/")([]() {
        return "Hello, world!";
    });

    app.port(18080).multithreaded().run();
}