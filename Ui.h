#pragma once
#include "Service.h"

class ProductUi {
private:
    
    ProductService& serv;
    void uiAdd();
    void uiDisplay();
    void uiRemove();
    void uiSearch();
    void uiModify();
    void uiSort();
    void uiFilter();
    void uiBasket();
    void uiRaport();
    void uiUndo();
public:
   
    ProductUi(ProductService& service) noexcept: serv{ service } {}
    void Console();
};