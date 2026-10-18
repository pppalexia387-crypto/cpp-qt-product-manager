#pragma once
#include<vector>
#include<algorithm>
using std::vector;
class Observer {
public:
	virtual void update() = 0;
};


class Observable {
public:
	void addObserver(Observer* obs) {
		observers.push_back(obs);
	}
	void deleteObserver(Observer* obs) {
		observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
	}
	void doStuff() {
		notifyObservers();
	}

private:
	
	vector<Observer*> observers;
	void notifyObservers() {
		for (auto obs : observers) {
			obs->update();
		}
	}

};