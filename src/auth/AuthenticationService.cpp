#include "../../include/auth/AuthenticationService.h"
#include "../../include/models/Customer.h"
#include "../../include/database/DB_Manager.h"

#include <../external/libbcrypt-master/include/bcrypt/bcrypt.h>
#include <../external/libbcrypt-master/include/bcrypt/BCrypt.hpp>

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  

#include <jwt-cpp/jwt.h>

#include <fstream>
#include <string>
#include <iostream>

bool AuthenticationService::validateCredentials(const std::string& userType, const std::string& username, const std::string& password) {

    std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

    // Determine the table based on user type
    std::string table = userType == "employee" ? "Employees" : "Customers";

    std::string query = "SELECT PasswordHash FROM " + table + " WHERE Email = ?";
    auto statement = conn->prepareStatement(query);
    statement->setString(1, username); 

    auto resultSet = statement->executeQuery();

    // Fetch the results
    if (resultSet->next()) {
        std::string storedHash = resultSet->getString("PasswordHash");

        // Verify the password against the hash
        return verifyPassword(password, storedHash);
    } else {
        // User not found
        return false;
    }
}

std::string AuthenticationService::hashPassword(const std::string& password) {
    return BCrypt::generateHash(password);
}

bool AuthenticationService::verifyPassword(const std::string& password, const std::string& hash) {
    return BCrypt::validatePassword(password, hash);
}

bool AuthenticationService::registerCustomer(const std::string& email, const std::string& password) {
    // Hash the password
    std::string hashedPassword = hashPassword(password);
    
    try {
        // Get a connection to the database
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        // Prepare SQL statement to insert the new user
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO Customers (Email, PasswordHash) VALUES (?, ?)"));

        // Bind parameters
        pstmt->setString(1, email);
        pstmt->setString(2, hashedPassword);

        // Execute the statement
        int updateCount = pstmt->executeUpdate();

        return updateCount > 0;
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in registerUser: " << e.what() << std::endl;
        return false;
    }
}

bool AuthenticationService::emailExists(const std::string& email) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT COUNT(*) AS count FROM Customers WHERE Email = ?"
        ));

        pstmt->setString(1, email);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        
        if (res->next()) {
            return res->getInt("count") > 0;
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in emailExists: " << e.what() << std::endl;
    }
    return false;
}


std::string AuthenticationService::getSecretKey() {
    std::string filePath = "C:/Users/Ryan/Documents/Projects/Inventory Based Managment Code/secretconfig.txt";
    std::ifstream configFile(filePath);
    std::string key;

    if (!configFile) {
        std::cerr << "Failed to open the file at: " << filePath << std::endl;
        return "";
    }

    if (std::getline(configFile, key)) {
        std::cerr << "Secret key loaded: " << key << std::endl;  // Log the key for debugging, you may want to remove this in production
        return key;
    } else {
        std::cerr << "Failed to read the secret key from the file." << std::endl;
        return ""; 
    }
}

std::string AuthenticationService::generateToken(int userId, const std::string& userType) {
    try {
        std::string secret = getSecretKey();
        auto token = jwt::create()
            .set_issuer("InventoryManagementSystem")
            .set_type("JWT")
            .set_payload_claim("userType", jwt::claim(userType))
            .set_payload_claim("userId", jwt::claim(std::to_string(userId)))
            .sign(jwt::algorithm::hs256{secret});
        
        return token;
    } catch (const std::exception& e) {
        std::cerr << "Token generation failed: " << e.what() << std::endl;
        return ""; 
    }
}

bool AuthenticationService::validateToken(const std::string& token) {
    try {
        std::string secret = getSecretKey();
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::hs256{secret})
                                    .with_issuer("InventoryManagementSystem");
        verifier.verify(decoded);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Token validation failed: " << e.what() << std::endl;
        return false;
    }
}

int AuthenticationService::fetchSpecificId(const std::string& userType, const std::string& email) {
    std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
    std::unique_ptr<sql::PreparedStatement> pstmt;
    std::unique_ptr<sql::ResultSet> res;

    if (userType == "employee") {
        pstmt.reset(conn->prepareStatement("SELECT EmployeeID FROM Employees WHERE Email = ?"));
    } else if (userType == "customer") {
        pstmt.reset(conn->prepareStatement("SELECT CustomerID FROM Customers WHERE Email = ?"));
    } else {
        throw std::runtime_error("Invalid user type");
    }

    pstmt->setString(1, email);
    res.reset(pstmt->executeQuery());

    if (res->next()) {
        // Assumes the ID column is the first column in both queries
        return res->getInt(1); 
    } else {
        throw std::runtime_error("User not found");
    }
}