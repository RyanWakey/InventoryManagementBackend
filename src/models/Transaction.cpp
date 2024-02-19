#include "../../include/models/Transaction.h"
#include "../../include/database/DB_Manager.h"

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>  
#include <jdbc/cppconn/prepared_statement.h>  


Transaction::Transaction()
    : id(0), quantity(0), transactionType(TransactionType::Sale), date(""), comments(""), productId(0) { }


Transaction::Transaction(int id, int quantity, TransactionType transactionType, 
                         const std::string& date, const std::string& comments, int productId)
            : id(id), quantity(quantity), transactionType(transactionType), 
              date(date), comments(comments), productId(productId) { }


void Transaction::createTransaction(int quantity, TransactionType transactionType, const std::string& date, const std::string& comments, int productId) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "INSERT INTO Transactions (Quantity, TransactionType, Date, Comments, ProductID) VALUES (?, ?, ?, ?, ?)"));


        pstmt->setInt(1, quantity);
        pstmt->setString(2, transactionTypeToString(transactionType));
        pstmt->setString(3, date);
        pstmt->setString(4, comments);
        pstmt->setInt(5, productId);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in createTransaction: " << e.what() << std::endl;
    }
}


Transaction Transaction::readTransaction(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM Transactions WHERE TransactionID = ?"));

        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            Transaction transaction(
                res->getInt("TransactionID"),
                res->getInt("Quantity"),
                stringToTransactionType(res->getString("TransactionType")),
                res->getString("Date"),
                res->getString("Comments"),
                res->getInt("ProductID")
            );
            return transaction;
        } else {
            throw std::runtime_error("Transaction not found");
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in readTransaction: " << e.what() << std::endl;
        throw;
    }
}

std::vector<Transaction> Transaction::getAllTransactions() {
    std::vector<Transaction> transactions;
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM Transactions"));

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            Transaction transaction(
                res->getInt("TransactionID"),
                res->getInt("Quantity"),
                stringToTransactionType(res->getString("TransactionType")),
                res->getString("Date"),
                res->getString("Comments"),
                res->getInt("ProductID")
            );
            transactions.push_back(transaction);
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in getAllTransactions: " << e.what() << std::endl;
    }
    return transactions;
}


void Transaction::updateTransaction(int id, int newQuantity, TransactionType newTransactionType, const std::string& newDate, const std::string& newComments, int newProductId) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE Transactions SET Quantity = ?, TransactionType = ?, Date = ?, Comments = ?, ProductID = ? WHERE TransactionID = ?"));

        pstmt->setInt(1, newQuantity);
        pstmt->setString(2, transactionTypeToString(newTransactionType));
        pstmt->setString(3, newDate);
        pstmt->setString(4, newComments);
        pstmt->setInt(5, newProductId);
        pstmt->setInt(6, id);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in updateTransaction: " << e.what() << std::endl;
    }
}


void Transaction::deleteTransaction(int id) {
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM Transactions WHERE TransactionID = ?"));

        pstmt->setInt(1, id);

        pstmt->executeUpdate();
    } catch (const sql::SQLException &e) {
        std::cerr << "SQL Error in deleteTransaction: " << e.what() << std::endl;
    }
}


std::string Transaction::transactionTypeToString(TransactionType type) {
    switch (type) {
        case TransactionType::Sale: return "Sale";
        case TransactionType::Purchase: return "Purchase";
        case TransactionType::Return: return "Return";
        default: return "Unknown";
    }
}


TransactionType Transaction::stringToTransactionType(const std::string& typeStr) {
    if (typeStr == "Sale") return TransactionType::Sale;
    if (typeStr == "Purchase") return TransactionType::Purchase;
    if (typeStr == "Return") return TransactionType::Return;
    
    throw std::invalid_argument("Unknown TransactionType string: " + typeStr);
}

int Transaction::getTransactionCount() {
    int salesTransactionCount = 0;
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT COUNT(*) AS salesCount FROM Transactions"));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
    
        if (res->next()) {
            salesTransactionCount = res->getInt("salesCount");
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQLException in getSalesTransactionCount: " << e.what();
    }
    return salesTransactionCount;
}


std::map<TransactionType, int> Transaction::getTransactionCountsByType() {
    std::map<TransactionType, int> transactionCountsByType;
    try {
        std::shared_ptr<sql::Connection> conn = DB_Manager::getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT TransactionType, COUNT(*) AS typeCount "
                "FROM Transactions "
                "GROUP BY TransactionType"
            )
        );
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            std::string typeStr = res->getString("TransactionType");
            TransactionType type = stringToTransactionType(typeStr); // Use your conversion function here
            int typeCount = res->getInt("typeCount");
            transactionCountsByType[type] = typeCount;
        }
    } catch (const sql::SQLException &e) {
        std::cerr << "SQLException in getTransactionCountsByType: " << e.what();
    }
    return transactionCountsByType;
}
