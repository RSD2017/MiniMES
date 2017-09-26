#include "OrderManager.hpp"

#include <iostream>
#include <fstream>
#include <string>

OrderManager::OrderManager()
{

}

std::list<Order::Ptr>& OrderManager::getOrders()
{
	return _openOrders;
}

void OrderManager::addOrder(Order::Ptr order)
{
	boost::mutex::scoped_lock lock(_mutex);
	_openOrders.push_back(order);
}

std::pair < int, std::vector<int> > OrderManager::getNextOrder(const std::string& unitId)
{
	boost::mutex::scoped_lock lock(_mutex);
	if (_openOrders.size() > 0) {
		Order::Ptr order = _openOrders.front();
		if (order->isOrderFinished())
		{
			_closedOrders[order->getId()] = order;			
			_openOrders.pop_front();			
		}
		return std::make_pair(order->getId(), order->getTasks(unitId));
	}
	return std::make_pair(-1, std::vector<int>());
}


void OrderManager::postOrderStatus(int id, const std::string& status)
{
	if (_closedOrders.find(id) != _closedOrders.end()) {
		_closedOrders[id]->postStatus(status);
	}
	else {
		for (Order::Ptr order : _openOrders) {
			if (order->getId() == id) {
				order->postStatus(status);
			}
		}
	}
}

void OrderManager::save(const std::string& filename)
{
	std::ofstream outfile(filename);
	if (outfile.is_open() == false) {
		std::cerr << "Unable to open file: " << filename << std::endl;
		return;
	}
	
	for (std::map<int, Order::Ptr>::iterator it = _closedOrders.begin(); it != _closedOrders.end(); ++it) {
		outfile << *(*it).second << std::endl;
	}

	for (Order::Ptr order : _openOrders) {
		outfile << (*order) << std::endl;
	}

	outfile.close();
}


void OrderManager::load(const std::string& filename)
{
	_openOrders.clear();
	_closedOrders.clear();
	std::ifstream infile(filename);
	if (infile.is_open() == false) {
		std::cerr << "Unable to open file: " << filename << std::endl;
		return;
	}

	while (infile.eof() == false) {
		Order::Ptr order = boost::shared_ptr<Order>(new Order());
		infile >> (*order);
		if (infile.eof() == false) {
			if (order->isOrderFinished()) {
				_closedOrders[order->getId()] = order;
			}
			else {
				_openOrders.push_back(order);
			}
		}
	}
	infile.close();

}