#ifndef AUTHENCTICATION_SERVICE_H
#define AUTHENCTICATION_SERVICE_H

#include <string>

class AuthenticationService {
public:
    static bool validateCredentials(const std::string& userType, const std::string& username, const std::string& password);
    static std::string hashPassword(const std::string& password);
    static bool verifyPassword(const std::string& password, const std::string& hash);
    static bool registerCustomer(const std::string& username, const std::string& password);
    static bool emailExists(const std::string& email);
    static std::string getSecretKey();
    static std::string generateToken(int userId, const std::string& userType);
    static bool validateToken(const std::string& token);
    static int fetchSpecificId(const std::string& userType, const std::string& email);
};

#endif