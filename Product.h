#pragma once
#include<string>
#include<iostream>
using std::string;
class Product {

private:
	float price;
	string name;
	string type;
	string manufacturer;
public:
	//constructors
	
	Product(float price, string name, string type, string man) :price{ price }, name{ name }, type{ type }, manufacturer{ man } {}Product() noexcept : price{ 0 } {}
	//copy constructor
	//Product(const Product& p) = default;
	
	bool operator==(const Product& other) const noexcept;
	//getters
	string getName()const noexcept;
	string getType()const noexcept;
	float getPrice()const noexcept;
	string getMan()const noexcept;

	Product(const Product& o) :price{ o.price }, name{ o.name }, type{ o.type }, manufacturer{ o.manufacturer }  {
	}
	


};