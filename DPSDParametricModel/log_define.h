#ifndef LOG_DEFINE_H
#define LOG_DEFINE_H

#define PT_TEST_LOG(func,status,err) \
if (status) \
{ \
    printf (" LOG Error: %s\t%d\n", func, status); \
    fprintf (errlog_fp, " LOG Error: %s\t%d\n", func, status); \
} \
else \
{ \
   printf (" LOG %s\t%d\n", func, status); \
    fprintf (errlog_fp, " LOG %s\t%d\n", func, status); \
}

#define PT_TEST_LOG_SUCC(func) \
 PT_TEST_LOG (func, status, status != PRO_TK_NO_ERROR)

#endif