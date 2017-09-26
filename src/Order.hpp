/* */
#ifndef ORDER_HPP
#define ORDER_HPP


#include <map>
#include <string>
#include <vector>
#include <iostream>


#include <boost/shared_ptr.hpp>

class Task
{
public:
	Task()
	{

	}

	Task(const std::string& unit, int val):
		val(val),
		unit(unit),
		taken(false)
	{}
		

	int val;
	std::string unit;
	bool taken;
};


class Order
{
public:
	typedef boost::shared_ptr<Order> Ptr;


	Order();
	Order(int id);

	void addTask(const std::string& unit, int val);

	std::vector<int> getTasks(const std::string& unit);

	bool isOrderFinished() const;

	int getId() const;

	void postStatus(const std::string& status);

	friend std::ostream& operator<<(std::ostream& os, const Order&  order)
	{
		os << "Order " << order._id << std::endl;
		for (const Task& task : order._tasks) {
			os << "\t Task " << task.unit << " " << task.val << " " << task.taken << std::endl;
		}
		for (const std::string& str : order._status) {
			os << "\tStatus " << str << std::endl;
		}
		os << "EndOrder"<< std::endl;
		return os;
	}

	friend std::istream& operator >> (std::istream& is, Order& order) {
		std::string token;
		is >> token;
		if (is.eof())
			return is;
		if (token != "Order") {
			// throw std::exception((std::string("Expected token 'Order' but got: ") + token).c_str());
			throw (std::string("Expected token 'Order' but got: ") + token).c_str();
		}
		is >> order._id;
		is >> token;
		while (token == "Task") {
			
			Task task;
			is >> task.unit;
			is >> task.val;
			is >> task.taken;
			order._tasks.push_back(task);

			is >> token;			
		}
		while(token == "Status") {
			std::string status;
			
			char ch;
			do {
				is.read(&ch, 1);
				if (ch != '\n' && is.eof() == false)
					status.append(&ch, 1);
			} while (ch != '\n' && is.eof() == false);
						
			order._status.push_back(status);
			is >> token;
		}
		return is;
	}

private:
	int _id;
	std::vector<Task> _tasks;
	std::vector<std::string> _status;

	static int _nextOrderId;
	
};




#endif //#ifndef ORDER_HPP
 
