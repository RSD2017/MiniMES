/* */
#ifndef SOCKETCOMMUNICATOR_HPP
#define SOCKETCOMMUNICATOR_HPP


#include "OrderManager.hpp"
#include "PartManager.hpp"
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <string>

/**
 * Provides a socket based server to which clients can connect to retrieve orders and part ids as well as post statuses.
 */
class SocketCommunicator
{
public:

	SocketCommunicator(OrderManager* orderManager, PartManager* partManager);
	

	bool startServer(const std::string& ip, int port);

	void waitForServer();

	
private:
	OrderManager* _orderManager;
	PartManager* _partManager;
	boost::asio::ip::tcp::socket* _socket;
	boost::asio::io_service _ioService;

	
	boost::thread* _thread;
	std::vector<boost::thread*> _socketCommunications;
	void runServer();
	void runCommunication(boost::asio::ip::tcp::socket* socket);
	std::string _serverip;
	int _serverport;

	bool _stopServer;
	bool _isRunning;
	
};


#endif //#ifndef MIRURCAP_MIRURCOMMUNICATOR_HPP
 