#include "Order.hpp"

#include <iostream>


int Order::_nextOrderId = 1;

Order::Order():
	_id(_nextOrderId++)
{

}


Order::Order(int id):
	_id(id)
{
	if (id > _nextOrderId)
		_nextOrderId = id+1;
}

int Order::getId() const
{
	return _id;
}

void Order::addTask(const std::string& unit, int val)
{	
	_tasks.push_back(Task(unit, val));
}

std::vector<int> Order::getTasks(const std::string& unit)
{
	std::vector<int> result;
	for (Task& task : _tasks) {
		if (task.taken == false) {
			if (task.unit == unit) {
				task.taken = true;
				result.push_back(task.val);
			}
		}
	}
	return result;
}

//
//int Order::getNextTask(const std::string& unit)
//{
//	for (Task& task : _tasks) {
//		if (task.taken == false) {
//			if (task.unit == unit) {
//				task.taken = true;
//				return task.val;
//			}
//		}
//	}
//	return 0;
//}

bool Order::isOrderFinished() const
{
	for (const Task& task : _tasks) {
		if (task.taken == false) {
			return false;
		}
	}
			
	return true;

}


void Order::postStatus(const std::string& status)
{
	_status.push_back(status);
}