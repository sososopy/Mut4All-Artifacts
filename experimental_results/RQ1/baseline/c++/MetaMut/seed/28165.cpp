
typedef struct TSFastDbgCntl_
{
    const char * const tag; // nul-terminated string
    const char * const on;  // pointer to 1-byte flag
}
TSFastDbgCntl;

TSFastDbgCntl * TSCreateFastDbgCntl(const char *tag);

#include <stdarg.h>
void TSVDebug(const char *tag, const char *fmt, va_list args);
inline void TSFastDbg(TSFastDbgCntl *fd_cntl, const char *fmt, ...)
{
    if (fd_cntl->on)
    {
        va_list ap;
        va_start(ap, fmt);
        TSVDebug(fd_cntl->tag, fmt, ap);
    }
}

void dummy(int i, double d)
{
    TSFastDbgCntl *fd_cntl = TSCreateFastDbgCntl("pluggymcplugin");
    TSFastDbg(fd_cntl, "Test %d %f", i * 5, d * 7);
    TSFastDbg(fd_cntl, "Test fixed string");
}

