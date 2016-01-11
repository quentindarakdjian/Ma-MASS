/* ---------------------------------------------------------------------------*
 * Implementation of the FMI interface based on functions and macros to
 * be defined by the includer of this file.
 * If FMI_COSIMULATION is defined, this implements "FMI for Co-Simulation 1.0",
 * otherwise "FMI for Model Exchange 1.0".
 * The "FMI for Co-Simulation 1.0", implementation assumes that exactly the
 * following capability flags are set to fmiTrue:
 *    canHandleVariableCommunicationStepSize, i.e. fmiDoStep step size can vary
 *    canHandleEvents, i.e. fmiDoStep step size can be zero
 * and all other capability flags are set to default, i.e. to fmiFalse or 0.
 *
 * Revision history
 *  07.02.2010 initial version for "Model Exchange 1.0" released in FMU SDK 1.0
 *  05.03.2010 bug fix: fmiSetString now copies the passed string argument
 *     and fmiFreeModelInstance frees all string copies
 *  11.12.2010 replaced calloc by functions.allocateMemory in fmiInstantiateModel
 *  04.08.2011 added support for "FMI for Co-Simulation 1.0"
 *
 * (c) 2011 QTronic GmbH
 * ---------------------------------------------------------------------------*/

#include <iterator>
#include <vector>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "DataStore.h"
#include "Simulation.h"
#include "SimulationConfig.h"
#include "fmuTemplate.h"
#include "fmiPlatformTypes.h"

// array of value references of states
#if NUMBER_OF_REALS>0
// Linux: declare vrStates to be static so that we get the local definition.
static fmiValueReference vrStates[NUMBER_OF_STATES] = STATES;
#endif

#ifndef max
#define max(a,b) ((a)>(b) ? (a) : (b))
#endif


// ---------------------------------------------------------------------------
// Private helpers used below to implement functions
// ---------------------------------------------------------------------------



// fname is fmiInstantiateModel or fmiInstantiateSlave
static fmiComponent instantiateModel(const char* fname, fmiString instanceName, fmiString GUID,
        fmiCallbackFunctions functions, fmiBoolean loggingOn) {

    if(valToRefs.empty()){
        modelInstance->sim.preprocess();
        loadVariables();
    }
    return modelInstance.get();
}

fmiComponent fmiInstantiateModel(fmiString instanceName, fmiString GUID, fmiCallbackFunctions functions, fmiBoolean loggingOn){
    //std::cout << "its a trap" << std::endl;
    return instantiateModel("",instanceName, GUID, functions, loggingOn);
}

// fname is fmiInitialize or fmiInitializeSlave


fmiStatus fmiInitialize(fmiComponent c, fmiBoolean toleranceControlled,
                                     fmiReal relativeTolerance, fmiEventInfo* eventInfo){
    return fmiOK;
}

// fname is fmiTerminate or fmiTerminateSlave
static fmiStatus terminate(const char* fname, fmiComponent c){
   printf("terminate\n");

    return fmiOK;
}

// fname is freeModelInstance of freeSlaveInstance
void freeInstance(char* fname, fmiComponent c) {
   printf("freeInstance\n");
}


// ---------------------------------------------------------------------------
// FMI functions: class methods not depending of a specific model instance
// ---------------------------------------------------------------------------

const char* fmiGetVersion() {
    return fmiVersion;
}

// ---------------------------------------------------------------------------
// FMI functions: for FMI Model Exchange 1.0 and for FMI Co-Simulation 1.0
// logging control, setters and getters for Real, Integer, Boolean, String
// ---------------------------------------------------------------------------

fmiStatus fmiSetDebugLogging(fmiComponent c, fmiBoolean loggingOn) {
    return fmiOK;
}

fmiStatus fmiSetReal(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiReal value[]){
    for (unsigned int i = 0; i < nvr; i++) {
        if(valToRefs.at(vr[i]) == "EMSwarmUpComplete"){
            if(value[i] != SimulationConfig::info.timePeriod){
                save = false;
                return fmiOK;
            }
            continue;
        }
    }
    save = true;
    for (unsigned int i = 0; i < nvr; i++) {
        //std::cout << valToRefs.at(vr[i]) << ":" << value[i] <<  " ";
        DataStore::addValue(valToRefs.at(vr[i]), value[i]);
    }
   // std::cout << std::endl;
  //  ModelInstance* comp = (ModelInstance *)c;
    modelInstance->sim.preTimeStep();
    return fmiOK;
}

