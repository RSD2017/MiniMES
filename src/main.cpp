
#include <iostream>

#include "SocketCommunicator.hpp"
#include "Order.hpp"
#include "OrderManager.hpp"
#include "PartManager.hpp"

OrderManager orderManager;
PartManager partManager;


void saveOrders() {
	std::cerr << ">>>>>>>>>> SAVE ORDERS <<<<<<<<<<" << std::endl;
	std::cerr << "Enter filename: ";
	std::string filename;
	std::cin >> filename;

	orderManager.save(filename);
}

void loadOrders() {
	std::cerr << ">>>>>>>>>> LOAD ORDERS <<<<<<<<<<" << std::endl;
	std::cerr << "Enter filename: ";
	std::string filename;
	std::cin >> filename;
	try {
		orderManager.load(filename);
	}
	catch (const std::exception& exp) {
		std::cerr << "Exception: " << exp.what() << std::endl;
	}

}

void addOrder() {
	std::cerr << ">>>>>>>>>> ADD ORDER <<<<<<<<<<" << std::endl;
	std::cerr << "Add Task for Order: " << std::endl;
	Order::Ptr order = Order::Ptr(new Order());

	while (true) {
		std::cerr << "\tUnit Id (enter 'finished' for finishing order): ";
		std::string unitId;
		std::cin >> unitId;
		if (unitId == "finished")
			break;
		while (true) {
			std::cerr << "\tAdd Task (integer. Enter -1 for finishing task.): ";
			int task;
			std::cin >> task;
			if (task != -1) {
				order->addTask(unitId, task);
			}
			else {
				break;
			}
		}
	}
	if (order->isOrderFinished())
		return;
	orderManager.addOrder(order);
}

void printOrders() 
{
	std::cerr << ">>>>>>>>>> PRINT ORDERS <<<<<<<<<<" << std::endl;
	std::cerr << orderManager << std::endl;	
}

void loadParts() 
{
	std::cerr << ">>>>>>>>>> LOAD PARTS <<<<<<<<<<" << std::endl;
	std::cerr << "Enter filename: ";
	std::string filename;
	std::cin >> filename;
	try {
		partManager.load(filename);
	}
	catch (const std::exception& exp) {
		std::cerr << "Exception: " << exp.what() << std::endl;
	}

}

void saveParts()
{
	std::cerr << ">>>>>>>>>> SAVE PARTS <<<<<<<<<<" << std::endl;
	std::cerr << "Enter filename: ";
	std::string filename;
	std::cin >> filename;
	try {
		partManager.save(filename);
	}
	catch (const std::exception& exp) {
		std::cerr << "Exception: " << exp.what() << std::endl;
	}

}


void printParts()
{
	std::cerr << ">>>>>>>>>> PRINT PARTS <<<<<<<<<<" << std::endl;
	std::cerr << partManager << std::endl;
}

void mainMenu() {
	
	bool quit = false;
	while (!quit) {
		std::cerr << std::endl << std::endl;
		std::cerr << ">>>>>>>>>> MENU <<<<<<<<<<" << std::endl;
		std::cerr << "1: Add order" << std::endl;
		std::cerr << "2: Print Order Status" << std::endl;
		std::cerr << "3: Print Part Status" << std::endl;
		std::cerr << "4: Load orders from file " << std::endl;
		std::cerr << "5: Save orders to file " << std::endl;
		std::cerr << "6: Load Parts from file " << std::endl;
		std::cerr << "7: Save Parts to file " << std::endl;
		std::cerr << "0: quit" << std::endl;
		std::cerr << "Enter command: ";
		int cmd;
		std::cin >> cmd;
		switch (cmd) {
		case 0: quit = true;
			break;
		case 1: addOrder();
			break;
		case 2: printOrders();
			break;
		case 3: printParts();
			break;
		case 4: loadOrders();
			break;
		case 5: saveOrders();
			break;
		case 6: loadParts();
			break;
		case 7: saveParts();
			break;
		default:
			break;
		}
	}
}

int main(int argumentCount, char** argumentArray) {
	if (argumentCount < 2) {
		std::cerr << "Missing arguments: \"serverip\" and optionally filename for order file" << std::endl;
		return 0;
	}


	if (argumentCount > 2) {
		std::string filename = argumentArray[2];
		try {
			orderManager.load(filename);
		}
		catch (const std::exception& exp) {
			std::cerr << "Exception: " << exp.what() << std::endl;
		}
	}

	SocketCommunicator com(&orderManager, &partManager);
	com.startServer(argumentArray[1], 4567);
	
	mainMenu();
		

	return 0;

}





