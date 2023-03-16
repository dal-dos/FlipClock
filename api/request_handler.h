// request_hander.h
// API to handle socket messages
#ifndef _REQUEST_HANDLER_H_
#define _REQUEST_HANDLER_H_

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
