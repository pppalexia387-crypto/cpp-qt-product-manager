#include "ProductGUI.h"
#include "BasketCRUDGUI.h"
#include"BasketReadOnlyGUI.h"
#include"ProductModel.h"

ProductGUI::ProductGUI(ProductService& service) noexcept : serv{ service } {
	initGUI();
	loadData();
	initConnect();
}

void ProductGUI::initConnect() {
	QObject::connect(btnExit, &QPushButton::clicked, [&]() {
		close();
		});
	QObject::connect(btnAdd, &QPushButton::clicked, [&]() {
		auto name = txtName->text();
		std::string name1 = name.toStdString();
		auto type = txtType->text();
		std::string type1 = type.toStdString();
		auto man = txtMan->text();
		std::string man1 = man.toStdString();
		auto price = txtPrice->text();

		if (name.isEmpty() || price.isEmpty() || type.isEmpty() || man.isEmpty()) {

			QMessageBox::warning(this, "Error", "Please fill out all of the fields!");
			return;
		}
		bool ok;
		float price1 = price.toFloat(&ok);
		if (ok) {
			try {
				serv.addService(price1, name1, type1, man1);
				loadData();
			}
			catch (const RepoException& exp) {

				QMessageBox::critical(this, "Error", QString::fromStdString(exp.GetMessage()));
			}
		}
		else {
			QMessageBox::warning(this, "Error", "Invalid price!");
			return;
		}
		});
	QObject::connect(btnDelete, &QPushButton::clicked, [&]() {
		auto name = txtName->text();
		std::string name1 = name.toStdString();
		auto type = txtType->text();
		std::string type1 = type.toStdString();
		if (name.isEmpty() || type.isEmpty()) {

			QMessageBox::warning(this, "Error", "Please fill out the name and type!");
			return;
		}

		try {
			serv.removeService(name1, type1);
			loadData();
		}
		catch (RepoException& exp) {
			QMessageBox::critical(this, "Error", QString::fromStdString(exp.GetMessage()));
		}
		});
	QObject::connect(btnSearch, &QPushButton::clicked, [&]() {
		auto name = txtName->text();
		std::string name1 = name.toStdString();
		auto type = txtType->text();
		std::string type1 = type.toStdString();
		if (name.isEmpty() || type.isEmpty()) {

			QMessageBox::warning(this, "Error", "Please fill out the name and type!");
			return;
		}

		try {
			const Product p = serv.searchService(name1, type1);
			auto price = p.getPrice();
			auto man = p.getMan();
			QString txt = QString("%1 %2 %3 %4").arg(name, type, QString::fromStdString(man), QString::number(price));
			QMessageBox::information(this, "Product found", txt);

		}
		catch (RepoException& exp) {
			QMessageBox::critical(this, "Error", QString::fromStdString(exp.GetMessage()));
		}
		});
	QObject::connect(btnModify, &QPushButton::clicked, [&]() {
		auto name = txtName->text();
		std::string name1 = name.toStdString();
		auto type = txtType->text();
		std::string type1 = type.toStdString();
		auto man = txtMan->text();
		std::string man1 = man.toStdString();
		auto price = txtPrice->text();

		if (name.isEmpty() || price.isEmpty() || type.isEmpty() || man.isEmpty()) {

			QMessageBox::warning(this, "Error", "Please fill out all of the fields!");
			return;
		}
		bool ok;
		float price1 = price.toFloat(&ok);
		if (ok) {
			try {
				serv.modifyService(name1, type1, name1, type1, man1, price1);
				loadData();
			}
			catch (const RepoException& exp) {

				QMessageBox::critical(this, "Error", QString::fromStdString(exp.GetMessage()));
			}
		}
		else {
			QMessageBox::warning(this, "Error", "Invalid price!");
			return;
		}
		});
	QObject::connect(list->selectionModel(), &QItemSelectionModel::selectionChanged, [this]() {
		QModelIndexList sel = list->selectionModel()->selectedIndexes();

		if (sel.isEmpty()) {
			txtName->setText("");
			txtType->setText("");
			txtMan->setText("");
			txtPrice->setText("");
		}
		else {
			auto selIndex = sel.at(0);

			auto name = selIndex.data(Qt::UserRole + 3).toString();
			auto type = selIndex.data(Qt::UserRole).toString();
			auto man = selIndex.data(Qt::UserRole + 1).toString();
			auto price = selIndex.data(Qt::UserRole + 2).toFloat();

			txtName->setText(name);
			txtType->setText(type);
			txtMan->setText(man);
			txtPrice->setText(QString::number(price));
		}
		});
	QObject::connect(btnSort, &QPushButton::clicked, [&]() {
		openSortWindow();
		return;
		});
	QObject::connect(btnFilter, &QPushButton::clicked, [&]() {
		openFilterWindow();
		return;
		});
	QObject::connect(btnUndo, &QPushButton::clicked, [&]() {
		try {
			serv.undo();
			loadData();
			
		}
		catch (const RepoException& exp) {
			QMessageBox::critical(this, "Error", QString::fromStdString(exp.GetMessage()));
		}
		});
	QObject::connect(btnBasket, &QPushButton::clicked, [&]() {
		BasketGUI* basket = new BasketGUI(serv);
		basket->show();
		});
	QObject::connect(addBtn, &QPushButton::clicked, [this]() {
		auto name = txtName->text();
		std::string name1 = name.toStdString();


		if (name.isEmpty()) {

			QMessageBox::warning(this, "Error", "Please fill out the name field!");
			return;
		}

		try {
			serv.addBasket(name1);
			loadDataBasket();
		}
		catch (const RepoException& exp) {

			QMessageBox::critical(this, "Error", QString::fromStdString(exp.GetMessage()));
		}


		});
	QObject::connect(emptyBtn, &QPushButton::clicked, [this]() {
		serv.removeBasket();
		loadDataBasket();
		});
	QObject::connect(exportBtn, &QPushButton::clicked, [this]() {
		auto name = txtFile->text();
		std::string f = name.toStdString();


		if (name.isEmpty()) {

			QMessageBox::warning(this, "Error", "Please fill out the file field!");
			return;
		}

		try {
			if (f.find(".csv") != string::npos)
			{
				serv.exportBasketCSV(f);
				loadDataBasket();
			}
			else if (f.find(".html") != string::npos) {
				serv.exportBasketHTML(f);
				loadDataBasket();
			}
			else {
				QMessageBox::warning(this, "Error", "Please enter a valid file name (.csv/ .html)");
				return;

			}

		}
		catch (const RepoException& exp) {

			QMessageBox::critical(this, "Error", QString::fromStdString(exp.GetMessage()));
		}

		});
	QObject::connect(generateBtn, &QPushButton::clicked, [this]() {
		auto number = txtNumber->text();

		if (number.isEmpty()) {

			QMessageBox::warning(this, "Error", "Please fill out the number field!");
			return;
		}
		bool ok;
		int nr = number.toInt(&ok);
		if (ok) {
			try {
				serv.generateBasket(nr);
				loadDataBasket();
			}
			catch (const RepoException& exp) {
				QMessageBox::critical(this, "Error", QString::fromStdString(exp.GetMessage()));
				return;
			}
		}
		});
	QObject::connect(btnBasketDraw, &QPushButton::clicked, [this]() {
		BasketReadOnlyGUI* basketDraw = new BasketReadOnlyGUI{serv};
		basketDraw->show();

		});
	QObject::connect(bskView, &QPushButton::clicked, [this]() {
		loadDataBasket();
		});
	QObject::connect(productView, &QPushButton::clicked, [this]() {
		loadData();
		});
}