fmiStatus fmiSetInteger(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiInteger value[]){
    return fmiOK;
}

fmiStatus fmiSetBoolean(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiBoolean value[]){
    return fmiOK;
}

fmiStatus fmiSetString(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiString value[]){
    return fmiOK;
}

fmiStatus fmiGetReal(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiReal value[]) {
    if(save){
        for (unsigned int i = 0; i < nvr; i++) {
            //std::cout <<valToRefs.at(vr[i])<< " " << VariableStore::getValue(valToRefs.at(vr[i]))<<std::endl;
            value[i] = DataStore::getValue(valToRefs.at(vr[i]));
        }
    }
    return fmiOK;
}

fmiStatus fmiGetInteger(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiInteger value[]) {
    return fmiOK;
}

fmiStatus fmiGetBoolean(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiBoolean value[]) {
    return fmiOK;
}

fmiStatus fmiGetString(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiString  value[]) {
    return fmiOK;
}


// ---------------------------------------------------------------------------
// FMI functions: only for FMI Co-Simulation 1.0
// ---------------------------------------------------------------------------

const char* fmiGetTypesPlatform() {
    return fmiPlatform;
}

fmiComponent fmiInstantiateSlave(fmiString  instanceName, fmiString  GUID,
    fmiString  fmuLocation, fmiString  mimeType, fmiReal timeout, fmiBoolean visible,
    fmiBoolean interactive, fmiCallbackFunctions functions, fmiBoolean loggingOn) {
    // ignoring arguments: fmuLocation, mimeType, timeout, visible, interactive
    printf("fmiInstantiateSlave\n");
    return instantiateModel("fmiInstantiateSlave", instanceName, GUID, functions, loggingOn);
}

fmiStatus fmiInitializeSlave(fmiComponent c, fmiReal tStart, fmiBoolean StopTimeDefined, fmiReal tStop) {
   printf("fmiInitializeSlave\n");
    return fmiOK;
}

fmiStatus fmiTerminateSlave(fmiComponent c) {
    printf("fmiTerminateSlave\n");
    return terminate("fmiTerminateSlave", c);
}

fmiStatus fmiResetSlave(fmiComponent c) {
    printf("fmiResetSlave\n");
    return fmiOK;
}

void fmiFreeSlaveInstance(fmiComponent c) {
    if(save){
        //ModelInstance* comp = (ModelInstance *)c;
        modelInstance->sim.postprocess();
    }
    printf("fmiFreeSlaveInstance\n");
}

fmiStatus fmiSetRealInputDerivatives(fmiComponent c, const fmiValueReference vr[], size_t nvr,
    const fmiInteger order[], const fmiReal value[]) {
    printf("fmiSetRealInputDerivatives\n");
    return fmiWarning;
}

fmiStatus fmiGetRealOutputDerivatives(fmiComponent c, const fmiValueReference vr[], size_t  nvr,
    const fmiInteger order[], fmiReal value[]) {
  printf("fmiGetRealOutputDerivatives\n");
    return fmiWarning;
}

fmiStatus fmiCancelStep(fmiComponent c) {
   printf("fmiCancelStep\n");
    return fmiError;
}

/**
 * @brief [brief description]
 * @details [long description]
 *
 * @param c simulation pointer
 * @param currentCommunicationPoint
 * @param communicationStepSize
 * @param newStep
 * @return
 */
fmiStatus fmiDoStep(fmiComponent c, fmiReal currentCommunicationPoint,
    fmiReal communicationStepSize, fmiBoolean newStep) {
    if(save){
        //ModelInstance* comp = (ModelInstance *)c;


        modelInstance->sim.timeStep();

    }
    return fmiOK;
}

static fmiStatus getStatus(const char* fname, fmiComponent c, const fmiStatusKind s) {
    printf("getStatus\n");
    return fmiError;
}

