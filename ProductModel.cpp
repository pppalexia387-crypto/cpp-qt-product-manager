#include "ProductModel.h"

int ProductModel::rowCount(const QModelIndex& parent) const {
    return products.size();
}

int ProductModel::columnCount(const QModelIndex& parent) const {
    return 4;
}

QVariant ProductModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= products.size()) {
        return QVariant();
    }

    const Product& p = products[index.row()];

    if (role == Qt::DisplayRole) {
        if (index.column() == 0) return QString::fromStdString(p.getName());
        if (index.column() == 1) return QString::fromStdString(p.getType());
        if (index.column() == 2) return QString::fromStdString(p.getMan());
        if (index.column() == 3) return QString::number(p.getPrice());
    }

    if (role == Qt::UserRole) return QString::fromStdString(p.getType());
    if (role == Qt::UserRole + 1) return QString::fromStdString(p.getMan());
    if (role == Qt::UserRole + 2) return p.getPrice();
    if (role == Qt::UserRole + 3) return QString::fromStdString(p.getName());

    return QVariant();
}

QVariant ProductModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == 0) return "Name";
        if (section == 1) return "Type";
        if (section == 2) return "Manufacturer";
        if (section == 3) return "Price";
    }
    return QVariant();
}

void ProductModel::setProducts(const std::vector<Product>& newProducts) {
    beginResetModel();
    products = newProducts;
    endResetModel();
}