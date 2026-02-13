#ifndef LEXER_CONFIG_H
#define LEXER_CONFIG_H

/* -------------------------------------------------------------------------- */
/* Output format configuration                                                  */
/* -------------------------------------------------------------------------- */

#define OUTFORMAT_RELEASE 0
#define OUTFORMAT_DEBUG   1

#ifndef OUTFORMAT
#define OUTFORMAT OUTFORMAT_DEBUG
#endif

/* -------------------------------------------------------------------------- */
/* Debug stream configuration                                                   */
/* -------------------------------------------------------------------------- */

#define DEBUG_OFF 0
#define DEBUG_ON  1

#ifndef DEBUG
#define DEBUG DEBUG_OFF
#endif

/* -------------------------------------------------------------------------- */
/* Counter configuration                                                        */
/* -------------------------------------------------------------------------- */

#define COUNT_DISABLED 0
#define COUNT_ENABLED  1

#ifndef COUNTCONFIG
#define COUNTCONFIG COUNT_DISABLED
#endif

#define DBGCOUNT 0
#define OUT      1

#ifndef COUNTOUT
#define COUNTOUT OUT
#endif

#ifndef COUNTFILE
#define COUNTFILE "dbgcnt"
#endif

/* -------------------------------------------------------------------------- */
/* Hook configuration for parser phase                                          */
/* -------------------------------------------------------------------------- */

#ifndef ENABLE_PARSER_HOOK
#define ENABLE_PARSER_HOOK 0
#endif

#endif
