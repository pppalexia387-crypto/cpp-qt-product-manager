#pragma once
#include "Product.h"
#include "Repo.h"

class ActiuneUndo {
public:
    virtual void doUndo() = 0;
    virtual ~ActiuneUndo() = default;
};

class UndoAdauga : public ActiuneUndo {
private:
    Product produsAdaugat;
    AbstractRepo& repo;
public:
    UndoAdauga(AbstractRepo& r, const Product& p) : repo{ r }, produsAdaugat{ p } {}

    void doUndo() override {
        int const pos = repo.search(produsAdaugat.getName(), produsAdaugat.getType());
        if (pos != -1) {
            repo.remove(pos);
        }
    }
};

class UndoSterge : public ActiuneUndo {
private:
    Product produsSters;
    AbstractRepo& repo;
public:
    UndoSterge(AbstractRepo& r, const Product& p) : repo{ r }, produsSters{ p } {}

    void doUndo() override {
        repo.add(produsSters);
    }
};
class UndoModifica : public ActiuneUndo {
private:
    Product produsVechi;
    Product produsNou; 
    AbstractRepo& repo;
public:
    UndoModifica(AbstractRepo& r, const Product& pOld, const Product& pNew)
        : repo{ r }, produsVechi{ pOld }, produsNou{ pNew } {
    }

    void doUndo() override {
        int const pos = repo.search(produsNou.getName(), produsNou.getType());

        if (pos != -1) {
            repo.modify(pos, produsVechi);
        }
    }
};