#ifndef CUSTOMER_ROUTES_H
#define CUSTOMER_ROUTES_H

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

class CustomerRoutes {
private:
    crow::App<crow::CORSHandler>& app;

public:
    explicit CustomerRoutes(crow::App<crow::CORSHandler>& app);
    void setupCustomerRoutes();
};

#endif