fmiStatus fmiGetStatus(fmiComponent c, const fmiStatusKind s, fmiStatus* value) {
    printf("fmiGetStatus\n");
    return getStatus("fmiGetStatus", c, s);
}

fmiStatus fmiGetRealStatus(fmiComponent c, const fmiStatusKind s, fmiReal* value){
    printf("fmiGetRealStatus\n");
    return getStatus("fmiGetRealStatus", c, s);
}

fmiStatus fmiGetIntegerStatus(fmiComponent c, const fmiStatusKind s, fmiInteger* value){
   printf("fmiGetIntegerStatus\n");
    return getStatus("fmiGetIntegerStatus", c, s);
}

fmiStatus fmiGetBooleanStatus(fmiComponent c, const fmiStatusKind s, fmiBoolean* value){
   printf("fmiGetBooleanStatus\n");
    return getStatus("fmiGetBooleanStatus", c, s);
}

fmiStatus fmiGetStringStatus(fmiComponent c, const fmiStatusKind s, fmiString*  value){
   printf("fmiGetStringStatus\n");
    return getStatus("fmiGetStringStatus", c, s);
}

/**
 * @brief Checks the modelDescription file for parameter names
 * @details Checks the modelDescription file so the agent model know which variables in the array are which
 */
void loadVariables() {

    std::string preferredSlash ="/";
    std::string filename = "tmp-fmus"+preferredSlash+"agentFMU.fmu_FMI"+preferredSlash+"modelDescription.xml";
    std::cout << " Loading XML file: -" << filename << "-" << std::endl;
    // Create an empty property tree object
    boost::property_tree::ptree pt;
  //  std::string temp = "~/Dropbox/AgentConfigurationFiles/simpleOfficeNoBlindsNoWindowsNoLightsNoGains/testGUI.xml";
    // Load the XML file into the property tree. If reading fails
    // (cannot open file, parse error), an exception is thrown.
    boost::property_tree::read_xml(filename, pt);

    // Iterate over the debug.modules section and store all found
    // modules in the m_modules set. The get_child() function
    // returns a reference to the child at the specified path; if
    // there is no such child, it throws. Property tree iterators
    // are models of BidirectionalIterator.

    for(boost::property_tree::ptree::value_type & x: pt.get_child("fmiModelDescription")) {
        if (x.first == "ModelVariables") {
            for(boost::property_tree::ptree::value_type & v: x.second) {
                std::string name = "";
                std::string causality = "";
                int valueReference;

                name = v.second.get_child_optional("<xmlattr>.name")->data();
                causality = v.second.get_child_optional("<xmlattr>.causality")->data();
                valueReference = v.second.get<int>("<xmlattr>.valueReference");

                if(causality == "input"){
                    DataStore::addVariable(name);
                }else{
                    double starValue = 0;
                    for(boost::property_tree::ptree::value_type & y: v.second) {
                        if(y.first == "Real"){
                            starValue = y.second.get<double>("<xmlattr>.start");
                        }
                    }
                    DataStore::addValue(name, starValue);
                }
                valToRefs[valueReference]=name;
            }
        }
    }
    std::cout << " Loaded XML file: -" << filename << "-" << std::endl;
}
 fmiStatus fmiGetModelTypesPlatform(){return fmiOK;}
 fmiStatus fmiFreeModelInstance(){return fmiOK;}
 fmiStatus fmiSetTime(){return fmiOK;}
 fmiStatus fmiSetContinuousStates(){return fmiOK;}
 fmiStatus fmiCompletedIntegratorStep(){return fmiOK;}

 fmiStatus fmiGetDerivatives(){return fmiOK;}
 fmiStatus fmiGetEventIndicators(){return fmiOK;}
 fmiStatus fmiEventUpdate(){return fmiOK;}
 fmiStatus fmiGetContinuousStates(){return fmiOK;}
 fmiStatus fmiGetNominalContinuousStates(){return fmiOK;}
 fmiStatus fmiGetStateValueReferences(){return fmiOK;}
 fmiStatus fmiTerminate(){return fmiOK;}
