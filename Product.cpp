#include"Product.h"
#include<string>
using std::string;
bool Product::operator==(const Product& other) const noexcept {
    return (this->name == other.name &&
        this->type == other.type &&
        this->manufacturer == other.manufacturer &&
        this->price == other.price);
   
}
//getters
string Product::getType() const noexcept {
	return type;
}
string Product::getName() const noexcept {
	return name;
}
string Product::getMan() const noexcept {
	return manufacturer;
}
float Product::getPrice() const noexcept {
	return price;
}