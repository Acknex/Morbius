/*
 *******************************************************************************
 * xmlreader.h
 * Creation date: 01.11.2007
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
 * Definition script for XML parser
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */

#ifndef XMLREADER_H
#define XMLREADER_H
/*! \file 
 *  Include file for XML file parser.
 */

/* ----- INCLUDES ----- */


#include "list.h"

#include "list.c"


/* ----- DEFINITIONS ----- */


#define XMLREADER_ACTIVE	/*!< This define can be evaluated to check if this module is included and active */

/*! \name Constant Data 
 *  These constants are used as identifiers in certain functions.
 * \{ */
#define XML_NOTAG -1
#define XML_CONTENT 0
#define XML_OPENTAG 1
#define XML_CLOSETAG 2
#define XML_EOF 255

#define EOF 255
/* \} */


/* ----- STRUCTURES ----- */


/*! Definition of a XML parameter. 
 *  Each parameter (tag) found in a XML file is represented by this structure.
 *  Use the XMLPAR_create function for creation of a new XML parameter.
 */
typedef struct xmlpar
{
	STRING* strTag;		/*!< String pointer to tag name */
	STRING* strContent;	/*!< String pointer to tag content */
	LIST* psTagList;		/*!< Pointer to a list of sub tags */
	LIST* psAttribList;	/*!< Pointer to a list of tag attributes */
	struct xmlpar* psParent;
}XMLPAR;

/*! Definition of a XML file. 
 *  For reading data from an XML file, this object is required.
 *  Use the XMLFILE_create function for creation of a new XML file.
 */
typedef struct
{
	var vFHndl;			/*!< File handle (Acknex format) */
	STRING* vFName;	/*!< String pointer to xml filename */
	XMLPAR* psAnchor;	/*!< Pointer to root XML parameter ("XML_root") */
		
	/* temp variables */
	char cTmp[2];	/*!< For temporary use */
	int iTmp;		/*!< For temporary use */
}XMLFILE;

/*! Definition of a XML attribute. 
 *  Each attribute  of a XML tag is represented by this structure.
 *  Use the XMLATTRIB_create function for creation of a new XML attribute.
 */
typedef struct
{
	STRING* strTag;		/*!< String pointer to attribute name */
	STRING* strContent;	/*!< String pointer to attribute content */
	XMLPAR* psParent;		/*!< Pointer to parent XML parameter (tag) */
}XMLATTRIB;


/* ----- EXTERNAL FUNCTIONS ----- */


/*! Open new xml file.
 *  \param	pcFile Pointer: full name of xml file
 *  \return	Pointer to XMLFILE
 */
XMLFILE* XMLFILE_create(char* pcFile);

/*! Removes XMLFILE from memory. Delete all allocated content.
 *  \param	psHost Pointer to XMLFILE to be deleted
 */
void XMLFILE_remove(XMLFILE* psHost);

/*! Read XML file into memory
 *  \param	psHost Pointer to XMLFILE to be parsed
 *  \return	Pointer to root element of XML list
 */
XMLPAR* XMLFILE_parse(XMLFILE* psHost);


/*! Creates new XMLPAR
 *  \return	Pointer to XMLPAR
 */
XMLPAR* XMLPAR_create();

/*! Removes XMLPAR from memory. Delete all allocated content.
 *  \param	psHost Pointer to XMLPAR to be deleted
 */
void XMLPAR_remove(XMLPAR* psHost);

/*! Copy tag content to specified string.
 *  \param	psHost Pointer to XMLPAR to be accessed
 *  \param	strTarget String pointer to string to be filled
 */
void XMLPAR_getContent(XMLPAR* psHost, STRING* strTarget);

/*! Copy tag name to specified string.
 *  \param	psHost Pointer to XMLPAR to be accessed
 *  \param	strTarget String pointer to string to be filled
 */
void XMLPAR_getTag(XMLPAR* psHost, STRING* strTarget);

/*! Retrieve pointer to tag content.
 *  \param	psHost Pointer to XMLPAR to be accessed
 *  \return	String pointer to tag name/tag content
 */
STRING* XMLPAR_getPContent(XMLPAR* psHost);

/*! Retrieve pointer to tag name.
 *  \param	psHost Pointer to XMLPAR to be accessed
 *  \return	String pointer to tag name/tag content
 */
