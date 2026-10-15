#pragma once
#include <vector>
#include <string>
#include "Product.h"
#include "AbstractRepo.h" // Foarte important

using std::string;
using std::vector;

class RepoException {
	//class for repository exceptions
private:
	string msg;
public:
	RepoException(const string& message) : msg{ message } {}

	const string& GetMessage() const noexcept {
		return msg;
	}
};

class ProductRepo : public AbstractRepo {
private:
	vector<Product> products;
public:
	ProductRepo(const ProductRepo& ot) = delete;
	ProductRepo() = default;

	//adds a product to the list
	void add(const Product& p) override;

	vector<Product> getAll() override;

	//removes a product from a position
	void remove(int pos) override;

	//searches for a product by name and by type, returs its index
	int search(const string& name, const string& type) noexcept override;

	//inserts a product at an index
	void modify(int index, const Product& p) override;
};