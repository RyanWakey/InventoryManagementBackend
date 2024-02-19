#include "../../include/validation/Validator.h"

#include <string>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath> 

bool Validator::validateName(const std::string& name){
    return name.length() > 2 && name.length() <= Validator::MAX_NAME_LENGTH;
}
    
    
bool Validator::validateDescription(const std::string& description){
    return !description.empty() && description.length() <= Validator::MAX_DESCRIPTION_LENGTH;
}
 

bool Validator::validateDouble(double value){
    // Check for range
    if (value > 99999999.99 || value < -99999999.99) {
        return false;
    }

    // Round the value to two decimal places
    double roundedValue = std::round(value * 100.0) / 100.0;

    // Check if the rounded value is equal to the original value
    if (roundedValue != value) {
        return false;
    }

    return true;
}

        
bool Validator::validateStockQuantity(int quantity){
    return quantity >= 0 && quantity <= Validator::MAX_STOCK_QUANTITY;
}
        

bool Validator::validateImageURL(const std::string& imageURL) {
    if (imageURL.empty()) {
        return true; // Allow empty imageURL
    }
    // Regex for a more general URL pattern
    std::regex urlRegex("(http|https)://(\\w+\\.)+\\w+(/\\S*)?(\\.(jpg|jpeg|png|gif))?");

    // Check length and format
    return imageURL.length() <= Validator::MAX_IMAGEURL_LENGTH && std::regex_match(imageURL, urlRegex);
}




bool Validator::validateDate(const std::string& date){
    // Check format (YYYY-MM-DD)
    std::regex dateRegex(R"(^\d{4}-\d{2}-\d{2}$)");
    
    if (!std::regex_match(date, dateRegex)) {
        return false;
    }

    // Parse the date
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");

    // Check if parsing was successful and date components are valid
    if (ss.fail() || !isValidDate(tm)) {
        return false;
    }

    // Check year range
    if (tm.tm_year + 1900 < MIN_YEAR || tm.tm_year + 1900 > MAX_YEAR) {
        return false;
    }

    return true;
}


bool Validator::isValidDate(const std::tm& tm){
    // Construct a time_t from the tm struct and convert it back to tm struct
        std::time_t t = std::mktime(const_cast<std::tm*>(&tm));
        std::tm tmCopy;
        localtime_s(&tmCopy, &t);  // For C++11 and later, use localtime_s on Windows

        // Check if the year, month, and day match
        return tm.tm_year == tmCopy.tm_year && 
               tm.tm_mon == tmCopy.tm_mon && 
               tm.tm_mday == tmCopy.tm_mday;
}


bool Validator::validateAddress(const std::string& address) {
    return !address.empty() && address.length() <= 255;
}

bool Validator::validateEmail(const std::string& email){
    std::regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(email, emailPattern);
}

bool Validator::validateCity(const std::string& city) {
    return !city.empty() && city.length() <= 255;
}


bool Validator::validatePhoneNumber(const std::string& phoneNumber) {
    std::regex phonePattern(R"(^\+?[\d\s-]{7,15}$)");
    return std::regex_match(phoneNumber, phonePattern);
}


bool Validator::validateGender(const std::string& gender) {
    if (gender.length() != 1) return false;

    char genderChar = gender[0];
    return genderChar == 'M' || genderChar == 'F';  
}


bool Validator::validateProfilePicture(const std::string& url) {
    // Regex to check if the URL format is valid
    std::regex urlPattern("^(http|https)://(\\w+\\.)+\\w+(/\\w+)*(\\.(jpg|jpeg|png|gif|bmp))?$");
    return std::regex_match(url, urlPattern);
}


bool Validator::validatePostalCode(const std::string& postalCode) {
    std::regex postalCodePattern("^[0-9]{5}(-[0-9]{4})?$"); 
    return std::regex_match(postalCode, postalCodePattern);
}
