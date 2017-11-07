#include "SocketCommunicator.hpp"

#include <iostream>
#include <map>
#include <string>
#include <sstream> 
#include <algorithm>
using namespace boost::asio::ip;

std::ostringstream stdout_logger;
std::map<int,int> partsList;
SocketCommunicator::SocketCommunicator(OrderManager* orderManager, PartManager* partManager):
	_isRunning(false),
	_orderManager(orderManager),
	_partManager(partManager)
{

}

std::vector<int> split(const std::string& s, char delimiter)
{
	std::vector<int> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)){
		if(token != " " && token != ""){
                        std::istringstream buffer(token);
                        int value;
			buffer >> value;
			tokens.push_back(value);
		}
	}
	return tokens;
}
bool SocketCommunicator::startServer(const std::string& ip, int port)
{
	_serverip = ip;
	_serverport = port;
	_stopServer = false;

	_thread = new boost::thread(boost::bind(&SocketCommunicator::runServer, this));
	return true;
}

namespace {

	bool getChar(boost::asio::ip::tcp::socket* socket, char* output) {
		return socket->read_some(boost::asio::buffer(output, 1)) > 0;
	}

	std::string readUntil(boost::asio::ip::tcp::socket* socket, char terminator) {
		char buffer[1024];
		size_t cnt = 0;
		getChar(socket, buffer);
		while (buffer[cnt] != terminator) {
			cnt++;
			bool read = getChar(socket, &(buffer[cnt]));
			if (!read) {
				cnt--;
//			} else {
//				std::cerr << buffer[cnt];
			}
		}
//		std::cerr << std::endl;
//		std::cerr << "cnt: " << cnt << std::endl;
		buffer[cnt] = 0;
		return std::string(buffer);
	}

	uint16_t getUInt16(boost::asio::ip::tcp::socket* socket) {
		uint16_t output = 0;
		getChar(socket, (char*)&output + 1);
		getChar(socket, (char*)&output + 0);
		return output;
	}

	//Extract a 32 bit unsigned int
	uint32_t getUInt32(boost::asio::ip::tcp::socket* socket) {
		uint32_t output = 0;
		char* a;
		getChar(socket, a );
		output += ((*a)-48)*1000;
		getChar(socket, a );
		output += ((*a)-48)*100;
		getChar(socket, a );
		output += ((*a)-48)*10;
		getChar(socket, a );
		output += (*a)-48;
		return output;
	}


	void send(boost::asio::ip::tcp::socket* socket, const std::string& str) {
		socket->send(boost::asio::buffer(str.c_str(), str.size()+1));
	}

	void sendBinaryInt32(boost::asio::ip::tcp::socket* socket, int val)
	{
		char buffer[4];
		buffer[3] = ((char*)(&val))[3];
		buffer[2] = ((char*)(&val))[2];
		buffer[1] = ((char*)(&val))[1];
		buffer[0] = ((char*)(&val))[0];
		socket->send(boost::asio::buffer(buffer, 4));
		boost::this_thread::sleep(boost::posix_time::milliseconds(20));
	}


	void send(boost::asio::ip::tcp::socket* socket, int val) {
		std::stringstream sstr;
		sstr << val;
		send(socket, sstr.str());
	}


}

void SocketCommunicator::waitForServer()
{
	_thread->join();
}


void SocketCommunicator::runCommunication(tcp::socket* socket)
{
	while (!_stopServer && socket->is_open()) {
		boost::system::error_code error;
		size_t available = socket->available(error);
		if (available == 0) {
			if (error == boost::asio::error::eof) {
				std::cerr << "Reached EOF" << std::endl;

				break;
			}
		}
		else {
			std::string message = readUntil(socket, '\n');
//			std::cerr << "Debug message: " << message << std::endl;
			std::istringstream input(message);
			std::string cmd;
			input >> cmd;
			if (cmd == "GET") {
				std::string type;
				input >> type;

				std::string unitId;
				input >> unitId;

				if (type == "ORDER") {
					std::cerr << "Get order id: " << std::endl;
					std::pair<int, std::vector<int> > task = _orderManager->getNextOrder(unitId);
					sendBinaryInt32(socket, task.first);
			                boost::this_thread::sleep(boost::posix_time::milliseconds(20));
					sendBinaryInt32(socket, task.second.size());
			                boost::this_thread::sleep(boost::posix_time::milliseconds(20));
					for (int i : task.second) {
						sendBinaryInt32(socket, i);
					}
					stdout_logger << task.first;
				}
				else if (type == "PART") {
					std::cerr << "Get part" << std::endl;
					int id = _partManager->getNextPart(unitId);
//					stdout_logger << ":"  << id << ",";
					sendBinaryInt32(socket, id);
				}
			}
			else if (cmd == "POST") {
				std::string type;
				input >> type;
				int id = getUInt32(socket);
				std::string msg = readUntil(socket, '\n');
				if (type == "ORDERSTATUS") {
					std::cerr << "Post order id: " << id << std::endl;
					_orderManager->postOrderStatus(id, msg);
					auto partids = split(msg,' ');
					std::cout << stdout_logger.str();
					for(auto id : partids){
						std::cout << ":" << id << "," << partsList[id];
					}
					partsList.clear();
//					std::cerr << "Map cleared..." << std::endl;
					std::cout << std::endl;
//std::string str = stdout_logger.str();
//str.erase(remove(str.begin(), str.end(), ' '), str.end());
//stdout_logger.str("");
//stdout_logger.clear();
//std::cout << str << std::endl;
				}
				else if (type == "PARTSTATUS") {
					std::cerr << "Post part id: " << id << std::endl;
					std::istringstream buffer(msg);
					int value;
					buffer >> value;   

					partsList[id] = value;
					_partManager->postPartStatus(id, msg);
					//stdout_logger << msg;
				}
			}
		}
//		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	}

}
class AutoSaver {
        bool _stopServer = false;
	public:

	void SaveThread(PartManager* partManager, OrderManager* orderManager){
		while(!_stopServer){
//			std::cerr << "Saving.." << std::endl;
			orderManager->save("orders_autosave.txt");
			partManager->save("parts_autosave.txt");
			boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
		}
	}
};

void SocketCommunicator::runServer() {
	try {
		_isRunning = true;
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(boost::asio::ip::address::from_string(_serverip), _serverport));
		while (!_stopServer)
		{
			tcp::socket* socket = new tcp::socket(io_service);
			acceptor.accept(*socket);
			boost::thread* thread = new boost::thread(boost::bind(&SocketCommunicator::runCommunication, this, socket));
			_socketCommunications.push_back(thread);
			AutoSaver autosaver;
			thread = new boost::thread(boost::bind(&AutoSaver::SaveThread,&autosaver,_partManager,_orderManager));
			_socketCommunications.push_back(thread);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		_isRunning = false;
	}

}
