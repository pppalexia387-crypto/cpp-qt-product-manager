#include <iostream>
#include "Tests.h"
#include "Product.h"
#include "Repo.h"
#include "Service.h"
#include "Validator.h"
#include <string>
#include <assert.h>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <map>
#include "RepoDict.h"

void testDomain() {
	Product p{ 123.12f, "a", "a", "a" };
	assert(p.getType() == "a");
	assert(p.getName() == "a");
	assert(p.getMan() == "a");
	assert((p.getPrice() - 123.12f) < 0.001);

	Product o = p;
	assert(o.getType() == p.getType());
	assert(o.getName() == p.getName());
	std::cout << "Domain tested successfully!\n";
}

void testRepo() {
	ProductRepo repo;
	Product p1{ 123.12f, "a", "a", "a" };
	repo.add(p1);
	assert(repo.getAll().size() == 1);
	try
	{
		repo.add(p1);
	}
	catch (const RepoException& exp)
	{
		assert(exp.GetMessage() == "The product already exists!");
		assert(true);
	}

	int pos = repo.search("a", "a");
	assert(pos == 0);
	pos = repo.search("b", "a");
	assert(pos == -1);

	repo.remove(0);
	try {
		repo.remove(-1);
	}
	catch (const RepoException& exp) {
		assert(exp.GetMessage() == "Invalid index!");
		assert(true);
	}
	std::cout << "Repo tested successfully!\n";
}

void testService() {
	ProductRepo repo;
	ProductService serv{ repo };

	Product p1{ 123.12f, "a", "a", "a" };
	serv.addService(p1.getPrice(), p1.getName(), p1.getType(), p1.getMan());
	assert(serv.getAllProducts().size() == 1);

	vector<Product> products = serv.getAllProducts();
	assert(products.at(0) == p1);

	try {
		serv.addService(123, "", "sa", "a");
	}
	catch (const ValidatorException& exp) {
		assert(exp.GetMessage() == "Invalid name!\n");
	}

	assert(serv.searchService("a", "a").getName() == "a");
	assert(serv.searchService("a", "a").getMan() == "a");

	try {
		serv.searchService("b", "b");
	}
	catch (const RepoException& exp) {
		assert(exp.GetMessage() == "The product was not found!");
	}

	serv.modifyService("a", "a", "b", "b", "b", 12);
	assert(serv.getAllProducts().at(0).getType() == "b");

	try
	{
		serv.modifyService("a", "a", "b", "b", "b", 12);
	}
	catch (const RepoException& exp) {
		assert(exp.GetMessage() == "The product was not found!");
	}

	try {
		serv.modifyService("b", "b", "", "", "", -1);
	}
	catch (const ValidatorException& exp) {
		assert(exp.GetMessage() == "Invalid price!\nInvalid name!\nInvalid type!\nInvalid manufacturer!\n");
	}

	serv.removeService("b", "b");
	assert(serv.getAllProducts().size() == 0);

	try
	{
		serv.removeService("b", "b");
	}
	catch (const RepoException& exp) {
		assert(exp.GetMessage() == "The product does not exist!");
	}

	serv.addService(3, "A1", "A2", "A3");
	serv.addService(1, "B1", "B2", "B3");
	serv.addService(4, "C1", "C2", "C3");
	serv.addService(213123, "A1", "HH", "H");
	std::map<const string, int> m;
	m = serv.raportType();
	assert(m["A2"] == 1);
	assert(m["C2"] == 1);

	vector<Product> prod = serv.genericSorting(0, 1);
	assert(prod.at(0).getName() == "B1");
	prod = serv.genericSorting(0, 2);
	assert(prod.at(1).getName() == "A1");
	prod = serv.genericSorting(0, 3);
	assert(prod.at(3).getName() == "C1");

	prod = serv.genericSorting(1, 1);
	assert(prod.at(2).getName() == "A1");
	prod = serv.genericSorting(1, 2);
	assert(prod.at(1).getName() == "B1");
	prod = serv.genericSorting(1, 3);
	assert(prod.at(0).getName() == "C1");

	vector<Product> test = serv.genericFiltering(2, "A1", 0);
	assert(test.size() == 2);
	test = serv.genericFiltering(1, "", 1);
	assert(test.size() == 1);
	test = serv.genericFiltering(3, "A3", 0);
	assert(test.size() == 1);

	std::cout << "Service tested successfully!\n";
}

