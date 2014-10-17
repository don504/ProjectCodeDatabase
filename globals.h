/****************************************************************
		GLOBAL VARIABLE DECLARATIONS
****************************************************************/

CacheEntry	catcache[MAXOPEN];	/* catalog cache */
Buffer		buffer[MAXOPEN];        /* buffer pool */

#pragma pack(1)
typedef struct relationCatalogRec{
	char relName[20];
	uint recLength;  		// uint is unsigned int
	uint recsPerPg;
	uint numAttrs;
	uint numRecs;
	uint numPgs;
}relcatRec;

typedef struct attrcatalogRec{
	uint offset;
	uint length;
	char type[2];
	char attrName[ATTRNAME];
	char relName[RELNAME];
}attrcatRec;
