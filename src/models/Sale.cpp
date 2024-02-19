#include "../../include/models/Sale.h"
#include "../../include/database/DB_Manager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  


Sale::Sale() : id(0), quantity(0),  saleDate(""), totalPrice(0.0) { }

Sale::Sale(int id, int quantity, const std::string& saleDate, double totalPrice, int productId, int customerId) : 
    id(id), quantity(quantity), saleDate(saleDate), totalPrice(totalPrice), productId(productId), customerId(customerId) { }


void Sale::createSale(int quantity, const std::string& saleDate, double totalPrice, int productId, int customerId) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "INSERT INTO Sales (Quantity, SaleDate, TotalPrice, ProductID, CustomerID) VALUES (?, ?, ?, ?, ?)"));

        pstmt->setInt(1, quantity);
        pstmt->setString(2, saleDate);
        pstmt->setDouble(3, totalPrice);
        pstmt->setInt(4, productId);
        pstmt->setInt(5, customerId);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in createSale: " << e.what() << std::endl;
    }
}


Sale Sale::readSale(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT * FROM Sales WHERE SaleID = ?"));

        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            Sale sale(
                res->getInt("SaleID"),
                res->getInt("Quantity"),
                res->getString("SaleDate"),
                res->getDouble("TotalPrice"),
                res->getInt("ProductID"),
                res->getInt("CustomerID")
            );
            return sale;
        } else {
            throw std::runtime_error("Sale not found");
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in readSale: " << e.what() << std::endl;
        throw;
    }
}

std::vector<Sale> Sale::getAllSales() {
    std::vector<Sale> sales;
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM Sales"));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            Sale sale(
                res->getInt("SaleID"),
                res->getInt("Quantity"),
                res->getString("SaleDate"),
                res->getDouble("TotalPrice"),
                res->getInt("ProductID"),
                res->getInt("CustomerID")
            );
            sales.push_back(sale);
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in getAllSales: " << e.what() << std::endl;
    }
    return sales;
}


void Sale::updateSale(int id, int newQuantity, const std::string& newSaleDate, double newTotalPrice, int newProductId, int newCustomerId) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE Sales SET Quantity = ?, SaleDate = ?, TotalPrice = ?, ProductID = ?, CustomerID = ? WHERE SaleID = ?"));

        pstmt->setInt(1, newQuantity);
        pstmt->setString(2, newSaleDate);
        pstmt->setDouble(3, newTotalPrice);
        pstmt->setInt(4, newProductId);
        pstmt->setInt(5, newCustomerId);
        pstmt->setInt(6, id);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in updateSale: " << e.what() << std::endl;
    }
}


void Sale::deleteSale(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM Sales WHERE SaleID = ?"));

        pstmt->setInt(1, id);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in deleteSale: " << e.what() << std::endl;
    }
}


int Sale::getTotalSalesMade() {
    
    try {

        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT COUNT(*) AS totalSales FROM Sales"));

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()){
            return res->getInt("totalSales");
        }

    } catch (sql::SQLException &e){
        std::cerr << "SQLException in getTotalSalesMade: " << e.what();
    }

    return 0;
    
}

double Sale::calculateTotalRevenue() {
    
    int totalRevenue = 0;
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT SUM(TotalPrice) AS TotalRevenue FROM Sales"));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        
        if(res->next()) {
            return res->getInt("TotalRevenue");
        }
    } catch (const sql::SQLException &e){
        std::cerr << "SQLException in calculateTotalRevenue: " << e.what();
    }
    return totalRevenue;

}


std::vector<MonthlyRevenue> Sale::getMonthlyRevenue() {
    std::vector<MonthlyRevenue> monthlyRevenueData;
    
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT YEAR(SaleDate) AS year, MONTH(SaleDate) AS month, SUM(TotalPrice) AS totalRevenue "
            "FROM Sales GROUP BY year, month ORDER BY year, month;"
            )
        );
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while(res->next()) {
            MonthlyRevenue revenue;
            revenue.year = res->getInt("year");
            revenue.month = res->getInt("month");
            revenue.totalRevenue = res->getDouble("totalRevenue");
            monthlyRevenueData.push_back(revenue);
        }
    } catch (const sql::SQLException &e){
        std::cerr << "SQLException in getMonthlyRevenue: " << e.what(); 
    }
    return monthlyRevenueData;
}



