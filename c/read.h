#ifndef READ__H_
#define READ__H_

/*
 * Read complete file into string
 * fname - file name / path
 * result - content of the file
 * Returns length of buffer, inluding the terminating \0.
 * On error, returns negative number
 *
 * RESULT NEEDS TO BE FREED AFTERWARDS!
 */

long frdall(const char *fname, char **result);

#endif
