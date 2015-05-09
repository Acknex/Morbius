/*
 *******************************************************************************
 * xmlreader.c
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
 * Script for XML parser
 *
 * Comments
 * 
 * for short descriptions see comments in xmlreader.h
 *
 *******************************************************************************
 */

/* ----- INCLUDES ----- */


//#include <acknex.h>


/* ----- EXTERNAL FUNCTIONS ----- */


/* ----- XMLFILE ----- */
XMLFILE* XMLFILE_create(char* pcFile)
{
	XMLFILE* psHost;
	psHost = (XMLFILE*)malloc(sizeof(XMLFILE));
	psHost->vFName = str_create(pcFile);
	psHost->vFHndl = NULL;
	psHost->psAnchor = NULL;
		
	return (psHost);
}

void XMLFILE_remove(XMLFILE* psHost)
{
	int i;

	/* remove all allocated instances */
	str_remove(psHost->vFName);
	if (psHost->psAnchor != NULL)
	{
		XMLPAR_remove(psHost->psAnchor);	
	}
	free (psHost);
}

XMLPAR* XMLFILE_parse(XMLFILE* psHost)
{
	psHost->vFHndl = file_open_game(psHost->vFName);
	/* only read file if filename is valid and psAnchor is not yet allocated */
	if ((psHost->vFHndl != NULL) && (psHost->psAnchor == NULL))
	{
		/* create psAnchor XMLPAR */
		psHost->psAnchor = XMLPAR_create();
		psHost->psAnchor->strTag = str_create("XML_root");
		psHost->iTmp = XMLFILE__findNext(psHost);
		do
		{
			psHost->iTmp = XMLFILE__read(psHost, psHost->psAnchor, psHost->iTmp);
		}while (psHost->iTmp == XML_OPENTAG);

		file_close(psHost->vFHndl);
		psHost->vFHndl = NULL;		
	}
	return (psHost->psAnchor);
}


/* ----- XMLPAR ----- */
XMLPAR* XMLPAR_create()
{
	XMLPAR* psHost;
	psHost = (XMLPAR*)malloc(sizeof(XMLPAR));
	psHost->psTagList = NULL;
	psHost->psAttribList = NULL;
	psHost->strTag = NULL;
	psHost->strContent = NULL;
	psHost->psParent = NULL; 
	
	return (psHost);
}

void XMLPAR_remove(XMLPAR* psHost)
{
	int i;
	str_remove(psHost->strTag);
	/* single par - remove content */
	if (psHost->strContent != NULL)
		str_remove(psHost->strContent);

	/* remove all XMLPARs on list */
	if (psHost->psTagList != NULL)
	{
		for (i = 0; i < LIST_items(psHost->psTagList); i++)
			XMLPAR_remove((XMLPAR*)LIST_getItem(psHost->psTagList, i));
		LIST_remove(psHost->psTagList);	
	}
	
	/* remove Attribute list */
	if (psHost->psAttribList !=NULL)
	{
		for (i = 0; i < LIST_items(psHost->psAttribList); i++)
			XMLATTRIB_remove((XMLATTRIB*)LIST_getItem(psHost->psAttribList, i));
		LIST_remove(psHost->psAttribList);	
	}

	free(psHost);
}

void XMLPAR_getContent(XMLPAR* psHost, STRING* strTarget)
{
	if (psHost->strContent == NULL)
		str_cpy(strTarget, "");
	else
		str_cpy(strTarget, psHost->strContent);
}

void XMLPAR_getTag(XMLPAR* psHost, STRING* strTarget)
{
	if (psHost->strTag == NULL)
		str_cpy(strTarget, "");
	else
		str_cpy(strTarget, psHost->strTag);
}

STRING* XMLPAR_getPContent(XMLPAR* psHost)
{
	if (psHost->strContent != NULL)
		return (psHost->strContent);
	else
		return (NULL);
}

STRING* XMLPAR_getPTag(XMLPAR* psHost)
{
	if (psHost->strTag != NULL)
		return (psHost->strTag);
	else
		return (NULL);
}

