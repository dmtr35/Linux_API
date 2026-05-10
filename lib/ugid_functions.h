#ifndef TLPI_HDR_H
#define TLPI_HDR_H

#include <sys/types.h>                  /* uid_t, gid_t */

#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <sys/stat.h>

char* userNameFromId(uid_t uid);
uid_t userIdFromName(const char *name);

char* groupNameFromId(gid_t gid);
gid_t groupIdFromName(const char *name);

#endif