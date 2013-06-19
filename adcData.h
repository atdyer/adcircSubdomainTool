/** @file */


#ifndef ADCDATA_H
#define ADCDATA_H

#include <stdio.h>


/** QT_DEBUG Macro used for displaying debug output
 *
 * This macro will only print output if compiling/running in debug mode in Qt Creator.
 * It accepts either a single string to print or a formatted string followed by a list of
 * values to print.
 *
 * If you aren't using Qt Creator, you can still print debug output by adding
 * the line
 *
 *    #define QT_DEBUG
 *
 * before this definition.
 *
 */
#ifdef QT_DEBUG
#define DEBUG(format, ...) fprintf(stdout, format, ## __VA_ARGS__)
#else
#define DEBUG(format, ...) do {} while(0)
#endif


#define EARTH_RADIUS 6378206.4


/**
 * @brief Defines an ADCIRC node (See fort.14 in ADCIRC manual)
 *
 * Typically, Nodes will be stored in a std::vector in order from lowest node number to highest.
 * ADCIRC currently requires that node numbers are the ordered set from 1 to n, where n is the total
 * number of Nodes in the dataset, so accessing a specific Node by node number is as simple as
 * accessing node [nodeNumber-1].
 *
 */
typedef struct {
		unsigned int nodeNumber;	/**< The node number as defined in fort.14 file */
		float x;	/**< First location coordinate defined in fort.14 file (typically longitude) */
		float y;	/**< Second location coordinate defined in fort.14 file (typically latitude) */
		float z;	/**< Third location coordinate defined in fort.14 file (depth with respect to GEOID) */
} Node;


/**
 * @brief Defines an ADCIRC element (See fort.14 in ADCIRC manual)
 *
 * Typically, Elements will be stored in a std::vector in order from lowest element number to highest.
 * ADCIRC currently requires that element numbers are the ordered integers from 1 to n, where n is the
 * total number of Elements in the dataset, so accessing a specific Element by element number is as simple
 * as accessing element [elementNumber-1].
 *
 */
typedef struct {
		unsigned int elementNumber;	/**< The element number as defined in fort.14 file */
		unsigned int n1;	/**< nodeNumber of the first Node specified in the fort.14 file */
		unsigned int n2;	/**< nodeNumber of the second Node specified in the fort.14 file */
		unsigned int n3;	/**< nodeNumber of the third Node specified in the fort.14 file */
} Element;


#endif // ADCDATA_H
