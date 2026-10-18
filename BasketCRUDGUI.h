#pragma once
#include <QMessageBox>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <Service.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/QApplication.h>
#include <map>
#include<qtablewidget.h>
#include<qheaderview.h>
#include<ProductGUI.h>
#include<QTableView>

using std::map;
class BasketGUI: public QWidget ,public Observer{
	private:
	ProductService& serv;
	QTableView* table = new QTableView;
	ProductModel* model=new ProductModel;

	//layouts
	QHBoxLayout* lyMain = new QHBoxLayout;
	QHBoxLayout* btns = new QHBoxLayout;
	QFormLayout* txt = new QFormLayout;

	//buttons
	QPushButton* addBtn = new QPushButton("&add to basket");
	QPushButton* emptyBtn = new QPushButton("&empty basket");
	QPushButton* generateBtn = new QPushButton("&generate basket");
	QPushButton* exportBtn = new QPushButton("&export basket");
	QPushButton* exitBtn = new QPushButton("&exit");


	//txt fields
	QLineEdit* txtName = new QLineEdit;
	QLineEdit* txtType = new QLineEdit;
	QLineEdit* txtMan = new QLineEdit;
	QLineEdit* txtPrice = new QLineEdit;
	QLineEdit* txtNumber = new QLineEdit;
	QLineEdit* txtFile = new QLineEdit;

	void initBasketGUI();
	void initConnectBasket();
	
	void loadDataBasket();
	void update() override;

public:

	
	BasketGUI(ProductService& service) noexcept;
	~BasketGUI() noexcept;
};