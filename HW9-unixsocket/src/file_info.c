
#include "file_info.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

size_t get_fileinfo_str(const char *path, char *buf)
{
    size_t retval = 0;

    struct stat sb;
    if (stat(path, &sb) == -1)
    {
        retval = sprintf(buf, "Can not get file info for %s\n", path);
    }
    if ((sb.st_mode & S_IFMT) != S_IFREG)
    {
        retval = sprintf(buf, "Not a regular file %s\n", path);
    }
    else
    {
        retval = sprintf(buf, "%s   size  %ld  on disk %ld\n", path, sb.st_size,
                         sb.st_blksize * sb.st_blocks);
    }

    return retval;
}