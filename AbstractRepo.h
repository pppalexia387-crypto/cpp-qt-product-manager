#pragma once
#include "Product.h"
#include <vector>
#include <string>

class AbstractRepo {
public:
	virtual void add(const Product& p) = 0;
	virtual void remove(int pos) = 0;
	virtual void modify(int pos, const Product& p) = 0;
	virtual int search(const std::string& name, const std::string& type) = 0;

	virtual std::vector<Product> getAll() = 0;

	virtual ~AbstractRepo() = default;
};