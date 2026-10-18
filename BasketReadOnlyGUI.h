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
#include<qpainter.h>
#include<ProductGUI.h>
#include<qgraphicsview.h>
#include<qgraphicsitem.h>
#include<qgraphicsscene.h>

using std::map;
class BasketReadOnlyGUI : public QWidget ,public Observer{
public:
	BasketReadOnlyGUI(ProductService& service) noexcept;
	~BasketReadOnlyGUI() noexcept;
private:
	ProductService& serv;

	//functions
	void initBasketGUI();
	void paintEvent(QPaintEvent* ev) override;
	void update() override;

	

};