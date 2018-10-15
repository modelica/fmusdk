/* ---------------------------------------------------------------------------*
 * Sample implementation of an FMU - increments an int counter every second.
 * Copyright QTronic GmbH. All rights reserved.
 * ---------------------------------------------------------------------------*/

#include <string.h>  // for size_t

// define class name and unique id
#define MODEL_IDENTIFIER inc
#define MODEL_GUID "{8c4e810f-3df3-4a00-8276-176fa3c9f008}"

// define model size
#define NUMBER_OF_STATES 0
#define NUMBER_OF_EVENT_INDICATORS 0

#define N_VARIABLES 1
char   s_variableTypes[N_VARIABLES] = "i";
size_t s_variableSizes[N_VARIABLES] = { 1 };

// include fmu header files, typedefs and macros
#include "fmu3Template.h"

// define all model variables and their value references
// conventions used here:
// - if x is a variable, then macro x_ is its variable reference
// - the vr of a variable is its index in array  r, i, b or s
// - if k is the vr of a real state, then k+1 is the vr of its derivative
#define counter_ 0

// called by fmi2Instantiate
// Set values for all variables that define a start value
// Settings used unless changed by fmi2SetX before fmi2EnterInitializationMode
void setStartValues(ModelInstance *comp) {
    i(counter_) = 1;
}

// called by fmi2GetReal, fmi2GetInteger, fmi2GetBoolean, fmi2GetString, fmi2ExitInitialization
// if setStartValues or environment set new values through fmi2SetXXX.
// Lazy set values for all variable that are computed from other variables.
void calculateValues(ModelInstance *comp) {
    if (comp->state == modelInitializationMode) {
        // set first time event
        comp->eventInfo.nextEventTimeDefined   = fmi3True;
        comp->eventInfo.nextEventTime          = 1 + comp->time;
    }
}

// used to set the next time event, if any.
void eventUpdate(ModelInstance *comp, fmi3EventInfo *eventInfo, int timeEvent, int isNewEventIteration) {
    if (timeEvent) {
        i(counter_) += 1;
        if (i(counter_) == 13) {
            eventInfo->terminateSimulation  = fmi3True;
            eventInfo->nextEventTimeDefined = fmi3False;
        } else {
            eventInfo->nextEventTimeDefined   = fmi3True;
            eventInfo->nextEventTime          = 1 + comp->time;
        }
    }
}

// include code that implements the FMI based on the above definitions
#include "fmu3Template.c"