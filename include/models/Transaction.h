#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <memory>
#include <vector>
#include "Product.h"

enum class TransactionType {
    Sale,
    Purchase,
    Return,
};

class Transaction {

private:

    int id;
    int quantity;
    TransactionType transactionType; 
    std::string date; 
    std::string comments;
    int productId;
    std::shared_ptr<Product> product; 

public:

    Transaction();
    Transaction(int id, int quantity, TransactionType transactionType, 
                const std::string& date, const std::string& comments, int productId);



    int getId() const noexcept { return id; }
    int getQuantity() const noexcept { return quantity; }
    
    TransactionType getTransactionType() const noexcept {
        return transactionType;
    }

    const std::string& getDate() const noexcept { return date; }
    const std::string& getComments() const noexcept { return comments; }
    int getProductId() const noexcept { return productId; }


    void setQuantity(int newQuantity) noexcept { quantity = newQuantity; }
    void setTransactionType(TransactionType newTransactionType) noexcept { transactionType = newTransactionType; }
    void setDate(const std::string& newDate) noexcept { date = newDate; }
    void setComments(const std::string& newComments) noexcept { comments = newComments; }
    void setProductId(int productId_) noexcept { productId = productId_; }
    void setProduct(const std::shared_ptr<Product>& product_) noexcept { product = product_; }

    static void createTransaction(int quantity, TransactionType transactionType, const std::string& date, const std::string& comments, int productId);
    static Transaction readTransaction(int id);
    static std::vector<Transaction> getAllTransactions();
    static void updateTransaction(int id, int newQuantity, TransactionType newTransactionType, const std::string& newDate, const std::string& newComments, int newProductId);
    static void deleteTransaction(int id);

    static std::string Transaction::transactionTypeToString(TransactionType type);
    static TransactionType Transaction::stringToTransactionType(const std::string& typeStr);

    static int getTransactionCount();
    static std::map<TransactionType, int> Transaction::getTransactionCountsByType();
};

#endif // TRANSACTION_H
