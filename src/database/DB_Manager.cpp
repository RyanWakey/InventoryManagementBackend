#include "../../include/database/DB_Manager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <map>

std::shared_ptr<sql::Connection> DB_Manager::dbConnection = nullptr;
std::string DB_Manager::dbUser;
std::string DB_Manager::dbPassword;
std::string DB_Manager::dbHost;


void DB_Manager::initialize() {
    std::ifstream configFile("C:/Users/Ryan/Documents/Projects/Inventory Based Managment Code/dbconfig.txt");
    std::string line;
    std::map<std::string, std::string> configValues;
    
    if (!configFile) {
        std::cerr << "Unable to open config file" << std::endl;
    }

    while (std::getline(configFile, line)) {
        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '=')) {
            std::string value;
            if (std::getline(is_line, value)) {
                configValues[key] = value;
            }
        }
    }

    dbUser = configValues["DB_USER"];
    dbPassword = configValues["DB_PASSWORD"];
    dbHost = configValues["DB_HOST"];

    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
        dbConnection = std::shared_ptr<sql::Connection>(driver->connect(dbHost, dbUser, dbPassword));
        dbConnection->setSchema("InventoryManagement");
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }
}

std::shared_ptr<sql::Connection> DB_Manager::getConnection() {
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
        std::shared_ptr<sql::Connection> newConnection(driver->connect(dbHost, dbUser, dbPassword));
        newConnection->setSchema("InventoryManagement");
        return newConnection;
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in getConnection: " << e.what() << std::endl;
        return nullptr; // should handle this more appropriately.
    }
}


void DB_Manager::close() {
    if(dbConnection) {
        dbConnection->close();
    }
}