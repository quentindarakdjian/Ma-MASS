# Ma-MASS

This repository is developed by AI Environnement and the University of La Rochelle based on the Multi-Agents Stochastic Simulations platform of the University of Nottingham [Jacob Chapman, Darren Robinson]. The code permits to make co-simulation via FMU with EnergyPlus. The platform is made of occupants behavior models.

## Guide

This guide is for **Windows**.
Use a compiler with at least the c++11 standard. No-MASS can be built using mingw64.

### Checkout

Clone the repository using git.

### Compiling

The [Boost](http://boost.org/users/download/) library is a prerequisit for compiling Ma-MASS. It should be linked to the C++ compiler

### Running

Complete the SimulationConfig file and generate the modelDescription file.

Take the compiled library (FMI.so or FML.dll) and place into the FMU zip file with the modelDescription file and the SimulationConfig file.

Place the file in the EnergyPlus folder, define the variables in the IDF and run EnergyPlus.
