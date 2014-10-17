    /* 
	This is the basic definition file.
    */

/*************************************************************
		HEADER FILES
*************************************************************/
#include<stdio.h>
/*************************************************************
		CONSTANTS
*************************************************************/
#define PAGESIZE	512	/* number of bytes in a page */
#define	MAXRECORD	508	/* PAGESIZE minus number of bytes taken up
                                   for slot map */

#define RELNAME		20	/* max length of a relation name */
#define MAXOPEN		20  	/* max number of files that can be open
                                   at the same time */
#define ATTRNAME	20 	/*max length of a attribute name*/

#define	OK		0	/* return codes */
#define NOTOK		-1

#define RELCAT		"relcat"   /* name of the relation catalog file */
#define ATTRCAT		"attrcat"  /* name of the attribute catalog file */

#define Handle FILE  /* Handle to a relation file */

#define TRUE 1
#define FALSE 0
#define uint unsigned int
/*************************************************************
		TYPE DEFINITIONS 
*************************************************************/

/* Rid structure */
typedef struct recid {
	short	pid;
	short	slotnum;
} Rid;

/* Available slots to Insert new record other than at the End */

typedef struct FreeSlotsOfNodes{
	Rid rid;
	struct AvailableSlotsOfNodes *next;
}freeSlots;

/* Page Structure */
typedef struct ps {
	unsigned slotmap;
	char contents [MAXRECORD];
	} Page;

typedef struct attrCatalogNode{
	unsigned int offset;
	unsigned int length;
	char type;
	char attrName[ATTRNAME];
	char relName[RELNAME];
	struct attrCatalogNode *next;
}attrNode;

typedef struct CacheEntry{
	char relName[RELNAME];
	unsigned int recLength;
	unsigned int recsPerPg;
	unsigned int numAttrs;
	unsigned int numRecs;
	unsigned int numPgs;
	Rid relcatRid;
	Handle *relFile;   	/* Handle to the file corresponding to the relation */
	short dirty;
	attrNode* attrList;
	//freeSlots *slotId;
}CacheEntry;

typedef struct Buffer{
	short pid;
	short dirty;
	Page page;
}Buffer;

/*****************************************************************/
