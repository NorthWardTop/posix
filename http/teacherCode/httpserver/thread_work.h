/*
 * thread_work.h
 *
 *  Created on: 2015-4-16
 *      Author: hsc
 */

#ifndef THREAD_WORK_H_
#define THREAD_WORK_H_

void get_http_command(char *http_msg, char *command);
int  make_http_content(const char *command, char **content);
void * http_thread(void *argc);

#endif /* THREAD_WORK_H_ */
