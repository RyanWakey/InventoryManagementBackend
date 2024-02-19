#ifndef PURCHASE_ORDER__ROUTES_H
#define PURCHASE_ORDER__ROUTES_H

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

class PurchaseOrderRoutes {

private:
    crow::App<crow::CORSHandler>& app;

public:
    explicit PurchaseOrderRoutes(crow::App<crow::CORSHandler>& app);
    void setupPurchaseOrderRoutes();
};

#endif


