#ifndef TRANSACTION_ROUTES_H
#define TRANSACTION_ROUTES_H

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

class TransactionRoutes {

private:
    crow::App<crow::CORSHandler>& app;

public:
    explicit TransactionRoutes(crow::App<crow::CORSHandler>& app);
    void setupTransactionRoutes();
};

#endif