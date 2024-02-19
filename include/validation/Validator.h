#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include <ctime>

class Validator {

    public: 
        static const size_t MAX_NAME_LENGTH = 255;
        static const size_t MAX_DESCRIPTION_LENGTH = 255;
        static const size_t MAX_STOCK_QUANTITY = 100000;
        static const size_t MAX_IMAGEURL_LENGTH = 255;
        static const int MIN_YEAR = 1900; 
        static const int MAX_YEAR = 2100; 

        static bool validateName(const std::string& name);
        static bool validateDescription(const std::string& description);
        static bool validateDouble(double price);
        static bool validateStockQuantity(int quantity);
        static bool validateImageURL(const std::string& imageURL);
        static bool validateDate(const std::string& date);
        static bool validateAddress(const std::string& address);
        static bool validateCity(const std::string& city);
        static bool validatePhoneNumber(const std::string& phoneNumber);
        static bool validateGender(const std::string& gender);
        static bool validatePostalCode(const std::string& postalCode);
        static bool validateProfilePicture(const std::string& profilePicture);
        static bool validateEmail(const std::string& email);

        static bool isValidDate(const std::tm& tm);
};

#endif