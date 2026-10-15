#include"BasketCRUDGUI.h"

BasketGUI::BasketGUI(ProductService& service) noexcept : serv{ service } {
	initBasketGUI();
	loadDataBasket();
	initConnectBasket();
	serv.addObserver(this);
}

void BasketGUI::initBasketGUI() {
	setLayout(lyMain);
	table->setModel(model);
	
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	lyMain->addWidget(table);
	QVBoxLayout* lyDr = new QVBoxLayout;
	//buttons
	btns->addWidget(addBtn);
	btns->addWidget(emptyBtn);
	btns->addWidget(generateBtn);
	btns->addWidget(exitBtn);
	
	//txt

	txt->addRow("Name: ", txtName);
	txt->addRow("Number: ", txtNumber);
	txt->addRow("File: ", txtFile);

	lyDr->addLayout(txt);
	lyDr->addLayout(btns);
	lyMain->addLayout(lyDr);
	

}
void BasketGUI::loadDataBasket() {
	model->setProducts(serv.getAllBasket());
}
void BasketGUI::update()
{
	loadDataBasket();
}
void BasketGUI::initConnectBasket() {
	QObject::connect(exitBtn, &QPushButton::clicked, [this]() {
		close();
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
}
BasketGUI::~BasketGUI() noexcept{
	serv.deleteObserver(this);
}