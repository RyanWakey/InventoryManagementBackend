#ifndef SUPPLIERS_H
#define SUPPLIERS_H

#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <crow.h>

class Supplier {

private:

    int id;
    std::string name;
    std::string contactPerson;
    std::string contactNumber;
    std::string email;
    std::string address;
    std::string notes;

public:

    Supplier();
    Supplier(int id, const std::string& name, const std::string& contactPerson, const std::string& contactNumber,
             const std::string& email, const std::string& address, const std::string& notes);

    int getId() const noexcept { return id; }
    const std::string& getName() const noexcept { return name; }
    const std::string& getContactPerson() const noexcept { return contactPerson; }
    const std::string& getContactNumber() const noexcept { return contactNumber; }
    const std::string& getEmail() const noexcept { return email; }
    const std::string& getAddress() const noexcept { return address; }
    const std::string& getNotes() const noexcept { return notes; }

    void setId(int newId) noexcept { id = newId; }
    void setName(const std::string& newName) noexcept { name = newName; }
    void setContactPerson(const std::string& newContactPerson) noexcept { contactPerson = newContactPerson; }
    void setContactNumber(const std::string& newContactNumber) noexcept { contactNumber = newContactNumber; }
    void setEmail(const std::string& newEmail) noexcept { email = newEmail; }
    void setAddress(const std::string& newAddress) noexcept { address = newAddress; }
    void setNotes(const std::string& newNotes) noexcept { notes = newNotes; }

    static void createSupplier(const std::string& name, const std::string& contactPerson, const std::string& contactNumber, const std::string& email, const std::string& address, const std::string& notes);
    static Supplier readSupplier(int id);
    static std::vector<Supplier> getAllSuppliers();
    static void updateSupplier(int id, const std::string& newName, const std::string& newContactPerson, const std::string& newContactNumber, const std::string& newEmail, const std::string& newAddress, const std::string& newNotes);
    static void deleteSupplier(int id);

    static int getTotalSuppliersActive();
    static crow::json::wvalue convertSuppliersToJson(const std::vector<Supplier>& suppliers);
};

#endif 
