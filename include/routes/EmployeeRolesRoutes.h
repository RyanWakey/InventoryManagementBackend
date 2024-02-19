#ifndef EMPLOYEE_ROLES_H
#define EMPLOYEE_ROLES_H

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

class EmployeeRolesRoutes {
    
private:
    crow::App<crow::CORSHandler>& app;

public:
    explicit EmployeeRolesRoutes(crow::App<crow::CORSHandler>& app);
    void setupEmployeeRolesRoutes();
};

#endif // EMPLOYEE_ROUTES_H
