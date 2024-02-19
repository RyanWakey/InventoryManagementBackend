// Employee.h
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <crow.h>

class Employee {
private:
    int id;
    std::string firstName;
    std::string lastName;
    std::string dob; // Date of Birth format "YYYY-MM-DD"
    char gender; // Gender is stored as 'M' or 'F'
    std::string address;
    std::string city;
    std::string postalCode;
    std::string phone;
    std::string profilePicture; // Path to the image or a URL
    std::string hireDate; 
    std::string email;
    std::string password_hash;

public:
    // Constructors
    Employee(); // Default constructor
    Employee(int id, const std::string& firstName, const std::string& lastName, const std::string& dob,
             char gender, const std::string& address, const std::string& city, const std::string& postalCode,
             const std::string& phone, const std::string& profilePicture, const std::string& hireDate,
             const std::string& email, const std::string& password_hash);

    // Accessors
    int getID() const noexcept { return id; }
    const std::string& getFirstName() const noexcept { return firstName; }
    const std::string& getLastName() const noexcept { return lastName; }
    const std::string& getDOB() const noexcept { return dob; }
    char getGender() const noexcept { return gender; }
    const std::string& getAddress() const noexcept { return address; }
    const std::string& getCity() const noexcept { return city; }
    const std::string& getPostalCode() const noexcept { return postalCode; }
    const std::string& getPhone() const noexcept { return phone; }
    const std::string& getProfilePicture() const noexcept { return profilePicture; }
    const std::string& getHireDate() const noexcept { return hireDate; }
    const std::string& getEmail () const noexcept { return email; }
    const std::string& getPasswordHash() const noexcept { return password_hash; }

    // Mutators
    void setFirstName(const std::string& newFirstName) noexcept { firstName = newFirstName; }
    void setLastName(const std::string& newLastName) noexcept { lastName = newLastName; }
    void setDOB(const std::string& newDOB) noexcept { dob = newDOB; }
    void setGender(char newGender) noexcept { gender = newGender; }
    void setAddress(const std::string& newAddress) noexcept { address = newAddress; }
    void setCity(const std::string& newCity) noexcept { city = newCity; }
    void setPostalCode(const std::string& newPostalCode) noexcept { postalCode = newPostalCode; }
    void setPhone(const std::string& newPhone) noexcept { phone = newPhone; }
    void setProfilePicture(const std::string& newProfilePicture) noexcept { profilePicture = newProfilePicture; }
    void setHireDate(const std::string& newHireDate) noexcept { hireDate = newHireDate; }
    void setEmail(const std::string& newEmail) noexcept { email = newEmail; }
    void setPasswordHash(const std::string& newPasswordHash) noexcept { password_hash = newPasswordHash; }

    static void createEmployee(const std::string& firstName, const std::string& lastName, const std::string& dob,
             char gender, const std::string& address, const std::string& city, const std::string& postalCode,
             const std::string& phone, const std::string& profilePicture, const std::string& hireDate, 
             const std::string& email, const std::string& hashedPassword);
    static void deleteEmployee(int id);         
    static void updateEmployee(int id, const std::string& firstName, const std::string& lastName, const std::string& dob,
                            char gender, const std::string& address, const std::string& city, const std::string& postalCode,
                            const std::string& phone, const std::string& profilePicture, const std::string& hireDate, 
                            const std::string& email);
    static Employee readEmployee(int id);

    static std::vector<Employee> getEmployeesFromDatabase();
    static crow::json::wvalue convertEmployeesToJSON(const std::vector<Employee>& employees);
    static crow::json::wvalue convertEmployeeToJSON(const Employee& employee);
};

#endif // EMPLOYEE_H
