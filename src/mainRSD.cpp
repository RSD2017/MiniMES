
#include <iostream>

#include "SocketCommunicator.hpp"
#include "Order.hpp"
#include "OrderScheduler.hpp"

OrderScheduler scheduler;


void saveOrders() {
	std::cout << ">>>>>>>>>> SAVE ORDERS <<<<<<<<<<" << std::endl;
	std::cout << "Enter filename: ";
	std::string filename;
	std::cin >> filename;

	scheduler.save(filename);
}

void loadOrders() {
	std::cout << ">>>>>>>>>> SAVE ORDERS <<<<<<<<<<" << std::endl;
	std::cout << "Enter filename: ";
	std::string filename;
	std::cin >> filename;
	try {
		scheduler.load(filename);
	}
	catch (const std::exception& exp) {
		std::cerr << "Exception: " << exp.what() << std::endl;
	}

}

void addOrder() {
	std::cout << ">>>>>>>>>> ADD ORDER <<<<<<<<<<" << std::endl;
	std::cout << "Add Task for Order: " << std::endl;
	Order order;

	while (true) {
		std::cout << "\tUnit Id (enter 'finished' for finishing order): ";
		std::string unitId;
		std::cin >> unitId;
		if (unitId == "finished")
			break;
		std::cout << "\tAdd Task (integer): ";
		int task;
		std::cin >> task;
		order.addTask(unitId, task);
	}
	if (order.isOrderFinished())
		return;
	scheduler.addOrder(order);
}

void printOrders() 
{
	for (Order& order : scheduler.getOrders()) {
		std::cout << order << std::endl;
	}
}

void mainMenu() {
	
	bool quit = false;
	while (!quit) {
		std::cout << "Menu:" << std::endl;
		std::cout << "1: Add order" << std::endl;
		std::cout << "2: Print Order Status" << std::endl;
		std::cout << "3: Load orders from file " << std::endl;
		std::cout << "4: Save orders to file " << std::endl;
		std::cout << "0: quit" << std::endl;
		std::cout << "Enter command: ";
		int cmd;
		std::cin >> cmd;
		std::cout << "Got Command " << cmd << std::endl;
		switch (cmd) {
		case 0: quit = true;
			break;
		case 1: addOrder();
			break;
		case 2: printOrders();
			break;
		case 3: loadOrders();
			break;
		case 4: saveOrders();
			break;
		default:
			break;
		}
	}
}

int main(int argumentCount, char** argumentArray) {
	std::cout << "Parsing arguments.. ";
	std::cout.flush();

	
	Order order;
	order.addTask("UR1", 1);
	order.addTask("UR1", 2);
	order.addTask("UR1", 3);
	order.addTask("UR2", 4);
	order.addTask("UR2", 5);
	scheduler.addOrder(order);

	SocketCommunicator com(&scheduler);
	com.startServer("192.168.56.1", 4567);
	
	mainMenu();
	
	//com.waitForServer();


	return 0;

}





