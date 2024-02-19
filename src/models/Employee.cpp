#include "../../include/models/Employee.h"
#include "../../include/database/DB_Manager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  

#include <boost/asio.hpp>
#include <crow.h>

Employee::Employee() : id(0), firstName(""), lastName(""), dob(""), gender(' '), address(""), 
                       city(""), postalCode(""), phone(""), profilePicture(""), hireDate(""),
                       email(""), password_hash("") { }

Employee::Employee(int id, const std::string& firstname, const std::string& lastname, 
                   const std::string& DOB, char gender, const std::string& address, 
                   const std::string& city, const std::string& postalCode, 
                   const std::string& phone, const std::string& profilePicture, 
                   const std::string& hireDate, const std::string& email, const std::string& password_hash)
            : id(id), firstName(firstname), lastName(lastname), dob(DOB), gender(gender), 
              address(address), city(city), postalCode(postalCode), phone(phone), 
              profilePicture(profilePicture), hireDate(hireDate), email(email), password_hash(password_hash) { }

void Employee::createEmployee(const std::string& firstName, const std::string& lastName, const std::string& dob,
                              char gender, const std::string& address, const std::string& city, const std::string& postalCode,
                              const std::string& phone, const std::string& profilePicture, const std::string& hireDate,
                              const std::string& email, const std::string& hashedPassword) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "INSERT INTO Employees (FirstName, LastName, DOB, Gender, Address, City, PostalCode, Phone, ProfilePicture, hireDate, Email, PasswordHash)"
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));

        pstmt->setString(1, firstName);
        pstmt->setString(2, lastName);
        pstmt->setString(3, dob);
        pstmt->setString(4, std::string(1, gender)); 
        pstmt->setString(5, address);
        pstmt->setString(6, city);
        pstmt->setString(7, postalCode);
        pstmt->setString(8, phone);
        pstmt->setString(9, profilePicture);
        pstmt->setString(10, hireDate);
        pstmt->setString(11, email);
        pstmt->setString(12, hashedPassword);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in createEmployee: " << e.what() << std::endl;
    }
}

void Employee::deleteEmployee(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM Employees WHERE EmployeeID = ?"));

        pstmt->setInt(1, id);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in deleteEmployee: " << e.what() << std::endl;
    }
}

void Employee::updateEmployee(int id, const std::string& firstName, const std::string& lastName, const std::string& dob,
                              char gender, const std::string& address, const std::string& city, const std::string& postalCode,
                              const std::string& phone, const std::string& profilePicture, const std::string& hireDate,
                              const std::string& email) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE Employees SET FirstName = ?, LastName = ?, DOB = ?, Gender = ?, "
            "Address = ?, City = ?, PostalCode = ?, Phone = ?, ProfilePicture = ?, HireDate = ?, "
            "Email = ? WHERE EmployeeID = ?"));

        pstmt->setString(1, firstName);
        pstmt->setString(2, lastName);
        pstmt->setString(3, dob);
        pstmt->setString(4, std::string(1, gender)); // Convert char to string
        pstmt->setString(5, address);
        pstmt->setString(6, city);
        pstmt->setString(7, postalCode);
        pstmt->setString(8, phone);
        pstmt->setString(9, profilePicture);
        pstmt->setString(10, hireDate);
        pstmt->setString(11, email); 
        pstmt->setInt(12, id);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in updateEmployee: " << e.what() << std::endl;
    }
}


Employee Employee::readEmployee(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT EmployeeID, FirstName, LastName, DOB, Gender, Address, City, PostalCode, Phone, ProfilePicture, HireDate, Email FROM Employees WHERE EmployeeID = ?"));

        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            Employee employee(
                res->getInt("EmployeeID"),
                res->getString("FirstName"),
                res->getString("LastName"),
                res->getString("DOB"),
                res->getString("Gender")[0], 
                res->getString("Address"),
                res->getString("City"),
                res->getString("PostalCode"),
                res->getString("Phone"),
                res->getString("ProfilePicture"),
                res->getString("HireDate"),
                res->getString("Email"),
                ""
            );
            return employee;
        } else {
            throw std::runtime_error("Employee not found");
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in readEmployee: " << e.what() << std::endl;
        throw; // Re-throw the exception for the caller to handle
    }
}

std::vector<Employee> Employee::getEmployeesFromDatabase(){
    std::vector<Employee> employees;

    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection(); 

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM Employees"));

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            Employee employee(
                res->getInt("EmployeeID"),
                res->getString("Firstname"),
                res->getString("Lastname"),
                res->getString("DOB"),
                res->getString("Gender")[0], // Assuming gender is stored as a single character string
                res->getString("Address"),
                res->getString("City"),
                res->getString("PostalCode"),
                res->getString("Phone"),
                res->getString("ProfilePicture"),
                res->getString("HireDate"),
                res->getString("Email"),
                ""
            );
            employees.push_back(std::move(employee));
        }
    } catch (const sql::SQLException& e) {
        std::cerr << "SQL Error in getEmployeesFromDatabase: " << e.what() << std::endl;
    }

    return employees;
}

crow::json::wvalue Employee::convertEmployeesToJSON(const std::vector<Employee>& employees) {
    crow::json::wvalue jsonEmployees(crow::json::type::List); // Initialize as a JSON array

    int index = 0;
    for (const auto& employee : employees) {
        crow::json::wvalue jsonEmployee;
        jsonEmployee["EmployeeID"] = employee.getID();
        jsonEmployee["FirstName"] = employee.getFirstName();
        jsonEmployee["LastName"] = employee.getLastName();
        jsonEmployee["DOB"] = employee.getDOB();
        jsonEmployee["Gender"] = std::string(1, employee.getGender()); // Convert char to string
        jsonEmployee["Address"] = employee.getAddress();
        jsonEmployee["City"] = employee.getCity();
        jsonEmployee["PostalCode"] = employee.getPostalCode();
        jsonEmployee["Phone"] = employee.getPhone();
        jsonEmployee["ProfilePicture"] = employee.getProfilePicture();
        jsonEmployee["HireDate"] = employee.getHireDate();
        jsonEmployee["Email"] = employee.getEmail();

        // Add the employee to the array
        jsonEmployees[index++] = std::move(jsonEmployee);
    }

    return jsonEmployees;
}

crow::json::wvalue Employee::convertEmployeeToJSON(const Employee& employee){
    crow::json::wvalue jsonEmployee;

    jsonEmployee["EmployeeID"] = employee.getID();
    jsonEmployee["FirstName"] = employee.getFirstName();
    jsonEmployee["LastName"] = employee.getLastName();
    jsonEmployee["DOB"] = employee.getDOB();
    jsonEmployee["Gender"] = std::string(1, employee.getGender()); // Convert char to string
    jsonEmployee["Address"] = employee.getAddress();
    jsonEmployee["City"] = employee.getCity();
    jsonEmployee["PostalCode"] = employee.getPostalCode();
    jsonEmployee["Phone"] = employee.getPhone();
    std::string imageUrl = "/images/" + employee.getProfilePicture(); // Ensure there's a forward slash
    std::cout << imageUrl << " <---- IMAGE PATH" << std::endl;
    jsonEmployee["ProfilePicture"] = imageUrl;
    jsonEmployee["HireDate"] = employee.getHireDate();
    jsonEmployee["Email"] = employee.getEmail();

    return jsonEmployee;
}