crow::json::wvalue Sale::convertMonthlyRevenueToJson(const std::vector<MonthlyRevenue>& monthlyRevenues) {
    crow::json::wvalue jsonMonthlyRevenues(crow::json::type::List);
    
    for (unsigned int i = 0; i < static_cast<unsigned int>(monthlyRevenues.size()); ++i) {
        jsonMonthlyRevenues[i]["year"] = monthlyRevenues[i].year;
        jsonMonthlyRevenues[i]["month"] = monthlyRevenues[i].month;
        jsonMonthlyRevenues[i]["totalRevenue"] = monthlyRevenues[i].totalRevenue;
    }

    return jsonMonthlyRevenues;
}

int Sale::calculateTotalProductsSold() {
    
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT SUM(Quantity) as totalProductsSold FROM Sales")
        );
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        
        if(res->next()) {
            return res->getInt("totalProductsSold");
        }

    } catch (const sql::SQLException &e) {
        std::cerr << "SQLException in calculateTotalProductsSold: " << e.what();
    }
    return 0;
}

std::vector<ProductSales> Sale::getTopSellingProductsLastSixMonths() {
    std::vector<ProductSales> topSellingProducts;
    
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
            "SELECT p.Name, SUM(s.Quantity) as Quantity "
            "FROM Sales s "
            "JOIN Products p ON s.ProductID = p.ProductID "
            "WHERE s.SaleDate BETWEEN DATE_SUB(NOW(), INTERVAL 6 MONTH) AND NOW() "
            "GROUP BY p.Name "
            "ORDER BY Quantity DESC "
            "LIMIT 6;" // Limits the results to the top 6
            )
        );
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        
        while (res->next()) {
            topSellingProducts.push_back({
                res->getString("Name"), 
                res->getUInt("Quantity") 
            });
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQLException in getTopSellingProductsLastSixMonths: " << e.what();
    }

    return topSellingProducts;
}

crow::json::wvalue Sale::convertTopSellingProductsToJson(const std::vector<ProductSales>& topSellingProducts) {
    crow::json::wvalue jsonTopSellingProducts(crow::json::type::List);
    
    for (unsigned int i = 0; i < static_cast<unsigned int>(topSellingProducts.size()); ++i) {
        jsonTopSellingProducts[i]["productName"] = topSellingProducts[i].name;
        jsonTopSellingProducts[i]["quantitySold"] = topSellingProducts[i].quantity;
    }

    return jsonTopSellingProducts;
}

std::pair<std::string, int> Sale::getMostPopularCategoryAndUnitsSold() {
    std::pair<std::string, int> result;
    try {
        auto conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT c.Name, SUM(s.Quantity) as UnitsSold "
                "FROM Sales s "
                "JOIN Products p ON s.ProductID = p.ProductID "
                "JOIN Categories c ON p.CategoriesID = c.CategoriesID "
                "GROUP BY c.Name "
                "ORDER BY UnitsSold DESC "
                "LIMIT 1;"
            )
        );

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            result.first = res->getString("Name");
            result.second = res->getInt("UnitsSold");
        }
    } catch (sql::SQLException &e) {
        std::cerr << "SQLException in getMostPopularCategoryAndUnitsSold: " << e.what();
    }
    return result;
}

std::map<std::string, double> Sale::getRevenueByCategory() {
    std::map<std::string, double> revenueByCategory;
    try {
        auto conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(R"(
                SELECT c.Name AS CategoryName, COALESCE(SUM(s.TotalPrice), 0) AS TotalRevenue
                FROM Categories c
                LEFT JOIN Products p ON c.CategoriesID = p.CategoriesID
                LEFT JOIN Sales s ON p.ProductID = s.ProductID
                GROUP BY c.Name;
            )")
        );

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            std::string categoryName = res->getString("CategoryName");
            double totalRevenue = res->getDouble("TotalRevenue");
            revenueByCategory[categoryName] = totalRevenue;
        }
    } catch (sql::SQLException &e) {
        std::cerr << "SQLException in getRevenueByCategory: " << e.what();
    }
    return revenueByCategory;
}


std::map<std::string, int> Sale::getUnitsSoldByCategory() {
    std::map<std::string, int> unitsSoldByCategory;
    try {

        auto conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(R"(
                SELECT c.Name AS CategoryName, COALESCE(SUM(s.Quantity), 0) AS UnitsSold
                FROM Categories c
                LEFT JOIN Products p ON c.CategoriesID = p.CategoriesID
                LEFT JOIN Sales s ON p.ProductID = s.ProductID
                GROUP BY c.Name;
            )")
        );

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            std::string categoryName = res->getString("CategoryName");
            int unitsSold = res->getInt("UnitsSold");
            
            unitsSoldByCategory[categoryName] = unitsSold;
        }
    } catch (sql::SQLException &e) {
        std::cerr << "SQLException in getUnitsSoldByCategory: " << e.what();
    }
    return unitsSoldByCategory;
}