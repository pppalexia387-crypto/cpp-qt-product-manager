#include"BasketReadOnlyGUI.h"

BasketReadOnlyGUI::BasketReadOnlyGUI(ProductService& service) noexcept :serv{ service } {
	serv.addObserver(this);
	std::srand(std::time(nullptr));
}


void BasketReadOnlyGUI::paintEvent(QPaintEvent* ev) {
	vector<Product> prods = serv.getAllBasket();
	QPainter p{ this };
	
	for (const auto& pr : prods) {
		
		p.drawEllipse(std::rand() % 400, std::rand() % 400, std::rand()%100,std::rand()%100);
	}
}

void BasketReadOnlyGUI::update()
{
	this->repaint();
}
BasketReadOnlyGUI::~BasketReadOnlyGUI() noexcept{
	serv.deleteObserver(this);
}
