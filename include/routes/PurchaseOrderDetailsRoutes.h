#ifndef PURCHASE_ORDER_DETAILS_ROUTES_H
#define PURCHASE_ORDER_DETAILS_ROUTES_H

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>


class PurchaseOrderDetailsRoutes {
    
private:
    crow::App<crow::CORSHandler>& app;

public:
    explicit PurchaseOrderDetailsRoutes(crow::App<crow::CORSHandler>& app);
    void setupPurchaseOrderDetailsRoutes();
};

#endif

