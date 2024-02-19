#include "../../include/validation/Validator.h"
#include "../../include/models/Sale.h"
#include "../../include/routes/SaleRoutes.h"

#include <boost/asio.hpp>
#include <crow.h>
#include <crow/middlewares/cors.h>

SaleRoutes::SaleRoutes(crow::App<crow::CORSHandler>& app) : app(app) {
    setupSaleRoutes();
}

void SaleRoutes::setupSaleRoutes() {
    CROW_ROUTE(app, "/sales/count").methods(crow::HTTPMethod::Get)
    ([this] {
        try {
            int totalSalesCount = Sale::getTotalSalesMade();
            crow::json::wvalue response;
            response["totalSales"] = totalSalesCount;
            return crow::response(200, response);
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving total sales count: " << e.what() << std::endl;
            return crow::response(500, "Internal server error");
        }
    });
    
    CROW_ROUTE(app, "/sales/revenue").methods(crow::HTTPMethod::Get)
    ([this] {
        try {
            double totalRevenue = Sale::calculateTotalRevenue();
            crow::json::wvalue response;
            response["totalRevenue"] = totalRevenue;
            return crow::response(200, response);     
        } catch (const std::exception& e){
            std::cerr << "Error retrieving total revenue: " << e.what() << std::endl;
            return crow::response(500, "Internal server error");
        }
    });

    CROW_ROUTE(app, "/sales/monthly-revenue").methods(crow::HTTPMethod::Get)
    ([this] {
        try {
            auto monthlyRevenues = Sale::getMonthlyRevenue();
            auto jsonMonthlyRevenues = Sale::convertMonthlyRevenueToJson(monthlyRevenues);
            return crow::response(200, jsonMonthlyRevenues);
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving monthly revenue data: " << e.what();
            return crow::response(500, "Internal server error");
        }
    });

    
    CROW_ROUTE(app, "/sales/total-products-sold").methods(crow::HTTPMethod::Get)
    ([this] {
        try {
            auto totalProductsSold = Sale::calculateTotalProductsSold();
            crow::json::wvalue response;
            response["totalProductsSold"] = totalProductsSold;
            return crow::response(200, response);
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving total products sold data: " << e.what();
            return crow::response(500, "Internal server error");
        }
    });

    CROW_ROUTE(app, "/sales/top-6-products").methods(crow::HTTPMethod::Get)
    ([this] {
        try {
            auto topSellingProducts = Sale::getTopSellingProductsLastSixMonths();
            auto jsonTopSellingProducts = Sale::convertTopSellingProductsToJson(topSellingProducts);
            return crow::response(200, jsonTopSellingProducts);
        } catch (const std::exception& e) {
            std::cerr << "Error in /sales/top-6-products: " << e.what() << std::endl;
            return crow::response(500, "Internal server error");
        }
    });

    CROW_ROUTE(app, "/sales/most-popular-category-units").methods("GET"_method)
    ([this]{
        try {
            auto [mostPopularCategory, unitsSold] = Sale::getMostPopularCategoryAndUnitsSold();
            crow::json::wvalue response;
            response["mostPopularCategory"] = mostPopularCategory;
            response["unitsSold"] = unitsSold;
            return crow::response(200, response);
        } catch (const std::exception& e) {
            std::cerr << "Error retrieving most popular category and units sold: " << e.what() << std::endl;
            return crow::response(500, "Internal server error");
        }
    });

    CROW_ROUTE(app, "/sales/revenue-by-category").methods("GET"_method)
    ([this] {
        auto revenueByCategory = Sale::getRevenueByCategory();
        crow::json::wvalue response;
        for (const auto& [category, revenue] : revenueByCategory) {
            response["revenueByCategory"][category] = revenue;
        }
        return crow::response(200, response);
    });
    
    CROW_ROUTE(app, "/sales/units-sold-by-category").methods("GET"_method)
    ([this] {
        auto unitsSoldByCategory = Sale::getUnitsSoldByCategory();
        crow::json::wvalue response;
        for (const auto& [category, units] : unitsSoldByCategory) {
            response["unitsSoldByCategory"][category] = units;
        }
        return crow::response(200, response);
    });
} 