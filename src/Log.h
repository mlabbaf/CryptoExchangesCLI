#ifndef LOG_H
#define LOG_H

#define KGRN "\033[0;32;32m"
#define KCYN "\033[0;36m"
#define KRED "\033[0;32;31m"
#define KYEL "\033[1;33m"
#define KBLU "\033[0;32;34m"
#define KCYN_L "\033[1;36m"
#define KBRN "\033[0;33m"
#define RESET "\033[0m"


#define RED(x) KRED << x << RESET
#define YELLOW(x) KYEL << x << RESET
#define GREEN(x) KGRN << x << RESET
#define REDw(x, y) KRED << setw(y) << x << RESET
#define YELLOWw(x, y) KYEL << setw(y) << x << RESET
#define GREENw(x, y) KGRN << setw(y) << x << RESET

#endif