void testBasket() {
	ProductRepo repo;
	Product p1{ 123, "masina", "suv", "mercedes" };
	repo.add(p1);
	Product p2{ 67, "lapte", "aaa", "napolact" };
	repo.add(p2);
	ProductService serv{ repo };

	serv.addBasket("masina");
	assert(serv.getAllBasket().size() == 1);
	serv.addBasket("lapte");
	assert(serv.getAllBasket().size() == 2);
	vector<Product> bask = serv.getAllBasket();
	assert(bask.at(0).getName() == "masina");
	assert(bask.at(1).getType() == "aaa");

	serv.generateBasket(3);
	assert(serv.getAllBasket().size() == 5);
	serv.removeBasket();
	assert(serv.getCost() == 0);
	try {
		serv.addBasket("a");
	}
	catch (const RepoException& exp) {
		assert(exp.GetMessage() == "The product does not exist!\n");
	}
	serv.removeService("masina", "suv");
	serv.removeService("lapte", "aaa");
	try {
		serv.generateBasket(3);
	}
	catch (const RepoException& exp) {
		assert(exp.GetMessage() == "There are no products!\n");
	}
	serv.addService(5.5f, "Lapte", "Lactate", "Napolact");
	serv.addBasket("Lapte");

	std::string fileCSV = "test_export.csv";
	serv.exportBasketCSV(fileCSV);

	std::ifstream finCSV(fileCSV);
	assert(finCSV.is_open());

	std::string linie;
	std::getline(finCSV, linie);
	assert(linie == "Name,Type,Price,Manufacturer");

	finCSV.close();
	std::remove(fileCSV.c_str());

	try {
		serv.exportBasketCSV("Z:/folder_care_nu_exista/test.csv");
	}
	catch (const RepoException& ex) {
		assert(ex.GetMessage() == "Could not open the file!\n");
	}

	std::string fileHTML = "test_export.html";
	serv.exportBasketHTML(fileHTML);

	std::ifstream finHTML(fileHTML);
	assert(finHTML.is_open());

	std::string primaLinieHTML;
	std::getline(finHTML, primaLinieHTML);
	assert(primaLinieHTML == "<html><body>");

	finHTML.close();
	std::remove(fileHTML.c_str());
	try {
		serv.exportBasketHTML("Z:/folder_care_nu_exista/test.html");

	}
	catch (const RepoException& ex) {
		assert(ex.GetMessage() == "Could not open the file!\n");
	}
	std::cout << "Basket tested succesfully!\n";
}

void testUndo() {
	ProductRepo testRepo;
	ProductService testSrv{ testRepo };

	testSrv.addService(5.5f, "Paine", "Panificatie", "VelPitar");
	assert(testRepo.getAll().size() == 1);

	testSrv.undo();
	assert(testRepo.getAll().size() == 0);

	testSrv.addService(7.0f, "Lapte", "Lactate", "Zuzu");
	assert(testRepo.getAll().size() == 1);

	testSrv.removeService("Lapte", "Lactate");
	assert(testRepo.getAll().size() == 0);

	testSrv.undo();
	assert(testRepo.getAll().size() == 1);
	assert(testRepo.getAll().at(0).getName() == "Lapte");

	testSrv.modifyService("Lapte", "Lactate", "Lapte", "Lactate", "Zuzu", 10.0f);
	assert(testRepo.getAll().at(0).getPrice() == 10.0f);

	testSrv.undo();
	assert(testRepo.getAll().at(0).getPrice() == 7.0f);

	testSrv.undo();
	assert(testRepo.getAll().size() == 0);

	try {
		testSrv.undo();assert(false);
	}
	catch (const RepoException&) {
		assert(true);
	}
	std::cout << "Undo tested succesfully!\n";
}
void testeRepoDict() {
	RepoDict repo{ 0.0f };
	Product p1{ 10.0f, "Lapte", "Lactate", "Zuzu" };
	Product p2{ 5.0f, "Paine", "Panificatie", "VelPitar" };

	repo.add(p1);
	repo.add(p2);
	assert(repo.getAll().size() == 2);

	int pos = repo.search("Lapte", "Lactate");
	assert(pos != -1);
	assert(repo.search("NuExista", "Nicaieri") == -1);

	Product p3{ 12.0f, "Lapte-Slab", "Lactate", "Zuzu" };
	repo.modify(pos, p3);
	assert(repo.search("Lapte-Slab", "Lactate") == pos);

	repo.remove(pos);
	assert(repo.getAll().size() == 1);

	RepoDict repoGhinion{ 1.0f };
	try {
		repoGhinion.add(p1);assert(false);
	}
	catch (const RepoException& ex) {
		assert(ex.GetMessage() == "Actiunea a picat din cauza probabilitatii!");
	}

	try {
		repoGhinion.getAll();assert(false);
	}
	catch (const RepoException&) {
		assert(true);
	}

	assert(repoGhinion.search("a", "b") == -1);

	std::cout << "RepoDict tested successfully!\n";
}
void runTests() {
	std::cout << "Testing begins...\n";
	testDomain();
	testRepo();
	testService();
	testBasket();
	testUndo();
	testeRepoDict();
}