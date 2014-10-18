#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"


void setSlotHelper(char *slotmap,int slot){
 *slotmap = *slotmap | 1<<slot;
 }

void setSlot(char *slotmap,uint slot,uint slotSize){

    uint pos = floor(slot/8.0);
    uint num = (slot % 8);
    setSlotHelper(&slotmap[pos],num);
}


relcatRec* fillRelationRecord(const char* rName,uint rLength,uint recsPage,uint nAttrs,uint nRecs,uint nPgs){
	relcatRec *rec = (relcatRec*)malloc(sizeof(relcatRec));
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
	int recsPerPage = floor(MAXRECORD/sizeOfRelRec); /* Error Can Come ! Correct  ?!*/
	relcatRec *relrec = fillRelationRecord("RELCAT",sizeOfRelRec,recsPerPage,6,2,1);

	unsigned int sizeOfAttrRec = sizeof(attrcatRec); //50 Bytes .. Look out for the alignment problem!
	int attrRecsPerPage = floor(MAXRECORD/sizeOfAttrRec); // 10
	relcatRec *attrrec = fillRelationRecord("ATTRCAT",sizeOfAttrRec,attrRecsPerPage,5,11,2);

	Page page1,page2;
	uint recP = ceil(recsPerPage/8.0);
	unsigned char* bitmap = malloc(recP);
	setSlot(bitmap,0,recP);
	setSlot(bitmap,1,recP);
	memset(&page1.contents[0],0,PAGESIZE);
	memcpy(&page1.contents[0],bitmap,recP);	//*(&page1.slotmap) = slotmap;
	memcpy(&page1.contents[0]+recP,relrec,sizeOfRelRec);
	memcpy(&page1.contents[0]+recP+sizeOfRelRec,attrrec,sizeOfRelRec);
	fwrite(&page1,PAGESIZE,1,h_rel);

	/* Writing attribute records */

	Page page[2];
	recP = ceil(attrRecsPerPage/8.0);
	*bitmap = malloc(recP);
	memset(bitmap,0,recP);
	memset(&page[0].contents[0],0,PAGESIZE);
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

	int i = 0;
	for(i=0;i<10;i++)
		setSlot(bitmap,i,recP);

	memcpy(&page[0].contents[0],bitmap,recP);
	printf("---- %d",recP);
	for(index=0;index<attrRecsPerPage;index++){
		memcpy(&page[0].contents[0] +recP +(index*sizeOfAttrRec),arecArray[index],sizeOfAttrRec);
		free(arecArray[index]);
	}

	memset(bitmap,0,recP);
	setSlot(bitmap,0,recP);
	memset(&page[1].contents[0],0,PAGESIZE);
	memcpy(&page[1].contents[0],bitmap,recP);
	memcpy(&page[1].contents[0]+ recP,arecArray[10],sizeOfAttrRec);
	fwrite(&page,PAGESIZE,2,h_attr);
	free(arecArray[10]);
	free(bitmap);

	/*
	fclose(h_rel);
	h_rel = fopen(relPath,"r");
	uint trial = 4;
	relcatRec *r1 = (relcatRec*)malloc(sizeof(relcatRec));
	fread(&page2,PAGESIZE,1,h_rel);
	char* temp1 = (char*)malloc(2* sizeof(char));
	memcpy(temp1,&page2.contents[0],recP);
	printf("\n %d %d\n",temp1[0],temp1[1]);
	memcpy(r1,&page2.contents[0]+recP,sizeOfRelRec);
	printf("%s\n",r1->relName);
	memcpy(r1,&page2.contents[0]+recP+sizeOfRelRec,sizeOfRelRec);
	printf("%s\n",r1->relName);
*/

	recP = 2;
	fclose(h_attr);
	h_attr = fopen(attrPath,"r");
	memset(&page2,0,PAGESIZE);
	attrcatRec *r2 = (attrcatRec*)malloc(sizeof(attrcatRec));
	fread(&page2,PAGESIZE,1,h_attr);
	unsigned char* temp = (unsigned char*)malloc(recP* sizeof(unsigned char));
	uint val = 0;
	memcpy(temp,&page2.contents[0],recP);
	printf("\n %u %u\n",temp[0],temp[1]);
	printf("-- %d",val);
	memcpy(r2,&page2.contents[0]+recP,sizeOfAttrRec);
	printf("attr1 = %s\n",r2->attrName);
	memcpy(r2,&page2.contents[0]+recP+sizeOfAttrRec,sizeOfAttrRec);
	printf("attr2 = %s\n",r2->attrName);

	fclose(h_rel);
	fclose(h_attr);
	free(relrec);
	free(attrrec);
	return OK;
}
