#include"Service.h"
#include"Validator.h"
#include<functional>
#include<random>
#include<fstream>
#include<map>
#include<algorithm>

using std::map;

void ProductService::addService(float price, const string& name, const string& type, const string& man) {
	Product p{ price,name,type,man };

	string errors;
	errors = ProductValidator::validate(p);
	if (!errors.empty()) {
		throw ValidatorException(errors);
	}
	repo.add(p);
	undoActions.push_back(std::make_unique<UndoAdauga>(repo, p));
}

std::vector<Product> ProductService::getAllProducts() {
	return repo.getAll();
}

void ProductService::removeService(const string& name, const string& type) {
	const int pos = repo.search(name, type);
	
	if (pos == -1)
		throw RepoException("The product does not exist!");
	Product pSters = repo.getAll().at(pos);
	repo.remove(pos);

	undoActions.push_back(std::make_unique<UndoSterge>(repo, pSters));
}

Product ProductService::searchService(const string& name, const string& type){
	const int pos = repo.search(name, type);
	if (pos == -1)
		throw RepoException("The product was not found!");
	else
		return repo.getAll().at(pos);
}

void ProductService::modifyService(const string& name, const string& type, const string& newName, const string& newType, const string& newMan, float newPrice) {
	const int pos = repo.search(name, type);
	if (pos == -1)
		throw RepoException("The product was not found!");
	else {
		Product p{ newPrice,newName,newType,newMan };
		string errors = ProductValidator::validate(p);
		if (!errors.empty()) {
			throw ValidatorException(errors);
		}
		Product pVechi = repo.getAll().at(pos);
		repo.modify(pos, p);
		undoActions.push_back(std::make_unique<UndoModifica>(repo, pVechi, p));

	}
}

int ProductService::priceFiltering(const Product& p, float price)  noexcept {
	return ((p.getPrice() - price) < 0.001);
}

int ProductService::nameFiltering(const Product& p, const string& name) noexcept {
	return (p.getName() == name);
}

int ProductService::manFiltering(const Product& p, const string& man)  noexcept {
	return (p.getMan() == man);
}

vector<Product> ProductService::genericFiltering(int crit, const string& txt, float price) {
	std::function<int(const Product&)> func = nullptr;

	if (crit == 1) {
		func = [price](const Product& p) noexcept {
			return priceFiltering(p, price);
			};
	}
	else if (crit == 2) {
		func = [txt](const Product& p) noexcept {
			return nameFiltering(p, txt);
			};
	}
	else if (crit == 3) {
		func = [txt](const Product& p) noexcept {
			return manFiltering(p, txt);
			};
	}

	const vector<Product>& ls = getAllProducts();
	return genericFilter(ls, func);
}
//helper functions sort
int ProductService::priceCmpInc(const Product& p1, const Product& p2) noexcept {
	return (p1.getPrice() < p2.getPrice());
}

int ProductService::priceCmpDec(const Product& p1, const Product& p2) noexcept {
	return (p1.getPrice() > p2.getPrice());
}

int ProductService::nameCmpInc(const Product& p1, const Product& p2) noexcept {
	return (p1.getName() < p2.getName());
}

int ProductService::nameCmpDec(const Product& p1, const Product& p2) noexcept {
	return (p1.getName() > p2.getName());
}

int ProductService::typeCmpInc(const Product& p1, const Product& p2)  noexcept {
	if (p1.getName() == p2.getName())
		return (p1.getType() < p2.getType());
	else
		return(p1.getName() < p2.getName());
}

int ProductService::typeCmpDec(const Product& p1, const Product& p2) noexcept {
	if (p1.getName() == p2.getName())
		return (p1.getType() > p2.getType());
	else
		return(p1.getName() > p2.getName());
}

vector<Product> ProductService::genericSorting(int order, int crit) {
	std::function <int(const Product&, const Product&)> func = nullptr;
	if (order == 0) {
		if (crit == 1)
			func = priceCmpInc;
		else if (crit == 2)
			func = typeCmpInc;
		else if (crit == 3)
			func = nameCmpInc;
	}
	else if (order == 1) {
		if (crit == 1)
			func = priceCmpDec;
		else if (crit == 2)
			func = typeCmpDec;
		else if (crit == 3)
			func = nameCmpDec;

	}

	
	vector<Product> ls = repo.getAll();
	genericSort(ls, func);
	return ls;
}

void ProductService::addBasket(const string& name) {
	vector<Product> prod = genericFiltering(2, name, 0);
	if (prod.size() == 0) {
		throw RepoException( "The product does not exist!\n");
		
	}
	for (auto& p : prod)
	{
		basket.push_back(p);
		cost += p.getPrice();
	}
	doStuff();
}
const float ProductService::getCost() const noexcept {
	return cost;
}

void ProductService::removeBasket() noexcept{
	basket.clear();
	cost = 0;
	doStuff();
}

const vector<Product> ProductService::getAllBasket() const noexcept {
	return basket;
}

void ProductService::generateBasket(int n) {
	vector<Product> prod = repo.getAll();
	if (prod.size() <= 0)
		throw RepoException("There are no products!\n");
	std::uniform_int_distribution<> dist(0, static_cast<int>(prod.size() - 1));
	std::mt19937 mt{ std::random_device{}() };
	for (int i = 0; i < n; i++) {
		const int poz = dist(mt);
		addBasket(prod.at(poz).getName());
		
	}
	doStuff();
}
void ProductService::exportBasketCSV(const string& name) {
	std::ofstream f(name);

	
	if (!f.is_open()) {
		throw RepoException("Could not open the file!\n");
	}
	f << "Name,Type,Price,Manufacturer\n";

	for (const auto& p : basket) {
		f << p.getName() << ","
			<< p.getType() << ","
			<< p.getPrice() << ","
			<< p.getMan() << "\n";
	}

	f.close();
	doStuff();
}

void ProductService::exportBasketHTML(const string& name) {
	std::ofstream fout(name);

	if (!fout.is_open()) {
		throw RepoException("Could not open the file!\n");
	}


	fout << "<html><body>\n";
	fout << "<h2>Shopping basket</h2>\n";
	fout << "<table border=\"1\">\n"; 

	fout << "<tr><th>Name</th><th>Type</th><th>Price</th><th>Manufacturer</th></tr>\n";

	for (const auto& p : basket) {
		fout << "<tr>\n";
		fout << "<td>" << p.getName() << "</td>\n";
		fout << "<td>" << p.getType() << "</td>\n";
		fout << "<td>" << p.getPrice() << "</td>\n";
		fout << "<td>" << p.getMan() << "</td>\n";
		fout << "</tr>\n";
	}

	fout << "</table>\n";
	fout << "</body></html>\n";

	fout.close();
	doStuff();
}

map<const string, int>  ProductService::raportType() {
	map<const string, int> m;
	for (const Product& p : repo.getAll()) {
		m[p.getType()]++;
	}
	return m;
}

void ProductService::undo() {
	if (undoActions.empty()) {
		throw RepoException("There aren't any operations left for undo!");
	}
	undoActions.back()->doUndo();
	undoActions.pop_back();
}