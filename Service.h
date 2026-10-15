#pragma once
#include"Repo.h"
#include<string>
#include<utility>
#include<vector>
#include<algorithm>
#include<iterator>
#include<map>
#include "Undo.h"
#include "AbstractRepo.h"
#include"Observer.h"
using std::string;
using std::vector;

//generic sort
//template <typename TElem, typename cmpFnc>
//void genericSort(std::vector<TElem>& ls, cmpFnc cmp) {
//	
//	for (size_t i = 0; i < ls.size() - 1; i++)
//		for (size_t j = i + 1; j < ls.size(); j++)
//			if (cmp(ls.at(i), ls.at(j)))
//				std::swap(ls.at(i), ls.at(j));
//}

template <typename TElem, typename cmpFnc>
void genericSort(std::vector<TElem>& ls, cmpFnc cmp) {

	std::sort(ls.begin(), ls.end(), cmp);

}

//generic filtering
//template <typename TElem, typename FilterFunction>
//std::vector<TElem> genericFilter(const std::vector<TElem>& ls, FilterFunction filter) {
//	std::vector<TElem> res;
//	for (const auto& p : ls) {
//		if (filter(p) == 1) {
//			res.push_back(p);
//		}
//	}
//	return res;
//}

template <typename TElem, typename FilterFunction>
std::vector<TElem> genericFilter(const std::vector<TElem>& ls, FilterFunction filter) {
	std::vector<TElem> res;
	std::copy_if(ls.begin(), ls.end(), std::back_inserter(res), filter);
	return res;
}

class ProductService : public Observable {
private:
	AbstractRepo& repo;
	vector<Product> basket;
	float cost = 0;
	std::vector<std::unique_ptr<ActiuneUndo>> undoActions;
	
public:
	ProductService(AbstractRepo& r) : repo{ r } {}
	ProductService(const ProductRepo& ot) = delete;
	ProductService() = default;
	ProductService(ProductRepo& rep) noexcept : repo{ rep } {}

	void addService(float price, const string& name, const string& type, const string& man);

	std::vector<Product> getAllProducts();

	//removes a product by name and by type
	void removeService(const string& name, const string& type);

	Product searchService(const string& name, const string& type);

	void modifyService(const string& name, const string& type, const string& newName, const string& newType, const string& newMan, float newPrice);

	//helper functions filter
	static int priceFiltering(const Product& p, float price) noexcept;
	static int nameFiltering(const Product& p, const string& name) noexcept;
	static int manFiltering(const Product& p, const string& man) noexcept;

	vector<Product> genericFiltering(int crit, const string& txt, float price);
	//generic filter


	//helper functions sort
	static int typeCmpDec(const Product& p1, const Product& p2) noexcept;
	static int typeCmpInc(const Product& p1, const Product& p2) noexcept;
	static int nameCmpDec(const Product& p1, const Product& p2) noexcept;
	static int nameCmpInc(const Product& p1, const Product& p2) noexcept;
	static int priceCmpInc(const Product& p1, const Product& p2) noexcept;
	static int priceCmpDec(const Product& p1, const Product& p2) noexcept;

	//sort func
	vector<Product> genericSorting(int order, int crit);
	//basket
	void addBasket(const string& name);
	const float getCost() const noexcept;
	void removeBasket() noexcept;
	const vector<Product> getAllBasket() const noexcept;
	void generateBasket(int n);
	void exportBasketCSV(const string& name);
	void exportBasketHTML(const string& name);
	std::map<const string, int> raportType();
	void undo();
};