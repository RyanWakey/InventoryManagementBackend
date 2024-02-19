#include "../../include/models/PermissionsRoles.h"
#include "../../include/database/DB_Manager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  


PermissionsRoles::PermissionsRoles() : roleId(0), permissionsId(0) { }

PermissionsRoles::PermissionsRoles(int roleId, int permissionId) : roleId(roleId), permissionsId(permissionId) { }

void PermissionsRoles::createPermissionToRole(int roleId, int permissionId) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "INSERT INTO PermissionsRoles (RoleID, PermissionID) VALUES (?, ?)"));

        pstmt->setInt(1, roleId);
        pstmt->setInt(2, permissionId);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in createPermissionToRole: " << e.what() << std::endl;
    }
}

void PermissionsRoles::deletePermissionFromRole(int roleId, int permissionId) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "DELETE FROM PermissionsRoles WHERE roleID = ? AND PermissionID = ?"));

        pstmt->setInt(1, roleId);
        pstmt->setInt(2, permissionId);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in deletePermissionFromRole: " << e.what() << std::endl;
    }
}

std::vector<int> PermissionsRoles::getPermissionsForRole(int roleId) {
    std::vector<int> permissions;

    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT PermissionID FROM PermissionsRoles WHERE RoleID = ?"));

        pstmt->setInt(1, roleId);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            permissions.push_back(res->getInt("PermissionID"));
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in getPermissionsForRole: " << e.what() << std::endl;
    }

    return permissions;
}