XMLPAR* XMLPAR_getElementByTag(XMLPAR* psHost, STRING* strTag)
{
	int i;
	XMLPAR* pXml;
	
	pXml = NULL;
	
	if (psHost->strTag != NULL)
	{
		/* at least one sub parameter found */
		if ((psHost->psTagList != NULL) && (psHost->strContent == NULL))
		{
			for (i = 0; i < LIST_items(psHost->psTagList); i++)
			{
				pXml = (XMLPAR*)LIST_getItem(psHost->psTagList, i);
				if (!str_cmp(pXml->strTag, strTag))
					/* tag not found */
					pXml = NULL;
				else
					/* tag found */
					break;
			}
		}
	}
			
	return (pXml);	
}

XMLPAR* XMLPAR_getElementByIndex(XMLPAR* psHost, int iIndex)
{
	XMLPAR* pXml;
	
	pXml = NULL;
	
	/* at least one sub parameter found */
	if (psHost->psTagList != NULL)	
	{
		/* limit index to valid range */
		//iIndex = clamp(iIndex, 0, LIST_items(psHost->psTagList) - 1);
		if (iIndex >= LIST_items(psHost->psTagList)) {
			return (NULL);
		}
		pXml = (XMLPAR*)LIST_getItem(psHost->psTagList, iIndex);
	}
	return (pXml);
}

// Added
int XMLPAR_getIndexByElement(XMLPAR* psHost, XMLPAR* element)
{
	int i;
	if (psHost->psTagList != NULL)
	{
		for (i = 0; i < LIST_items(psHost->psTagList); i++)
		{
			XMLPAR* pXml = (XMLPAR*)LIST_getItem(psHost->psTagList,i);
			
			if ((str_cmp(pXml->strTag,element->strTag) == 1) &&
				 (str_cmp(pXml->strContent,element->strContent) == 1) &&
				 (XMLPAR_compareAttributes(pXml->psAttribList, element->psAttribList) == 1))
			{
			 	return i;
			}	
		}
		return -1;
	}
}

int XMLPAR_compareAttributes(LIST* attr1, LIST* attr2)
{
	if (LIST_items(attr1) != LIST_items(attr2)) return 0;
	
	int i;
	XMLPAR* pXml1 = NULL;
	XMLPAR* pXml2 = NULL;
	
	for (i = 0; i < LIST_items(attr1); i++)
	{
		pXml1 = (XMLATTRIB*)LIST_getItem(attr1, i);
		pXml2 = (XMLATTRIB*)LIST_getItem(attr2, i);
			
		if (!str_cmp(pXml1->strTag, pXml2->strTag))
			return 0;
	}
	return 1;
}

int XMLPAR_getTagElements(XMLPAR* psHost)
{
	if (psHost->psTagList != NULL)
		return (LIST_items(psHost->psTagList));
	else
		return (0);	
}

int XMLPAR_getAttributeElements(XMLPAR* psHost)
{
	if (psHost->psAttribList != NULL)
		return (LIST_items(psHost->psAttribList));	
	else
		return (0);	
}


/* ----- XMLATTRIB ----- */
XMLATTRIB* XMLATTRIB_create()
{
	XMLATTRIB* psHost;
	psHost = (XMLATTRIB*)malloc(sizeof(XMLATTRIB));
	psHost->strTag = NULL;
	psHost->strContent = NULL;
	
	return (psHost);
}

void XMLATTRIB_remove(XMLATTRIB* psHost)
{
	str_remove(psHost->strTag);
	str_remove(psHost->strContent);

	free(psHost);
}

XMLATTRIB* XMLATTRIB_getElementByAttribute(XMLPAR* psHost, STRING* strTag)
{
	int i;
	XMLATTRIB* pXml;
	
	pXml = NULL;
	
	/* at least one sub parameter found */
	if (psHost->psAttribList != NULL)
	{
		for (i = 0; i < LIST_items(psHost->psAttribList); i++)
		{
			pXml = (XMLATTRIB*)LIST_getItem(psHost->psAttribList, i);
			if (!str_cmp(pXml->strTag, strTag))
				/* tag not found */
				pXml = NULL;
			else
				/* tag found */
				break;
		}
	}
			
	return (pXml);	
}

XMLATTRIB* XMLATTRIB_getElementByIndex(XMLPAR* psHost, int iIndex)
{
	XMLATTRIB* pXml;
	
	pXml = NULL;
	
	/* at least one sub parameter found */
	if (psHost->psAttribList != NULL)	
	{
		/* limit index to valid range */
		iIndex = clamp(iIndex, 0, LIST_items(psHost->psAttribList) - 1);
		pXml = (XMLATTRIB*)LIST_getItem(psHost->psAttribList, iIndex);
	}
	return (pXml);
}

