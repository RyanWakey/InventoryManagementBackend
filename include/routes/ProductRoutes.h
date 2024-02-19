#ifndef PRODUCT_ROUTES_H
#define PRODUCT_ROUTES_H

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

class ProductRoutes {
private:
    crow::App<crow::CORSHandler>& app;

public:
    explicit ProductRoutes(crow::App<crow::CORSHandler>& app);
    void setupProductRoutes();
};

#endif
