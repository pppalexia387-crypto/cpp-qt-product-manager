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
#include<QTableWidget>
#include<QTableView>
#include<QListView>
#include"ProductModel.h"
#include<QStringList>
#include <map>

using std::map;

class ProductGUI : public QWidget, public Observer {
public:
	ProductGUI(ProductService& service) noexcept;
	~ProductGUI() noexcept;

private:
	ProductService& serv;
	QListView* list = new QListView;
	ProductModel* model = new ProductModel;

	
	QHBoxLayout* btns = new QHBoxLayout;

	//buttons
	QPushButton* btnAdd = new QPushButton{ "&add" };
	QPushButton* btnDelete = new QPushButton{ "&delete" };
	QPushButton* btnSearch = new QPushButton{ "&search" };
	QPushButton* btnModify = new QPushButton{ "&modify" };
	QPushButton* btnUndo = new QPushButton{ "&undo" };
	QPushButton* btnExit = new QPushButton{ "&exit" };
	QWidget* btnDyn = new QWidget;
	QPushButton* btnSort = new QPushButton{ "&sort" };
	QPushButton* btnFilter = new QPushButton{ "&filter" };
	
	//view buttons
	QPushButton* bskView = new QPushButton{ "&view basket" };
	QPushButton* productView = new QPushButton{ "&view products" };
	
	//basket buttons
	QPushButton* btnBasket = new QPushButton{ "&basket crud" };
	QPushButton* btnBasketDraw = new QPushButton{ "&basket drawing" };
	QPushButton* addBtn = new QPushButton("&add to basket");
	QPushButton* emptyBtn = new QPushButton("&empty basket");
	QPushButton* generateBtn = new QPushButton("&generate basket");
	QPushButton* exportBtn = new QPushButton("&export basket");
	
	//text fields
	QLineEdit* txtName = new QLineEdit;
	QLineEdit* txtType = new QLineEdit;
	QLineEdit* txtMan = new QLineEdit;
	QLineEdit* txtPrice = new QLineEdit;
	QLineEdit* txtNumber = new QLineEdit;
	QLineEdit* txtFile = new QLineEdit;

	//layouts
	QVBoxLayout* lyBtnDy = new QVBoxLayout;
	QHBoxLayout* lyFunc = new QHBoxLayout;

	void initConnect();
	void loadData();
	void initGUI();
	void addButtons();
	void openSortWindow();
	void openFilterWindow();
	void loadDataBasket();
	void update() override;
	
};