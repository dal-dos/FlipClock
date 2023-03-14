// request_hander.h
// module to handle api requests
#ifndef _REQUEST_HANDLER_H_
#define _REQUEST_HANDLER_H_

#include <netdb.h>

/**
 * Handle an api request
 *
 * @param request the request received
 * @param buff a buffer to write the returned value to
 */
void Handler_make_request(char* request, char* buff);

// returns a bool representing whether the program should be running
bool Handler_get_flag(void);

#endif
