#include "PartManager.hpp"

#include <iostream>
#include <fstream>
#include <string>


PartManager::PartManager()
{

}

int PartManager::getNextPart(const std::string& unitId)
{
	int id = _parts.size() + 100000;
	_parts[id] = Part(id);
	return id;
}


void PartManager::postPartStatus(int id, const std::string& msg)
{
	if (_parts.find(id) != _parts.end()) {
		_parts[id].status.push_back(msg);
	}
}

void PartManager::save(const std::string& filename) const
{
	std::ofstream outfile(filename);
	if (outfile.is_open() == false) {
		std::cerr << "Unable to open file: " << filename << std::endl;
		return;
	}
	outfile << (*this) << std::endl;
	
}

void PartManager::load(const std::string& filename)
{
	_parts.clear();
	std::ifstream infile(filename);
	if (infile.is_open() == false) {
		std::cerr << "Unable to open file: " << filename << std::endl;
		return;
	}
	infile >> (*this);
}

