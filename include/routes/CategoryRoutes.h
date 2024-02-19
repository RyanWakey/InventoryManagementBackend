#ifndef CATEGORY_ROUTES_H
#define CATEGORY_ROUTES_H

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

class CategoryRoutes {
private:
    crow::App<crow::CORSHandler>& app;

public:
    explicit CategoryRoutes(crow::App<crow::CORSHandler>& app);
    void setupCategoryRoutes();
};

#endif