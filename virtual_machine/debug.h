#ifndef DEBUG_H
#define DEBUG_H

#if defined(DEBUG) && DEBUG > 0

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

 #define DEBUG( color , type , fmt, args...) fprintf(stderr, "%s[%s]: %s:%d:%s(): " fmt, \
     color , type , __FILE__, __LINE__, __func__, ##args)

 #define DEBUG_ERROR( fmt , args... ) DEBUG( KRED , "ERROR"  , fmt , ##args )
 #define DEBUG_WARING( fmt , args... ) DEBUG( KYEL  , "WARING" , fmt , ##args )
 #define DEBUG_SUCCESS( fmt , args... ) DEBUG( KGRN, "SUCCESS" ,fmt , ##args )
 #define DEBUG_PRINT( fmt , args... ) DEBUG( KBLU , "INFO" ,  fmt , ##args )

#else
 	#define DEBUG_ERROR( fmt , args... ) 
 	#define DEBUG_WARING( fmt , args... )
 	#define DEBUG_SUCCESS( fmt , args... ) 
 	#define DEBUG_PRINT( fmt , args... ) // nothing to debug :D
#endif


#endif