void XMLATTRIB_getContent(XMLATTRIB* psHost, STRING* strTarget)
{
	if (psHost->strContent == NULL)
		str_cpy(strTarget, "");
	else
		str_cpy(strTarget, psHost->strContent);
}

void XMLATTRIB_getAttribute(XMLATTRIB* psHost, STRING* strTarget)
{
	if (psHost->strTag == NULL)
		str_cpy(strTarget, "");
	else
		str_cpy(strTarget, psHost->strTag);
}

STRING* XMLATTRIB_getPContent(XMLATTRIB* psHost)
{
	if (psHost->strContent != NULL)
		return (psHost->strContent);
	else
		return (NULL);
}

STRING* XMLATTRIB_getPAttribute(XMLATTRIB* psHost)
{
	if (psHost->strTag != NULL)
		return (psHost->strTag);
	else
		return (NULL);
}


/* ----- INTERNAL FUNCTIONS ----- */


/* ----- XMLFILE ----- */
int XMLFILE__read(XMLFILE* psHost, XMLPAR* parentPar, int ntag)
{
	/* create new XMLPAR and add it to list of parent */
	XMLPAR* par; 
	par = XMLPAR_create();
	par->psParent = parentPar;
	if (parentPar->psTagList == NULL)
		parentPar->psTagList = LIST_create();
	LIST_append(parentPar->psTagList, (void*)par);

	if(ntag == XML_OPENTAG)
	{
		if (XMLFILE__readOpenTag(psHost, par))
		{
			/* no content was found */
			par->strContent = str_create("");
			par->psTagList = NULL; 
			ntag = XMLFILE__findNext(psHost);
		}
		else
		{
			ntag = XMLFILE__findNext(psHost);
			switch(ntag)
			{
				/* NOTE: readContent and read should be changed so text and tag mixtures can be read as well */
				case XML_CONTENT:
					XMLFILE__readContent(psHost, par);
					ntag = XMLFILE__findNext(psHost);
					break;
							
				case XML_OPENTAG:
					do
					{
						ntag = XMLFILE__read(psHost, par, ntag);
					}while(ntag == XML_OPENTAG);
					break;
							
				case XML_CLOSETAG:
					/* no content was found */
					par->strContent = str_create("");
					par->psTagList = NULL; 
					break;
							
				default:
					/* invalid file */
					ntag = XML_EOF;
					break;	
			}

			if (ntag == XML_CLOSETAG)
			{
				XMLFILE__readCloseTag(psHost);
				ntag = XMLFILE__findNext(psHost);
			}
			else
				/* invalid file */
				ntag = XML_EOF;
		}

	}
	else
		/* invalid file */
		ntag = XML_EOF;
			
	return (ntag);
}
	
void XMLFILE__readTagAttrib(XMLFILE* psHost, XMLPAR* par)
{
	/* create new XMLATTRIB and add it to list of parent */
	XMLATTRIB *attrib;

	attrib = XMLATTRIB_create();
	attrib->psParent = par;
	attrib->strTag = str_create("");
	attrib->strContent = str_create("");
	if (par->psAttribList == NULL)
		par->psAttribList = LIST_create();
	LIST_append(par->psAttribList, (void*)attrib);
	psHost->cTmp[1] = '\0';
	
	/* read attribute tag */
	do
	{
		psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
		if ((psHost->cTmp[0] > 32) && (psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '='))
		{
			str_cat(attrib->strTag, psHost->cTmp);
		}
	}
	while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != 32) && (psHost->cTmp[0] != '=') 
		&& (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>'));

	/* dummy read unnecessary bytes like spaces, " and = */ 
	if ((psHost->cTmp[0] == '=') || (psHost->cTmp[0] == 32))
	{
		do
		{
			psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
		}
		while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != 34) && (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>'));
	}

	/* read attribute strContent */
	if (psHost->cTmp[0] == 34)	/* 34 = " */
	{
		do
		{
			psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
			if ((psHost->cTmp[0] >= 32) && (psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != 34))
			{
				str_cat(attrib->strContent, psHost->cTmp);
			}
		}
		while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != 34) && (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>'));
	}
//	printf("attribute: [%s], content: [%s]", (attrib->strTag)->chars, (attrib->strContent)->chars);	
}

