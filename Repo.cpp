#include"Repo.h"
#include<algorithm>
#include<iterator>
void ProductRepo::add(const Product& p) {
	//adds a product to the list
	for (const Product& o : products) {
		if (p.getType() == o.getType() && p.getName() == o.getName()) {
			throw RepoException("The product already exists!");
		}
	}
	products.push_back(p);
}

vector<Product> ProductRepo::getAll(){
	//returns the product list
	return products;
}

void ProductRepo::remove(int pos) {
	//deletes the product from a position
	if (pos < 0 || pos >= products.size())
		throw RepoException("Invalid index!");
	products.erase(products.begin() + pos);
}

int ProductRepo::search(const string& name, const string& type) noexcept {
	//searches for a product by name and by type, returs its index
	auto poz = std::find_if(products.begin(), products.end(), [&](const Product& p) { return (p.getType() == type && p.getName() == name);});
	
	if (poz != products.end())
		return static_cast<int>(std::distance(products.begin(), poz));
	else
		return -1;}

void ProductRepo::modify(int index, const Product& p) {
	products.at(index) = p;
}