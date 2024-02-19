#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  

#include <memory>

class DB_Manager {

public:
    static void initialize();
    static std::shared_ptr<sql::Connection> getConnection();
    static void close();

private:
    static std::shared_ptr<sql::Connection> dbConnection;
    static std::string dbUser;
    static std::string dbPassword;
    static std::string dbHost;
};

#endif