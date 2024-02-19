#include "../../include/models/Supplier.h"
#include "../../include/database/DB_Manager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  

#include <vector>


Supplier::Supplier() : id(0), name(""), contactPerson(""), contactNumber(""), email(""), address(""), notes("") { }

Supplier::Supplier(int id, const std::string& name, const std::string& contactPerson, const std::string& contactNumber,
                  const std::string& email, const std::string& address, const std::string& notes) 
                    : id(id), name(name), contactPerson(contactPerson), contactNumber(contactNumber), email(email), 
                      address(address), notes(notes) { }
    
void Supplier::createSupplier(const std::string& name, const std::string& contactPerson, const std::string& contactNumber, 
                              const std::string& email, const std::string& address, const std::string& notes) {
    try {
      // Get DB connection from DB Manager
      std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
      // Prepare SQL statement
      std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO Suppliers (Name, ContactPerson,ContactNumber, Email, Address, Notes) VALUES (?, ?, ?, ?, ?, ?)"));

      // Bind the data to the statement
      pstmt->setString(1, name);
      pstmt->setString(2, contactPerson);
      pstmt->setString(3, contactNumber);
      pstmt->setString(4, email);
      pstmt->setString(5, address);
      pstmt->setString(6, notes);
      
      // Execute the statement
      pstmt->executeUpdate();

    } catch (const sql::SQLException &e) {
      std::cerr << "SQL Error in createCategory: " << e.what() << std::endl;
    }                         
}

void Supplier::deleteSupplier(int id){
  try{
    
    // Get DB connection from DB Manager
    std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

    // Prepare SQL statement
    std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM Suppliers WHERE SupplierID = ?"));

    // Bind the id to the statement
    pstmt->setInt(1,id);

    // Execute the statement
    pstmt->executeUpdate();

  } catch (const sql::SQLException &e){
    std::cerr << "SQL Error in deleteSupplier: " << e.what() << std::endl;
  }
}

void Supplier::updateSupplier(int id, const std::string& name, const std::string& contactPerson, 
                              const std::string& contactNumber, const std::string& email, 
                              const std::string& address, const std::string& notes) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        // Prepare the SQL statement for updating the supplier
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE Suppliers SET Name = ?, ContactPerson = ?, ContactNumber = ?, "
            "Email = ?, Address = ?, Notes = ? WHERE SupplierID = ?"
        ));

        // Bind data
        pstmt->setString(1, name);
        pstmt->setString(2, contactPerson);
        pstmt->setString(3, contactNumber);
        pstmt->setString(4, email);
        pstmt->setString(5, address);
        pstmt->setString(6, notes);
        pstmt->setInt(7, id);  // Binding the ID for the WHERE clause

        // Execute the statement
        pstmt->executeUpdate();

    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in updateSupplier: " << e.what() << std::endl;
    }
}


Supplier Supplier::readSupplier(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT SupplierID, Name, ContactPerson, ContactNumber, Email, Address, Notes FROM Suppliers WHERE SupplierID = ?"));

        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            Supplier supplier(
                res->getInt("SupplierID"),
                res->getString("Name"),
                res->getString("ContactPerson"),
                res->getString("ContactNumber"),
                res->getString("Email"),
                res->getString("Address"),
                res->getString("Notes")
            );
            return supplier;
        } else {
            throw std::runtime_error("Supplier not found");
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in readSupplier: " << e.what() << std::endl;
        throw; 
    }
}

std::vector<Supplier> Supplier::getAllSuppliers() {
    std::vector<Supplier> suppliers;

    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        // Prepare the SQL statement for fetching all suppliers
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT SupplierID, Name, ContactPerson, ContactNumber, Email, Address, Notes FROM Suppliers"));

        // Execute the query
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        // Iterate over the result set
        while (res->next()) {
            // Create a Supplier object for each row and add it to the vector
            Supplier supplier(
                res->getInt("SupplierID"),
                res->getString("Name"),
                res->getString("ContactPerson"),
                res->getString("ContactNumber"),
                res->getString("Email"),
                res->getString("Address"),
                res->getString("Notes")
            );
            suppliers.push_back(supplier);
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in getAllSuppliers: " << e.what() << std::endl;
    }

    return suppliers;
}

int Supplier::getTotalSuppliersActive() {
    
    try {

        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        // Prepare the SQL statement for fetching all suppliers
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT COUNT(*) AS activeSuppliers FROM Suppliers"));

        // Execute the query
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        
        if (res->next()){
            return res->getInt("activeSuppliers");
        }

    }catch (sql::SQLException &e){
        std::cerr << "SQLException in getTotalSalesMade: " << e.what();
    }

    return 0;
}

crow::json::wvalue Supplier::convertSuppliersToJson(const std::vector<Supplier>& suppliers) {
    crow::json::wvalue jsonSuppliers(crow::json::type::List); // Initialize as a JSON array

    int index = 0;
    for (const auto& supplier : suppliers) {
        crow::json::wvalue jsonSupplier;
        jsonSupplier["SupplierID"] = supplier.getId();
        jsonSupplier["Name"] = supplier.getName();
        jsonSupplier["ContactPerson"] = supplier.getContactPerson();
        jsonSupplier["ContactNumber"] = supplier.getContactNumber();
        jsonSupplier["Email"] = supplier.getEmail();
        jsonSupplier["Address"] = supplier.getAddress();
        jsonSupplier["Notes"] = supplier.getNotes();

        // Add the supplier to the array
        jsonSuppliers[index++] = std::move(jsonSupplier);
    }

    return jsonSuppliers;
    
}