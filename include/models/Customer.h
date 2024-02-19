#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

#include <boost/asio.hpp>
#include <crow.h>

class Customer {
    
private:
    int id;
    std::string name;
    std::string contactDetails;
    std::string address;
    std::string city;
    std::string country;
    std::string email;
    std::string password_hash;

public:
    Customer();
    Customer(int id, const std::string& name, const std::string& contactDetails, 
             const std::string& address, const std::string& city, const std::string& country, 
             const std::string& email, const std::string& password_hash);

    int getId() const noexcept { return id; }
    const std::string& getName() const noexcept { return name; }
    const std::string& getContactDetails() const noexcept { return contactDetails; }
    const std::string& getAddress() const noexcept { return address; }
    const std::string& getCity() const noexcept { return city; }
    const std::string& getCountry() const noexcept { return country; }
    const std::string& getEmail () const noexcept { return email; }
    const std::string& getPasswordHash() const noexcept { return password_hash; }

    void setName(const std::string& newName) noexcept { name = newName; }
    void setContactDetails(const std::string& newContactDetails) noexcept { contactDetails = newContactDetails; }
    void setAddress(const std::string& newAddress) noexcept { address = newAddress; }
    void setCity(const std::string& newCity) noexcept { city = newCity; }
    void setCountry(const std::string& newCountry) noexcept { country = newCountry; }
    void setEmail(const std::string& newEmail) noexcept { email = newEmail; }
    void setPasswordHash(const std::string& newPasswordHash) noexcept { password_hash = newPasswordHash; }

    static int createCustomer(const std::string& email, const std::string& hashedPassword);
    static void deleteCustomer(int id);                       
    static bool updateCustomer(int customerID, const std::string& name, const std::string& contactDetails, 
                              const std::string& address, const std::string& city, const std::string& country);
    static Customer readCustomer(int id);                       
    
    static crow::json::wvalue convertCustomerToJSON(const Customer& customer);
    static crow::json::wvalue convertProfileDataToJSON(const Customer& customer);
};

#endif // CUSTOMER_H
