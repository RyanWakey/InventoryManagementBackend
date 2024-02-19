#ifndef AUTHENTICATION_SERVICE_ROUTES_H
#define AUTHENTICATION_SERVICE_ROUTES_H

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

class AuthenticationServiceRoutes {
private:
    crow::App<crow::CORSHandler>& app;

public:
    explicit AuthenticationServiceRoutes(crow::App<crow::CORSHandler>& app);
    void setupAuthenticationServiceRoutes();
};

#endif