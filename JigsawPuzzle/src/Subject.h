#pragma once

#include "Observer.h"

#include <vector>

class Subject
{
public:
	~Subject()
	{
		std::vector<Observer>().swap(observers_);
	}
	void notify()
	{
		for (auto o : observers_) {
			o.update();
		}
	}
	void addObserver(Observer observer)
	{
		observers_.push_back(observer);
	}

private:
	std::vector<Observer> observers_;
};