void ProductGUI::loadData() {
	model->setProducts(serv.getAllProducts());
	addButtons();
}
void ProductGUI::initGUI() {
	QHBoxLayout* lyMain = new QHBoxLayout;
	setLayout(lyMain);
	QHBoxLayout* lyView = new QHBoxLayout;

	lyView->addWidget(bskView);
	lyView->addWidget(productView);
	QVBoxLayout* lySt = new QVBoxLayout;
	lySt->addLayout(lyView);
	list->setModel(model);
	lySt->addWidget(list);
	
	lyMain->addLayout(lySt);
	

	QFormLayout* lyForm = new QFormLayout;

	lyForm->addRow("Name: ", txtName);
	lyForm->addRow("Type: ", txtType);
	lyForm->addRow("Manufacturer: ", txtMan);
	lyForm->addRow("Price: ", txtPrice);
	lyForm->addRow("Number: ", txtNumber);
	lyForm->addRow("File: ", txtFile);

	btns->addWidget(btnAdd);
	btns->addWidget(btnDelete);
	btns->addWidget(btnSearch);
	btns->addWidget(btnModify);
	btns->addWidget(btnUndo);
	btns->addWidget(btnExit);

	lyFunc->addWidget(btnSort);
	lyFunc->addWidget(btnFilter);
	QHBoxLayout* lyBsk = new QHBoxLayout;
	lyBsk->addWidget(btnBasket);
	lyBsk->addWidget(btnBasketDraw);

	QHBoxLayout* lyBskFunc = new QHBoxLayout;
	lyBskFunc->addWidget(addBtn);
	lyBskFunc->addWidget(emptyBtn);
	lyBskFunc->addWidget(generateBtn);
	lyBskFunc->addWidget(exportBtn);

	QVBoxLayout* lyDr = new QVBoxLayout;
	lyDr->addLayout(lyForm);
	lyDr->addLayout(btns);
	lyDr->addLayout(lyFunc);
	lyDr->addLayout(lyBsk);
	lyDr->addLayout(lyBskFunc);

	lyMain->addLayout(lyDr);

	btnDyn->setLayout(lyBtnDy);
	lyMain->addWidget(btnDyn);
}

