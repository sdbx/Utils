#ifndef GLOBAL_H
#define GLOBAL_H

/**********
 * MACROS *
 **********/
#define MIN_DATA_COUNT 5
#define MIN_USER_COUNT 1
#
#ifndef INSTPATH /* install path; given when built */
#define INSTPATH "../"
#endif
#
#define DATA_DB_NAME "/dat.db"
#define RPTP_DB_NAME "/rt.db"
#define DATA_DB_PATH INSTPATH DATA_DB_NAME
#define RPTP_DB_PATH INSTPATH RPTP_DB_NAME
#
#define OLD_DATA_DB_NAME "/hsr.dat"
#define OLD_RPTP_DB_NAME "/usr.dat"
#define OLD_DATA_DB_PATH INSTPATH OLD_DATA_DB_NAME
#define OLD_RPTP_DB_PATH INSTPATH OLD_RPTP_DB_NAME

#endif
