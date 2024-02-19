#ifndef SALE_ROUTES_H
#define SALE_ROUTES_H

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

class SaleRoutes {

private:
    crow::App<crow::CORSHandler>& app;

public:
    explicit SaleRoutes(crow::App<crow::CORSHandler>& app);
    void setupSaleRoutes();
};

#endif