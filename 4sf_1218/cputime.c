#include <windows.h>
#include <stdio.h>

void get_cpu_time(double *usertime, double *systime)
{
    FILETIME creation, exit, kernel, user;
    GetProcessTimes(GetCurrentProcess(), &creation, &exit, &kernel, &user);
    ULARGE_INTEGER u, k;
    u.LowPart  = user.dwLowDateTime;
    u.HighPart = user.dwHighDateTime;
    k.LowPart  = kernel.dwLowDateTime;
    k.HighPart = kernel.dwHighDateTime;
    *usertime = (double)u.QuadPart / 1e7;
    *systime  = (double)k.QuadPart / 1e7;
}

double report_cpu(FILE *fp, char *str)
{
    double utime, stime;
    get_cpu_time(&utime, &stime);
    if (fp) {
        fprintf(fp, "%lf\t", utime + stime);
        fflush(fp);
    }
    return utime + stime;
}