void ProductGUI::addButtons() {
	QLayoutItem* item;
	while ((item = lyBtnDy->takeAt(0)) != nullptr) {
		if (item->widget()) {
			delete item->widget();
		}
		delete item;
	}
	map<const string, int> m;
	m = serv.raportType();
	auto ct = 0;
	for (auto& x : m) {
		auto btn = new QPushButton{ QString::fromStdString(x.first) };
		ct = x.second;
		lyBtnDy->addWidget(btn);
		QObject::connect(btn, &QPushButton::clicked, [this, ct]() {

			QMessageBox::information(this, "Raport", QString::number(ct));
			});
	}
}

void ProductGUI::openSortWindow() {
	QWidget* sortWindow = new QWidget();
	sortWindow->setAttribute(Qt::WA_DeleteOnClose);

	QHBoxLayout* lyMain = new QHBoxLayout(sortWindow);

	QListWidget* sortedList = new QListWidget;
	lyMain->addWidget(sortedList);


	QVBoxLayout* lySort = new QVBoxLayout;
	QPushButton* ascName = new QPushButton("Ascending sort by name");
	QPushButton* descName = new QPushButton("Descending sort by name");
	QPushButton* ascPrice = new QPushButton("Ascending sort by price");
	QPushButton* descPrice = new QPushButton("Descending sort by price");
	QPushButton* ascNameType = new QPushButton("Ascending sort by name and type");
	QPushButton* descNameType = new QPushButton("Descending sort by name and type");
	lySort->addWidget(ascName);
	lySort->addWidget(descName);
	lySort->addWidget(ascPrice);
	lySort->addWidget(descPrice);
	lySort->addWidget(ascNameType);
	lySort->addWidget(descNameType);
	lyMain->addLayout(lySort);
	
	QObject::connect(ascName, &QPushButton::clicked, [this, sortedList]() {
		sortedList->clear();
		//asc by name
		vector<Product> prods = serv.genericSorting(0, 3);

		for (const auto& p : prods) {
			QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(p.getName()));
			item->setData(Qt::UserRole, QString::fromStdString(p.getType()));
			item->setData(Qt::UserRole + 1, QString::fromStdString(p.getMan()));
			item->setData(Qt::UserRole + 2, QString::number(p.getPrice()));
			sortedList->addItem(item);
		}
		});
	QObject::connect(descName, &QPushButton::clicked, [this, sortedList]() {
		sortedList->clear();
		//desc by name 
		vector<Product> prods = serv.genericSorting(1, 3);
		for (const auto& p : prods) {
			QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(p.getName()));
			item->setData(Qt::UserRole, QString::fromStdString(p.getType()));
			item->setData(Qt::UserRole + 1, QString::fromStdString(p.getMan()));
			item->setData(Qt::UserRole + 2, QString::number(p.getPrice()));
			sortedList->addItem(item);
		}
		});
	QObject::connect(ascPrice, &QPushButton::clicked, [this, sortedList]() {
		sortedList->clear();
		
		//asc by price
		vector<Product> prods= serv.genericSorting(0, 1);
		
		for (const auto& p : prods) {
			QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(p.getName()));
			item->setData(Qt::UserRole, QString::fromStdString(p.getType()));
			item->setData(Qt::UserRole + 1, QString::fromStdString(p.getMan()));
			item->setData(Qt::UserRole + 2, QString::number(p.getPrice()));
			sortedList->addItem(item);
		}
		});
	QObject::connect(descPrice, &QPushButton::clicked, [this, sortedList]() {
		sortedList->clear();
		//desc by price
		vector<Product> prods = serv.genericSorting(1, 1);
		for (const auto& p : prods) {
			QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(p.getName()));
			item->setData(Qt::UserRole, QString::fromStdString(p.getType()));
			item->setData(Qt::UserRole + 1, QString::fromStdString(p.getMan()));
			item->setData(Qt::UserRole + 2, QString::number(p.getPrice()));
			sortedList->addItem(item);
		}
		});
	QObject::connect(ascNameType, &QPushButton::clicked, [this, sortedList]() {
		sortedList->clear();
		//asc by name and type
		vector<Product> prods = serv.genericSorting(0, 2);

		for (const auto& p : prods) {
			QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(p.getName()));
			item->setData(Qt::UserRole, QString::fromStdString(p.getType()));
			item->setData(Qt::UserRole + 1, QString::fromStdString(p.getMan()));
			item->setData(Qt::UserRole + 2, QString::number(p.getPrice()));
			sortedList->addItem(item);
		}
		});
	QObject::connect(descNameType, &QPushButton::clicked, [this, sortedList]() {
		sortedList->clear();
		//asc by name and type
		vector<Product> prods = serv.genericSorting(1, 2);

		for (const auto& p : prods) {
			QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(p.getName()));
			item->setData(Qt::UserRole, QString::fromStdString(p.getType()));
			item->setData(Qt::UserRole + 1, QString::fromStdString(p.getMan()));
			item->setData(Qt::UserRole + 2, QString::number(p.getPrice()));
			sortedList->addItem(item);
		}
		});
	


	sortWindow->show();
}
void ProductGUI::openFilterWindow() {
	QWidget* filterWindow = new QWidget();
	filterWindow->setAttribute(Qt::WA_DeleteOnClose);
	QHBoxLayout* lyMain = new QHBoxLayout(filterWindow);
	QListWidget* filterList = new QListWidget;

	lyMain->addWidget(filterList);
	QPushButton* nameFilter = new QPushButton("Name filtering");
	QPushButton* priceFilter = new QPushButton("Price filtering");
	QPushButton* manFilter = new QPushButton("Manufacturer filtering");

	QVBoxLayout* lyFilter = new QVBoxLayout;
	lyFilter->addWidget(nameFilter);
	lyFilter->addWidget(priceFilter);
	lyFilter->addWidget(manFilter);

	lyMain->addLayout(lyFilter);
	//text fields
	QLineEdit* txtName1 = new QLineEdit;
	QLineEdit* txtPrice1 = new QLineEdit;
	QLineEdit* txtMan1 = new QLineEdit;


	QFormLayout* lyTxt = new QFormLayout;
	lyTxt->addRow("Name: ", txtName1);
	lyTxt->addRow("Manufacturer", txtMan1);
	lyTxt->addRow("Price: ", txtPrice1);
	lyMain->addLayout(lyTxt);
	QObject::connect(nameFilter, &QPushButton::clicked, [this,filterList,txtName1]() {
		filterList->clear();
		auto name = txtName1->text();
		if (name.isEmpty()) {

			QMessageBox::warning(this, "Error", "Please fill out the name field!");
			return;
		}
		std::string name1 = name.toStdString();
		vector<Product> prods = serv.genericFiltering(2, name1, 0);
		for (const auto& p : prods) {
			QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(p.getName()));
			item->setData(Qt::UserRole, QString::fromStdString(p.getType()));
			item->setData(Qt::UserRole + 1, QString::fromStdString(p.getMan()));
			item->setData(Qt::UserRole + 2, QString::number(p.getPrice()));
			filterList->addItem(item);
		}
		});

	QObject::connect(priceFilter, &QPushButton::clicked, [this,filterList,txtPrice1]() {
		filterList->clear();
		auto price = txtPrice1->text();
		if (price.isEmpty()) {
			QMessageBox::warning(this, "Error", "Please fill out the price field!");
			return;
		}
		bool ok;
		float price1 = price.toFloat(&ok);
		if (!ok) {
			QMessageBox::warning(this, "Error", "Invalid price");
			return;
		}
		else {
			vector<Product> prods = serv.genericFiltering(1, "", price1);
			for (const auto& p : prods) {
				QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(p.getName()));
				item->setData(Qt::UserRole, QString::fromStdString(p.getType()));
				item->setData(Qt::UserRole + 1, QString::fromStdString(p.getMan()));
				item->setData(Qt::UserRole + 2, QString::number(p.getPrice()));
				filterList->addItem(item);
			}
		}});

	QObject::connect(manFilter, &QPushButton::clicked, [this,filterList,txtMan1]() {
			filterList->clear();
			auto man = txtMan1->text();
			
			if (man.isEmpty()) {

				QMessageBox::warning(this, "Error", "Please fill out the manufacturer field!");
				return;
			}
			std::string man1 = man.toStdString();
			
			vector<Product> prods = serv.genericFiltering(3, man1, 0);
			for (const auto& p : prods) {
				QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(p.getName()));
				item->setData(Qt::UserRole, QString::fromStdString(p.getType()));
				item->setData(Qt::UserRole + 1, QString::fromStdString(p.getMan()));
				item->setData(Qt::UserRole + 2, QString::number(p.getPrice()));
				filterList->addItem(item);
			}
			});
	
		filterWindow->show();
}
void ProductGUI::loadDataBasket() {
	model->setProducts(serv.getAllBasket());
}

void ProductGUI::update()
{
	loadDataBasket();
	
}

ProductGUI::~ProductGUI() noexcept{
	serv.deleteObserver(this);
}
		

