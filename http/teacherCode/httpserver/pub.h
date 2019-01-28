/*
 * pub.h
 *
 *  Created on: 2015-4-16
 *      Author: hsc
 */

#ifndef PUB_H_
#define PUB_H_
void save_log(char *buf);
int socket_create(int port);
int socket_accept(int st);
const char *get_filetype(const char *filename); //根据扩展名返回文件类型描述
int get_file_content(const char *file_name, char **content);
void setdaemon();

#endif /* PUB_H_ */
