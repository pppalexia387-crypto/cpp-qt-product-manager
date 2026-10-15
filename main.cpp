#include <QtWidgets/QApplication>
#include "ProductGUI.h"
#include "Tests.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    runTests();
    ProductRepo repo;
    ProductService service{ repo };

    ProductGUI gui{ service };
    gui.show();
   
    
    return app.exec();
}
    