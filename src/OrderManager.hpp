/* */
#ifndef ORDERMANAGER_HPP
#define ORDERMANAGER_HPP


#include "Order.hpp"

#include <list>
#include <boost/thread.hpp>

class OrderManager
{
public:

	OrderManager();
	
	void addOrder(Order::Ptr order);

	std::pair<int, std::vector<int> > getNextOrder(const std::string& unitId);

	std::list<Order::Ptr>& getOrders();


	void postOrderStatus(int id, const std::string& status);

	void save(const std::string& filename);
	void load(const std::string& filename);

	friend std::ostream& operator<<(std::ostream& os, const OrderManager&  manager)
	{
		for (std::map<int, Order::Ptr>::const_iterator it = manager._closedOrders.begin(); it != manager._closedOrders.end(); ++it) {
			os << *(*it).second << std::endl;
		}

		for (const Order::Ptr order : manager._openOrders) {
			os << (*order) << std::endl;
		}
		return os;
	}


private:
	std::list<Order::Ptr> _openOrders;
	std::map<int, Order::Ptr> _closedOrders;
	boost::mutex _mutex;

};


#endif //#ifndef ORDERMANAGER_HPP
 