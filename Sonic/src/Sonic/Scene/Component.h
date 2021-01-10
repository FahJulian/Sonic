#pragma once

/**
* Adds a static method that returs the unique ID of the component struct as well as
* a static methods that returns the expected amount of components that will be added
* 
* @param expectedMagnitude The log() of the expected amount of components of this
*                          type that will be added. (E.g. expected Magnitude = 4
*                          -> 10^4 (10000) are expected to be added. It should be
*                          rather to low than to high -> If you expect that there 
*                          will be 500 components, use 2 (100), not 3 (1000)
*/
#define SONIC_REGISTER_COMPONENT(expectedMagnitude) \
	/* \
	* @return The unique ID of this component struct \
	*/ \
    static Sonic::ComponentType getComponentType() \
    { \
        static const Sonic::ComponentType type = Sonic::getNextComponentType(); \
        return type; \
    } \
    /* \
	* @return The log() of the expected amount of components of this type that will be \
    * added during the game \
	*/ \
    static int getExpectedMagnitude() \
    { \
        return expectedMagnitude; \
    }

namespace Sonic {

    /**
    * Unique ID for each component struct
    */
    using ComponentType = unsigned short;

    /**
    * @return The component type the next registered component struct gets assigned
    */
    ComponentType getNextComponentType();

}   