STRING* XMLPAR_getPTag(XMLPAR* psHost);

/*! Find specified tag inside parent XML parameter.	
 *  \param	psHost Pointer to parent XMLPAR
 *  \param	strTag String pointer: name of xml tag
 *  \return	Pointer to found XMLPAR or NULL
 */
XMLPAR* XMLPAR_getElementByTag(XMLPAR* psHost, STRING* strTag);

/*! Find specified tag inside parent XML parameter.	
 *  \param	psHost Pointer to parent XMLPAR
 *  \param	iIndex Index of xml tag
 *  \return	Pointer to found XMLPAR or NULL
 */
XMLPAR* XMLPAR_getElementByIndex(XMLPAR* psHost, int iIndex);

/*! Retrieve number of sub tags of XML parameter.	
 *  \param	psHost Pointer to XMLPAR to be accessed
 *  \return	Number of sub tags
 */
int XMLPAR_getTagElements(XMLPAR* psHost);

/*! Retrieve number of attributes of XML parameter.
 *  \param	psHost Pointer to XMLPAR to be accessed
 *  \return	Number of attributes
 */
int XMLPAR_getAttributeElements(XMLPAR* psHost);

/*! Creates new XML attribute.
 *  \return	Pointer to XMLATTRIB
 */
XMLATTRIB* XMLATTRIB_create();

/*! Removes XML attribute from memory. Delete all allocated content.
 *  \param	psHost Pointer to XMLATTRIB to be deleted
 */
void XMLATTRIB_remove(XMLATTRIB* psHost);

/*! Copy tag content to specified string.
 *  \param	psHost XMLATTRIB to be accessed
 *  \param	strTarget String pointer to string to be filled
 */
void XMLATTRIB_getContent(XMLATTRIB* psHost, STRING* strTarget);

/*! Copy tag name to specified string.
 *  \param	psHost XMLATTRIB to be accessed
 *  \param	strTarget String pointer to string to be filled
 */
void XMLATTRIB_getAttribute(XMLATTRIB* psHost, STRING* strTarget);

/*! Retrieve pointer to attribute content.
 *  \param	psHost Pointer to XMLATTRIB to be accessed
 *  \return	String pointer to tag name/tag content
 */
STRING* XMLATTRIB_getPContent(XMLATTRIB* psHost);

/*! Retrieve pointer to attribute name.
 *  \param	psHost Pointer to XMLATTRIB to be accessed
 *  \return	String pointer to tag name/tag content
 */
STRING* XMLATTRIB_getPAttribute(XMLATTRIB* psHost);

/*! Find specified attribute inside parent XML parameter.	
 *  \param	psHost Pointer to parent XMLPAR
 *  \param	strTag String pointer: name of xml attribute
 *  \return	Pointer to found XMLATTRIB or NULL
 */
XMLATTRIB* XMLATTRIB_getElementByAttribute(XMLPAR* psHost, STRING* strTag);

/*! Find specified attribute inside parent XML parameter.
 *  \param	psHost Pointer to parent XMLPAR
 *  \param	iIndex Index of xml attribute
 *  \return	Pointer to found XMLATTRIB or NULL
 */
XMLATTRIB* XMLATTRIB_getElementByIndex(XMLPAR* psHost, int iIndex);


/* ----- INTERNAL FUNCTIONS - DO NOT USE ----- */

/*! \internal - Do not use! */
int XMLFILE__read(XMLFILE* psHost, XMLPAR* parentPar, int ntag);
/*! \internal - Do not use! */
void XMLFILE__readTagAttrib(XMLFILE* psHost, XMLPAR* par);
/*! \internal - Do not use! */
int XMLFILE__readOpenTag(XMLFILE* psHost, XMLPAR* par);
/*! \internal - Do not use! */
void XMLFILE__readCloseTag(XMLFILE* psHost);
/*! \internal - Do not use! */
void XMLFILE__readContent(XMLFILE* psHost, XMLPAR* par);
/*! \internal - Do not use! */
int XMLFILE__findNext(XMLFILE* psHost);

//Added
int XMLPAR_compareAttributes(LIST* attr1, LIST* attr2);
int XMLPAR_getIndexByElement(XMLPAR* psHost, XMLPAR* element);


#endif