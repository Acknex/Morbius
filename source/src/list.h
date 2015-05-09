/*
 *******************************************************************************
 * list.h
 * Creation date: 07.03.2007
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date: 2010-08-05 00:23:10 +0200 (Do, 05 Aug 2010) $
 * $Revision: 4 $
 * $Author: Firo $
 *
 *******************************************************************************
 * Description
 *
 * Definition script for List management
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */

#ifndef LIST_H
#define LIST_H
/*! \file 
 *  Include file for expendable lists.
 */

/* ----- DEFINITIONS ----- */


#define LIST_ACTIVE	/*!< This define can be evaluated to check if this module is included and active */


/*! \name Configuration Data 
 *  The values of these parameters can be overwritten by defining them before including this file.
 * \{ */
#ifndef LIST_ARRAY_STEPS
#define LIST_ARRAY_STEPS 20 /*!< Choosing higher values results in less copying but requires more memory from beginning */
#endif
/* \} */


/* ----- STRUCTURES ----- */


/*! Definition of a list. 
 *  A list is used to store a set of pointers to user defined objects.
 *  The size of the list is adapted automatically at runtime. The size of new memory 
 *  allocated when the list has reached its current limit, can be modified by redefining
 *  LIST_ARRAY_STEPS.
 *  Use the LIST_create function for creation of a new list.
 */
typedef struct
{
	int iCount;		/*!< Number of objects stored in list */
	int iRange;		/*!< Number of memory extensions done (multiplied with LIST_ARRAY_STEPS) */	
	void** ppList;	/*!< Pointer to pointer array for custom objects */

}LIST;


/* ----- EXTERNAL FUNCTIONS ----- */


/*! creates new list.
 *  \return	pointer to newly created LIST
 */
LIST* LIST_create();

/*! Removes a list from memory. 
 *  Caution: Contained items are not affected and remain in memory! 
 *  \param	psHost  Pointer to host LIST 
 */
void LIST_remove(LIST* psHost);

/*! Removes a list from memory. 
 *  Contained items are deleted  - use with care!! 
 *	 DO NOT USE with stored engine objects!! 
 *	 Take care when storing structs containing pointers to allocated memory!! 
 *  \param	psHost  Pointer to host LIST 
 */
void LIST_removeAll(LIST* psHost);

/*! Remove item from LIST by index. 
 *  Memory occupied by item is NOT freed! 
 *  Indices of follow-up items are NOT affected 
 *  \param	psHost  Pointer to host LIST 
 *  \param	iIndex List index for stored item 
 */
void LIST_removeItem(LIST* psHost, int iIndex);

/*! Append new item to LIST 
 *  \param	psHost    Pointer to host LIST 
 *  \param	psNewItem Pointer to item to be added (casted to void*) 
 */
void LIST_append(LIST* psHost, void* psNewItem);

/*! Get item from list by index.
 *  \param	psHost  Pointer to host LIST 
 *  \param	iIndex List index for stored item 
 *  \return	Pointer to item or NULL 
 */
void* LIST_getItem(LIST* psHost, int iIndex);

/*! Get amount of stored items.
 *  \param	psHost Pointer to host LIST 
 *  \return	Amount of stored items 
 */
int LIST_items(LIST* psHost);


/* ----- INTERNAL FUNCTIONS - DO NOT USE ----- */


/*! \internal - Do not use! */
void LIST__extend(LIST* psHost);
/*! \internal - Do not use! */
void LIST__init(LIST* psHost);

#endif