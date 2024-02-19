#ifndef EMPLOYEE_ROUTES_H
#define EMPLOYEE_ROUTES_H

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

class EmployeeRoutes {

private:
    crow::App<crow::CORSHandler>& app;

public:
    explicit EmployeeRoutes(crow::App<crow::CORSHandler>& app);
    void setupEmployeeRoutes();
};

#endif