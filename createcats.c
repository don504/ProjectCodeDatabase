#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"



relcatRec* fillRelationRecord(const char* rName,uint rLength,uint recsPage,uint nAttrs,uint nRecs,uint nPgs){
	relcatRec * rec = malloc(sizeof(relcatRec));
	strcpy(rec->relName,rName);
	rec->recLength = rLength;		// uint is unsigned int
	rec->recsPerPg = recsPage;
	rec-> numAttrs = nAttrs;
	rec -> numRecs = nRecs;
	rec -> numPgs = nPgs;
return rec;
}

attrcatRec* fillAttributeRecord (uint oset,uint len,const char* type,const char *aName,const char *rName){
	attrcatRec * arec = malloc(sizeof(attrcatRec));
	arec->offset = oset;
	arec->length = len;
	strcpy(arec->type,type);
	strcpy(arec->attrName,aName);
	strcpy(arec->relName,rName);
	return arec;
}

int CreateCats(char* dbPath)
{
 /* Creating Relation Catalog as the first record in the Catalog Table */

	Handle *h_rel,*h_attr;
	int index = 0;
	char relPath[256],attrPath[256]; /* ext4 system has a maximum file path length of 255 Bytes */
	mkdir(dbPath,00777);  //should be moved to the createDb function and should be checked for existence before creation
	strcpy(relPath,dbPath);
	strcat(relPath,"/RELCAT");
	strcpy(attrPath,dbPath);	//required.. Just for Testing i have commented
	strcat(attrPath,"/ATTRCAT");

	h_rel = fopen(relPath, "w");
	if(!h_rel)
		return (ErrorMsgs(RELATIONCATCREATEFAILED,TRUE,dbPath));
	h_attr = fopen(attrPath, "w");
		if(!h_rel)
			return (ErrorMsgs(ATTRCATCREATEFAILED,TRUE,dbPath));

		/* Filling up Data */
	uint sizeOfRelRec = sizeof(relcatRec);		//40 bytes
	printf("xxx %d",sizeOfRelRec);
	int recsPerPage = floor(MAXRECORD/sizeOfRelRec); /* Error Can Come ! Correct  ?!*/
	relcatRec *relrec = fillRelationRecord("RELCAT",sizeOfRelRec,recsPerPage,6,2,1);

	unsigned int sizeOfAttrRec = sizeof(attrcatRec); //50 Bytes .. Look out for the alignment problem!
	printf("yyy %d",sizeOfAttrRec);
	int attrRecsPerPage = floor(MAXRECORD/sizeOfAttrRec); // 10
	relcatRec *attrrec = fillRelationRecord("ATTRCAT",sizeOfAttrRec,attrRecsPerPage,5,11,2);

	size_t slotMapSize = 4; // have to be decided (recsPerPage/8.0);
	Page page1,page2;
	uint slotmap = 3;
	memset(&page1.slotmap,0,slotMapSize);
	memset(&page1.contents[0],0,MAXRECORD);
	memcpy(&page1.slotmap,&slotmap,slotMapSize);	//*(&page1.slotmap) = slotmap;
	memcpy(&page1.contents[0],relrec,sizeOfRelRec);
	memcpy(&page1.contents[0]+sizeOfRelRec,attrrec,sizeOfRelRec);
	int amount = fwrite(&page1,PAGESIZE,1,h_rel);

	/* Writing attribute records */

	Page page[2];
	memset(&page[0].slotmap,0,slotMapSize);
	memset(&page[0].contents[0],0,MAXRECORD);
	slotmap = 10;
	attrcatRec *arecArray[11] ; // = malloc(11*sizeof(attrcatRec));

	arecArray[0] = fillAttributeRecord(0,20,"s","relName","RELCAT");
	arecArray[1] = fillAttributeRecord(20,4,"i","recLength","RELCAT");
	arecArray[2] = fillAttributeRecord(24,4,"i","recsPerPg","RELCAT");
	arecArray[3] = fillAttributeRecord(28,4,"i","numAttrs","RELCAT");
	arecArray[4] = fillAttributeRecord(32,4,"i","numRecs","RELCAT");
	arecArray[5] = fillAttributeRecord(36,4,"i","numPgs","RELCAT");
	arecArray[6] = fillAttributeRecord(0,sizeof(uint),"i","offset","ATTRCAT");
	arecArray[7] = fillAttributeRecord(4,sizeof(uint),"i","length","ATTRCAT");
	arecArray[8] = fillAttributeRecord(8,2,"s","type","ATTRCAT");
	arecArray[9] = fillAttributeRecord(10,20,"s","attrName","ATTRCAT");
	arecArray[10] =fillAttributeRecord(30,20,"s","relName","ATTRCAT");

	memcpy(&page[0].slotmap,&slotmap,slotMapSize);
	for(index=0;index<attrRecsPerPage;index++){
		memcpy(&page[0].contents[0]+(index*sizeOfAttrRec),arecArray[index],sizeOfAttrRec);
		free(arecArray[index]);
	}
	slotmap = 1;
	memset(&page[1].slotmap,0,slotMapSize);
	memset(&page[1].contents[0],0,MAXRECORD);
	memcpy(&page[1].slotmap,&slotmap,slotMapSize);
	memcpy(&page[1].contents[0],arecArray[10],sizeOfAttrRec);
	free(arecArray[10]);

	//int eff = fwrite(&page1,PAGESIZE,1,h_attr);
	//printf("\n sss = %d\n",eff);
	//fseek(h_attr,0,SEEK_END	);
	fwrite(&page,PAGESIZE,2,h_attr);

	//Test Code  - Do not Delete NOW .Can be used for testing

	/* UNCOMMENT TO SEE IF IT WORKS OR NOT
	fclose(h_rel);
	h_rel = fopen(relPath,"r");
	//Page page2;
	uint trial = 4;
	relcatRec *r1 = (relcatRec*)malloc(sizeof(relcatRec));
	fread(&page2,PAGESIZE,1,h_rel);
	memcpy(&trial,&page2.slotmap,slotMapSize );//sizeof(uint));
	printf("fgsg %d\n",trial);
	memcpy(r1,&page2.contents[0],sizeOfRelRec);
	printf("%s\n",r1->relName);
	memcpy(r1,&page2.contents[0]+sizeOfRelRec,sizeOfRelRec);
	printf("%s\n",r1->relName);

 End of Test Code */

	/* Test Code for attribute catalog

	fclose(h_attr);
	h_attr = fopen(attrPath,"r");

	uint trial1 = 4;
	memset(&page2,0,PAGESIZE);
	attrcatRec *r2 = (attrcatRec*)malloc(sizeof(attrcatRec));
	fread(&page2,PAGESIZE,1,h_attr);
	memcpy(&trial1,&page2.slotmap,slotMapSize );//sizeof(uint));
	printf("trial1 =  %d\n",trial1);
	memcpy(r2,&page2.contents[0],sizeOfAttrRec);
	printf("attr1 = %s\n",r2->attrName);
	memcpy(r2,&page2.contents[0]+sizeOfAttrRec,sizeOfAttrRec);
	printf("attr2 = %s\n",r2->attrName);


	fseek(h_attr,SEEK_SET,512);
	fread(&page2,PAGESIZE,1,h_attr);
	memcpy(&trial1,&page2.slotmap,slotMapSize );//sizeof(uint));
	printf("trial1 =  %d\n",trial1);
	memcpy(r2,&page2.contents[0],sizeOfAttrRec);
	printf("attr1 of page 2 = %s %d\n",r2->attrName,r2->offset);

	 End Of Test Code  */

	fclose(h_rel);
	fclose(h_attr);
	free(relrec);
	free(attrrec);
	return amount;
}
