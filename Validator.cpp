#include "Validator.h"
#include <string>

std::string ProductValidator::validate(const Product& p) {
    std::string errors = "";
    if (p.getPrice() <= 0.0f) {
        errors += "Invalid price!\n";
    }
    if (p.getName().empty()) { 
        errors += "Invalid name!\n";
    }
    if (p.getType().empty()) {
        errors += "Invalid type!\n";
    }
    if (p.getMan().empty()) {
        errors += "Invalid manufacturer!\n";
    }

    return errors;
}