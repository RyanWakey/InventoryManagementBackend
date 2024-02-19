#ifndef SUPPLIER_ROUTES_H
#define SUPPLIER_ROUTES_H

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

class SupplierRoutes {

private:
    crow::App<crow::CORSHandler>& app;

public:
    explicit SupplierRoutes(crow::App<crow::CORSHandler>& app);
    void setupSupplierRoutes();
};

#endif