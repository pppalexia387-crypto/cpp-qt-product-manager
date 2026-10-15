#pragma once
#include "AbstractRepo.h"
#include "Repo.h" 
#include <unordered_map>
#include <random>

class RepoDict : public AbstractRepo {
private:
    std::unordered_map<int, Product> products;
    float probabilitate;

    void aruncaExceptieRandom() const {
        float prob = ((float)rand() / RAND_MAX);
        if (prob < probabilitate) {
            throw RepoException("Actiunea a picat din cauza probabilitatii!");
        }
    }

public:
    RepoDict(float prob) : probabilitate{ prob } {}
    
    void add(const Product& p) override {
        aruncaExceptieRandom();
        int id = static_cast<int>(products.size());
        products[id] = p;
    }

    void remove(int pos) override {
        aruncaExceptieRandom();
        products.erase(pos);
    }

    void modify(int index, const Product& p) override {
        aruncaExceptieRandom();
        products[index] = p;
    }

    int search(const std::string& name, const std::string& type) noexcept override {
        for (const auto& pair : products) {
            if (pair.second.getName() == name && pair.second.getType() == type) {
                return pair.first;
            }
        }
        return -1;
    }

    std::vector<Product> getAll() override {
        aruncaExceptieRandom();
        std::vector<Product> rez;
        for (const auto& pair : products) {
            rez.push_back(pair.second);
        }
        return rez;
    }
};