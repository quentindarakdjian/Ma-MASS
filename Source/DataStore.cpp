
#include <vector>
#include <iostream>
#include <fstream>
#include "DataStore.h"

std::unordered_map<std::string, std::vector<double> > DataStore::variableMap;

DataStore::DataStore() {
}

void DataStore::addVariable(std::string name) {
    variableMap.insert(std::make_pair(name, std::vector<double>()));
}

void DataStore::addValue(std::string name, double value) {
    variableMap[name].push_back(value);
}

double DataStore::getValueForZone(std::string name, std::string zoneName){
    return getValue(zoneName + name);
}

double DataStore::getValue(std::string name) {
    if (variableMap.find(name) == variableMap.end()) {
        std::cout << "Forgot to define: " << name << std::endl;
        std::cout << "Check the Zone Name is correct in the NoMass Simulation File" << std::endl;
        std::cout << std::flush;
        exit(-1);
    }
    return variableMap[name].back(); // Returns the last value of the vector (unordered_map)
}

void DataStore::clear(){
    variableMap.clear();
}

void DataStore::print() {
  std::ofstream myfile;
  myfile.open("agent.csv");
  myfile << "stepCount,";
  int maxSize = 0;
  for (std::unordered_map<std::string, std::vector<double> >::iterator it=variableMap.begin(); it != variableMap.end(); ++it) {
      myfile << it->first << ",";
      if (maxSize < it->second.size()) {
        maxSize = it->second.size();
      }
  }
  myfile << std::endl;
  for (unsigned int i =0; i < maxSize; i++) {
      myfile << i << ",";
      for (std::unordered_map<std::string, std::vector<double> >::iterator it=variableMap.begin(); it != variableMap.end(); ++it) {
          if (it->second.size() > i) {
              myfile << it->second.at(i);
          }
          myfile << ",";
      }
      myfile << std::endl;
  }
  myfile.close();
}
