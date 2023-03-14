// request_hander.h
// module to handle api requests
#ifndef _REQUEST_HANDLER_H_
#define _REQUEST_HANDLER_H_

#include <netdb.h>

/**
 * Handle an api request
 *
 * @param request the request received
 * @param sin_remote a pointer referencing the client sending the request
 */
void Handler_make_request(char* request, struct sockaddr *sin_remote);

#endif
