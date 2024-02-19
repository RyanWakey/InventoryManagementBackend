#include "../../include/models/Roles.h"
#include "../../include/database/DB_Manager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  



Roles::Roles() : id(0), name("") { }

Roles::Roles(int id, const std::string& name)
    : id(id), name(name) { }


void Roles::createRole(const std::string& roleName) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "INSERT INTO Roles (Name) VALUES (?)"));
        
        pstmt->setString(1, roleName);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in createRole: " << e.what() << std::endl;
    }
}


Roles Roles::readRole(int roleId) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT * FROM Roles WHERE RoleID = ?"));

        pstmt->setInt(1, roleId);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            Roles role(
                res->getInt("RoleID"),
                res->getString("Name")
            );
            return role;
        } else {
            throw std::runtime_error("Role not found");
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in readRole: " << e.what() << std::endl;
        throw;
    }
}

std::vector<Roles> Roles::getAllRoles() {
    std::vector<Roles> roles;
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM Roles"));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            Roles role(
                res->getInt("RoleID"),
                res->getString("Name")
            );
            roles.push_back(role);
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in getAllRoles: " << e.what() << std::endl;
    }
    return roles;
}


void Roles::updateRole(int roleId, const std::string& newRoleName) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE Roles SET Name = ? WHERE RoleID = ?"));

        pstmt->setString(1, newRoleName);
        pstmt->setInt(2, roleId);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in updateRole: " << e.what() << std::endl;
    }
}


void Roles::deleteRole(int roleId) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM Roles WHERE RoleID = ?"));

        pstmt->setInt(1, roleId);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in deleteRole: " << e.what() << std::endl;
    }
}

