#pragma once

#include "Observer.h"

#include <vector>

class Subject
{
public:
	Subject() {}
	Subject(const Subject& subject)
	{
		for (auto o : observers_) {
			delete o;
		}

		observers_ = std::vector<Observer*>(subject.observers_);
	}
	~Subject()
	{
		for (auto o : observers_) {
			delete o;
		}

		std::vector<Observer*>().swap(observers_);
	}
	Subject& operator=(const Subject& subject)
	{
		for (auto o : observers_) {
			delete o;
		}

		observers_ = std::vector<Observer*>(subject.observers_);
	}
	void notify()
	{
		for (auto o : observers_) {
			o->update();
		}
	}
	void addObserver(Observer* observer)
	{
		observers_.push_back(observer);
	}

	void deleteObserver(Observer* observer)
	{
		auto result = std::find(observers_.begin(), observers_.end(), observer);

		if (result != observers_.end()) {
			observers_.erase(result);
		}
	}

private:
	std::vector<Observer*> observers_;
};