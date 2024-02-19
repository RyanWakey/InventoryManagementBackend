#include "../../include/models/Customer.h"
#include "../../include/database/DB_Manager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  


Customer::Customer() : id(0), name(""), contactDetails(""), address(""), city(""), country(""), email(""), password_hash("") { }

Customer::Customer(int id, const std::string& name, const std::string& contactDetails, 
                   const std::string& address, const std::string& city, const std::string& country,
                   const std::string& email, const std::string& password_hash) 
                : id(id), name(name), contactDetails(contactDetails), address(address), city(city), country(country),
                  email(email), password_hash(password_hash) { }



int Customer::createCustomer(const std::string& email, const std::string& hashedPassword) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        // Insert the new customer and retrieve the generated ID in one go
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "INSERT INTO Customers (Email, PasswordHash) VALUES (?, ?); SELECT LAST_INSERT_ID() AS NewID;"
        ));

        pstmt->setString(1, email);
        pstmt->setString(2, hashedPassword);

        bool isResultSet = pstmt->execute();
        if (isResultSet) {
            std::unique_ptr<sql::ResultSet> res(pstmt->getResultSet());
            // Move to the first row of the result set
            if (res->next()) {
                int newId = res->getInt("NewID");
                return newId; // Return the new customer ID
            }
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in createCustomer: " << e.what() << std::endl;
    }
    return -1; // Return -1 or another specific value to indicate failure
}


void Customer::deleteCustomer(int id) {
    try {
        // Get DB connection from DB Manager
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        // Prepare SQL statement
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM Customers WHERE CustomerID = ?"));

        // Bind the data to the statement  
        pstmt->setInt(1, id);

        // Execute the statement    
        pstmt->executeUpdate();

    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in deleteCustomer: " << e.what() << std::endl;
    }
}

bool Customer::updateCustomer(int customerID, const std::string& name, const std::string& contactDetails, 
                              const std::string& address, const std::string& city, const std::string& country) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE Customers SET Name = ?, ContactDetails = ?, Address = ?, City = ?, Country = ? WHERE CustomerID = ?"
        ));

        pstmt->setString(1, name);
        pstmt->setString(2, contactDetails);
        pstmt->setString(3, address);
        pstmt->setString(4, city);
        pstmt->setString(5, country);
        pstmt->setInt(6, customerID);

        int affectedRows = pstmt->executeUpdate();
        return affectedRows > 0;
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in updateCustomer: " << e.what() << std::endl;
        return false;
    }
}


Customer Customer::readCustomer(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT CustomerID, Name, ContactDetails, Address, City, Country, Email FROM Customers WHERE CustomerID = ?"));

        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            Customer customer(
                res->getInt("CustomerID"),
                res->getString("Name"),
                res->getString("ContactDetails"),
                res->getString("Address"),
                res->getString("City"),
                res->getString("Country"),
                res->getString("Email"),
                ""
            );
            return customer;
        } else {
            throw std::runtime_error("Customer not found");
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in readCustomer: " << e.what() << std::endl;
        throw;
    }
}


crow::json::wvalue Customer::convertCustomerToJSON(const Customer& customer) {
    crow::json::wvalue jsonCustomer;

    jsonCustomer["CustomerID"] = customer.getId();
    jsonCustomer["Name"] = customer.getName();
    jsonCustomer["ContactDetails"] = customer.getContactDetails();
    jsonCustomer["Address"] = customer.getAddress();
    jsonCustomer["City"] = customer.getCity();
    jsonCustomer["Country"] = customer.getCountry();
    jsonCustomer["Email"] = customer.getEmail();
    jsonCustomer["Customer"] = "";

    return jsonCustomer;
}

crow::json::wvalue Customer::convertProfileDataToJSON(const Customer& customer) {
    crow::json::wvalue jsonCustomer;

    jsonCustomer["CustomerID"] = customer.getId();
    jsonCustomer["Name"] = customer.getName();
    jsonCustomer["ContactDetails"] = customer.getContactDetails();
    jsonCustomer["Address"] = customer.getAddress();
    jsonCustomer["City"] = customer.getCity();
    jsonCustomer["Country"] = customer.getCountry();

    return jsonCustomer;
}