int XMLFILE__readOpenTag(XMLFILE* psHost, XMLPAR* par)
{
	par->strTag = str_create("");
	psHost->cTmp[1] = '\0';
	do
	{
		psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
		/* check for valid byte and add it to tag string */
		
 		if (psHost->cTmp[0] == 32 || psHost->cTmp[0] == '\n') 
		{
			/* possible attribute tag detected */
			do
			{
				/* dummy read unnecessary bytes */
				do
				{
					psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
				}
				while((psHost->cTmp[0] != EOF) && psHost->cTmp[0] <= 32);
	
				/* check whether next character is attribute name or tag element */
				if((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>'))
				{
					file_seek(psHost->vFHndl, -1, 1);
	 				XMLFILE__readTagAttrib(psHost, par);
	 			}
 			}while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>'));
		}
 		else
 		{
			if ((psHost->cTmp[0] >= 32) && (psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>'))
			{
				str_cat(par->strTag, psHost->cTmp);
			}
		}
	}while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '/') && (psHost->cTmp[0] != '>')); 
//printf ("tagname: [%s]", (par->strTag)->chars);			

	switch ((int)psHost->cTmp[0])
	{
		case '/':
			do
			{
				psHost->cTmp[0]  = file_asc_read(psHost->vFHndl);
			}while((psHost->cTmp[0]  != '>') && (psHost->cTmp[0]  != EOF)); 
		
			psHost->iTmp = 1;	/* tag is empty */
			break;
			
		case EOF:
			psHost->iTmp = 1;	/* tag is empty */
			break;
		
		default:
			psHost->iTmp = 0;	/* tag has content */ 			
			break;
	}
			
	return (psHost->iTmp);
}

void XMLFILE__readCloseTag(XMLFILE* psHost)
{
	/* just read in the closetag - no further action */
	do
	{
		psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
	}while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '>')); 
}

void XMLFILE__readContent(XMLFILE* psHost, XMLPAR* par)
{
	par->strContent = str_create("");
	psHost->cTmp[1] = '\0';
	
	do
	{
		psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
		/* check for valid byte and add it to tag string */
		if ((psHost->cTmp[0] >= 32) && (psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '<'))
		{
			str_cat(par->strContent, psHost->cTmp);
		}			
	}while((psHost->cTmp[0] != EOF) && (psHost->cTmp[0] != '<')); 
	str_cat(par->strContent, "\0");		
	file_seek(psHost->vFHndl, -1, 1);
	
}

int XMLFILE__findNext(XMLFILE* psHost)
{
	psHost->iTmp = XML_NOTAG;
	
	while(psHost->iTmp == XML_NOTAG)
	{
		/* search first useful sign */
		do
		{
			psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
		}while((psHost->cTmp[0] < 32) && (psHost->cTmp[0] != EOF)); 
		
		/* evaluate finding*/
		if (psHost->cTmp[0] == '<')
		{
			/* skip control bytes */
			do
			{
				psHost->cTmp[0] = file_asc_read(psHost->vFHndl);
			}while((psHost->cTmp[0] < 32) && (psHost->cTmp[0] != EOF));
			switch ((int)psHost->cTmp[0])
			{
				case '/':	/* closing tag */
					psHost->iTmp = XML_CLOSETAG;
					break;
					
				case '!':
				case '?':	/* comment tag - skip and continue searching */
					XMLFILE__readCloseTag(psHost);	//ugly
					psHost->iTmp = XML_NOTAG;
					break;
					
				case EOF:	/* end of file */
					psHost->iTmp = XML_EOF;
					break;
					
				default:		/* opening tag */
					psHost->iTmp = XML_OPENTAG;	
					/* set file pointer to previous byte */
					file_seek(psHost->vFHndl, -1, 1);
					break;
			}
		}
		else
		{
			/* end of file check */
			if (psHost->iTmp != EOF)
			{
				/* tag content found */
				psHost->iTmp = XML_CONTENT;
				file_seek(psHost->vFHndl, -1, 1);
			}
			else
				psHost->iTmp = XML_EOF;
		}
	}
	
	return (psHost->iTmp);
}