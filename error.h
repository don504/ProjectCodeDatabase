/************************************************************
   This file contains the error constants that are
   used by the routine that reports error messages.
   As you find error conditions to report, 
   make additions to this file. 
************************************************************/

#define RELATIONDOESNOTEXIST  101 /* Relation does not exist */
#define ATTRDOESNOTEXIST 102	  /* Attribute does not exist */
#define BADATTRTYPE	103      	  /* Invalid attribute type */
#define BADCOMMAND 104       	  /* Invalid relational operator */
#define	WRONGNUMARGS 105     	  /* Wrong number of arguments for routine */
#define	DUPLICATEATTR 106    	  /* Attribute specified more than once */
#define	RECORDALREADYEXIST	107   /* Record already exists */
#define	ILLEGALCATACCESS	108   /* Tried to append to catalogs */
#define	DATABASEDOESNOTEXIST 109  /* Tried to open a Database that does not exist */
#define	DATABASEALREADYOPENED 110 /* Database have already been opened */
#define	DATABASEALREADYEXIST  111 /* Database already exits */
#define RELATIONALREADYEXIST 112	/* Relation already exists */
#define RELATIONCATCREATEFAILED 113	/* Relation Catalog Create Failed */
#define ATTRCATCREATEFAILED 114		/*Attribute Catalog Create Failed */
