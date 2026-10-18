#pragma once
#include<string>
#include"Product.h"
class ValidatorException {
	//class for validator exceptions
private:
	std::string msg;
public:

	ValidatorException(const std::string& message) :msg{ message } {

	}
	std::string GetMessage() const{
		return msg;
	}
};
class ProductValidator {
public:
	static std::string validate(const Product& p);
};