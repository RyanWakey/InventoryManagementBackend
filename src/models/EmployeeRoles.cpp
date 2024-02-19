#include "../../include/models/EmployeeRoles.h"
#include "../../include/database/DB_Manager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h> 


EmployeeRoles::EmployeeRoles() : employeeId(0), roleId(0) { }

EmployeeRoles::EmployeeRoles(int employeeId, int roleId) : employeeId(employeeId), roleId(roleId) { }


void EmployeeRoles::createRoleToEmployee(int employeeId, int roleId) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "INSERT INTO EmployeeRoles (EmployeeId, RoleId) VALUES (?, ?)"));

        pstmt->setInt(1, employeeId);
        pstmt->setInt(2, roleId);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in createRoleToEmployee: " << e.what() << std::endl;
    }
}


void EmployeeRoles::deleteRoleFromEmployee(int employeeId, int roleId) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "DELETE FROM EmployeeRoles WHERE EmployeeId = ? AND RoleId = ?"));

        pstmt->setInt(1, employeeId);
        pstmt->setInt(2, roleId);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in deleteRoleFromEmployee: " << e.what() << std::endl;
    }
}

std::vector<int> EmployeeRoles::readRolesForEmployee(int employeeId) {    
    std::vector<int> roles;

    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT RoleId FROM EmployeeRoles WHERE EmployeeId = ?"));

        pstmt->setInt(1, employeeId);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            roles.push_back(res->getInt("RoleId"));
        }  
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in readRolesForEmployee: " << e.what() << std::endl;
    }

    return roles;
}


crow::json::wvalue EmployeeRoles::convertRolesToJSON(const std::vector<int>& roleIds) {
    crow::json::wvalue jsonRoles(crow::json::type::List);

    int index = 0;
    for (int roleId : roleIds) {
        Roles role = Roles::readRole(roleId); 
        crow::json::wvalue jsonRole;
        jsonRole["RoleID"] = role.getId();
        jsonRole["RoleName"] = role.getName();

        jsonRoles[index++] = std::move(jsonRole);
    }
    return jsonRoles;
}
