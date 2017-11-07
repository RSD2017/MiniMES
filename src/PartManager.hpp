/* */
#ifndef PARTMANAGER_HPP
#define PARTMANAGER_HPP


#include "Order.hpp"

#include <map>
#include <boost/thread.hpp>

/**
* Represents a part with an id and status messages
*/
class Part
{
public:
	Part() {}

	Part(int id):
		id(id)
	{}

	int id;
	std::vector<std::string> status;
};


/**
* Managers the parts in the system
*/
class PartManager
{
public:

	PartManager();

	int getNextPart(const std::string& unitId);
	void postPartStatus(int id, const std::string& msg);

	void save(const std::string& filename) const;
	void load(const std::string& filename);

	friend std::ostream& operator<<(std::ostream& os, const PartManager&  manager)
	{
		for (std::map<int, Part>::const_iterator it = manager._parts.begin(); it != manager._parts.end(); ++it) {
			os << "Part " << (*it).first << std::endl;
			for (const std::string& str : (*it).second.status) {
				os << "\tStatus " << str << std::endl;
			}
		}
		return os;
	}

	friend std::istream& operator >> (std::istream& is, PartManager& manager)
	{
		std::string token;
		is >> token;
        while (!is.eof()) {
    		if (is.eof())
    			return is;
    		if (token != "Part") {
    			 // throw std::exception((std::string("Expected token 'Part' but got: ") + token).c_str());
    			 throw (std::string("Expected token 'Part' but got: ") + token).c_str();
    		}
    		Part part;
    		is >> part.id;
    		is >> token;
    		while (token == "Status" && is.eof() == false) {
    			std::string status;

    			char ch;
    			do {
    				is.read(&ch, 1);
    				if (ch != '\n' && is.eof() == false)
    					status.append(&ch, 1);
    			} while (ch != '\n' && is.eof() == false);

    			part.status.push_back(status);
    			is >> token;
    		}
    		manager._parts[part.id] = part;
        }
		return is;
	}


private:
	std::map<int, Part> _parts;
};


#endif //#ifndef PARTMANAGER_HPP
