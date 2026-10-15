#include"Ui.h"
#include"Validator.h"
#include<iostream>
#include<map>


void printMenu() {
	std::cout << "\n--------------------------MENU------------------------------\n";
	std::cout << "1. Add product\n";
	std::cout << "2. Remove product\n";
	std::cout << "3. Display products\n";
	std::cout << "4. Search product\n";
	std::cout << "5. Modify product\n";
	std::cout << "6. Sort products\n";
	std::cout << "7. Filter products\n";
	std::cout << "8. Shopping basket\n";
	std::cout << "9. Type report\n";
	std::cout << "10. Undo\n";
	std::cout << "0. Exit app\n";
}
void ProductUi::uiAdd() {
	float price;
	string name, type, man;
	std::cout << "Type: ";
	std::cin >> type;
	std::cout << "Name: ";
	std::cin >> name;
	std::cout << "Manufacturer: ";
	std::cin >> man;
	std::cout << "Price: ";
	std::cin >> price;
	try {
		serv.addService(price, name, type, man);
		std::cout << "Product added!";
	}
	catch (const ValidatorException& exp) {
		std::cout << exp.GetMessage();
	}
	catch (const RepoException& exp) {
		std::cout << exp.GetMessage();
	}
}
void ProductUi::uiDisplay() {
	try {
		if (serv.getAllProducts().size() == 0) {
			std::cout << "There are no products to display!\n";
			return;
		}
		else {
			for (const auto& p : serv.getAllProducts())
				std::cout << p.getName() << ", " << p.getType() << ", " << p.getMan() << ", " << p.getPrice() << "\n";
		}
	}
	catch (const RepoException& ex) {

		std::cout << ex.GetMessage() << "\n";
	}

}
void ProductUi::uiRemove() {
	string name, type;
	std::cout << "Type: ";
	std::cin >> type;
	std::cout << "Name: ";
	std::cin >> name;
	try {
		serv.removeService(name, type);
		std::cout << "Product removed!";
	}
	catch (const RepoException& exp) {
		std::cout << exp.GetMessage();
	}
}
void ProductUi::uiSearch() {
	string name, type;
	std::cout << "Type: ";
	std::cin >> type;
	std::cout << "Name: ";
	std::cin >> name;
	try {
		Product p=serv.searchService(name, type);
		std::cout << p.getName() << ", " << p.getType() << ", " << p.getMan() << ", " << p.getPrice() << "\n";
	}
	catch (const RepoException& exp) {
		std::cout << exp.GetMessage();
	}
}
void ProductUi::uiModify() {
	float newPrice;
	string name, type,newType,newName,newMan;
	std::cout << "Type: ";
	std::cin >> type;
	std::cout << "Name: ";
	std::cin >> name;
	
	std::cout << "New type: ";
	std::cin >> newType;
	std::cout << "New name: ";
	std::cin >> newName;
	std::cout << "New manufacturer: ";
	std::cin >> newMan;
	std::cout << "New price: ";
	std::cin >> newPrice;
	try {
		serv.modifyService(name, type, newName,newType,newMan,newPrice);
		std::cout << "Product modified!";
	}
	catch (const ValidatorException& exp) {
		std::cout << exp.GetMessage();
	}
	catch (const RepoException& exp) {
		std::cout << exp.GetMessage();
	}
}
void ProductUi::uiSort() {
	int order=-1, criteria=-1;
	while (order != 0 && order != 1) {
		std::cout << "\n0. Ascending\n1. Descending\nOrder: ";
		std::cin >> order;
		if (order == 0 || order == 1)
			break;
		std::cout << "Please enter a valid order!\n";
	}
	while (criteria != 1 && criteria != 2 && criteria != 3) {
		std::cout << "\n1. Price\n2. Name+Type\n3. Name\nCriteria: ";
		std::cin >> criteria;
		if (criteria == 1 || criteria == 2 || criteria == 3)
			break;
		std::cout << "Please enter a valid criteria!\n";
	}
	vector<Product> prod = serv.genericSorting(order, criteria);
	for (const auto& p: prod)
		std::cout << p.getName() << ", " << p.getType() << ", " << p.getMan() << ", " << p.getPrice() << "\n";


}
void ProductUi::uiFilter() {
	int criteria = -1;
	float price=0;
	string txt;
	while (criteria != 1 && criteria != 2 && criteria != 3) {
		std::cout << "\n1. Price\n2. Name\n3. Manufacturer\nCriteria: ";
		std::cin >> criteria;
		if (criteria == 1 || criteria == 2 || criteria == 3)
			break;
		std::cout << "Please enter a valid criteria!\n";
	}
	if (criteria == 1) {
		std::cout << "Please enter the price: ";
		std::cin >> price;
	}
	else if (criteria == 2) {
		std::cout << "Please enter the name: ";
		std::cin >> txt;
	}
	if (criteria == 3) {
		std::cout << "Please enter the manufacturer: ";
		std::cin >> txt;
	}
	vector<Product> prod = serv.genericFiltering(criteria,txt,price);
	for (const auto& p : prod)
		std::cout << p.getName() << ", " << p.getType() << ", " << p.getMan() << ", " << p.getPrice() << "\n";


}
void ProductUi::uiBasket() {
	std::cout << "1. Add to basket\n";
	std::cout << "2. Empty basket\n";
	std::cout << "3. Generate basket\n";
	std::cout << "4. Export basket\n";
	int cmd;
	std::cout << "Enter a command: ";
	std::cin >> cmd;
	switch (cmd) {
	case 1: {
		std::cout << "Name: ";
		string name;
		std::cin >> name;
		serv.addBasket(name);
		break;
	}
	case 2: {
		serv.removeBasket();
		std::cout << "Basket has been emptied succesfully!\n";
		break;
	}
	case 3: {
		int n;
		std::cout << "Please enter the number of products to generate: ";
		std::cin >> n;
		while (n <= 0) {
			std::cout << "Please enter a valid number: ";
			std::cin >> n;
		}
		try { 
			serv.generateBasket(n); }
		catch(const RepoException& exp){
			std::cout << exp.GetMessage();
			break;
		}
		
		std::cout << "The products have been added succesfully!\n";
		break;
	}

	case 4:
	{
		string f;
		std::cout << "Please enter the file name ( ex.csv / ex.html ): ";
		std::cin >> f;
		while (1) {
			if (f.find(".csv") != string::npos)
			{
				serv.exportBasketCSV(f);
				break;
			}
			else if (f.find(".html") != string::npos) {
				serv.exportBasketHTML(f);
				break;
			}
			else {
				std::cout << "Please enter a valid file name: ";
			    std::cin >> f;

			}
		}
		break;

	}
	case 5: {
		vector<Product> basket = serv.getAllBasket();
		if (basket.size() == 0) {
			std::cout << "There are no products to display!\n";
			return;
		}
		else {
			for (const auto& p : basket)
				std::cout << p.getName() << ", " << p.getType() << ", " << p.getMan() << ", " << p.getPrice() << "\n";
		}
		break;
	}
	default:
		std::cout << "Please enter a valid command!\n";
		break;


	}
	std::cout << "Basket cost: " << serv.getCost() << "\n";
}
void ProductUi::uiRaport() {
	std::map <const string, int> m;
	m = serv.raportType();
	std::cout << "------------------Type Report------------------------\n";
	for (auto& x : m) {
		std::cout << x.first << ": " << x.second<<"\n";
	}

}
void ProductUi::uiUndo() {
	try {
		serv.undo();
		std::cout << "Undo was successfull!\n";
	}
	catch (const RepoException& ex) {
		std::cout << "Eroare la undo: " << ex.GetMessage() << "\n";
	}
}
void ProductUi::Console() {
	int isRunning = 1;
	while (isRunning) {
		printMenu();
		std::cout << "Please enter a command: ";
		int cmd;
		std::cin >> cmd;
		switch (cmd) {
		case 1:
			uiAdd();
			break;
		case 2:
			uiRemove();
			break;
		case 3:
			uiDisplay();
			break;
		case 4:
			uiSearch();
			break;
		case 5:
			uiModify();
			break;
		case 6:
			uiSort();
			break;
		case 7:
			uiFilter();
			break;
		case 8:
			uiBasket();
			break;
		case 9:
			uiRaport();
			break;
		case 10:
			uiUndo();
			break;
		case 0: {
			std::cout << "Good bye!!\n";
			isRunning = 0;
			break;
		}
		default:
			std::cout << "Please enter a valid command!";
			break;
		}

	}
}