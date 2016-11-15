#ifndef HW_TEMPLATES_H
#define HW_TEMPLATES_H

#define HW_CALL_TPL(argc, argv, hw_name) hw_name(argc, argv)
#define HW_MAIN_TPL(hw_name)   int main(int argc, char *argv[]) {return HW_CALL_TPL(argc, argv, hw_name);}

#include <stdio.h>
#include <stdlib.h>
const char* invalid_input_error;
const int invalid_input_error_code;
const char* range_error;
const int range_error_code;
// lol
#ifdef NO_HOME
    #undef HOME
#endif


#ifdef HOME
    #define HW_MAIN(hw_name)

    #ifdef _WIN32
        #define PAUSE system("pause")
    #else
        #define PAUSE system("read -n1 -r -p \"Press any key to continue...\"")
    #endif

    #undef HW_MAIN_TPL
    #define HW_MAIN_TPL(hw_name)   int main(int argc, char *argv[]) {int tmp = HW_CALL_TPL(argc, argv, hw_name);PAUSE;return tmp;}

    #define chyba(E, N, ...) {fprintf(stderr, E, ##__VA_ARGS__); fprintf(stderr, "\n"); PAUSE; exit(N);}
    #define chyba_debug(E, N, ...) chyba(E, N, ##__VA_ARGS__)
    #define chyba1(E, ...) {fprintf(stderr, E, ##__VA_ARGS__); PAUSE; exit(1);}

    #define log_err(M, ...) {fprintf(stderr, "[ERROR] " M " [%s:%d]\a\n", ##__VA_ARGS__, __FILE__, __LINE__); PAUSE;exit(1);}

    #define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d:) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

    #define log_info(M, ...) {\
             printf("[INFO] " M " [%s:%d]\n",##__VA_ARGS__, __FILE__, __LINE__);\
    }

#else
    
    #define chyba(E, N, ...) {fprintf(stderr, E, ##__VA_ARGS__); fprintf(stderr, "\n");exit(N);}
    #define chyba_debug(E, N, ...) 
    #define chyba1(E, ...) {exit(1);}
    #define PAUSE
    #define log_info(M, ...) 
#endif

#if !defined (HOME) || defined (NORMAL_MAIN)
    #ifdef HW_MAIN
        #undef HW_MAIN
    #endif
    #define HW_MAIN(hw_name) HW_MAIN_TPL(hw_name)
#endif
// Prevent multiple main problem]
#ifdef NO_HOME
    #undef HW_MAIN
    #define HW_MAIN(hw_name)
#endif
#ifdef NDEBUG
    #define debug(M, ...)
#else
    #define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif




#endif // HW_TEMPLATES_H
