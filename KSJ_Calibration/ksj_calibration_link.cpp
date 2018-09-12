#ifdef _WIN64

#else
#     ifdef _WIN32
#        if ( defined (DEBUG) )        // /MDd  use libcmt.lib
#            pragma comment(lib, "..\\..\\git.KSJLog\\KSJLog.Lib\\win32\\MDd_KSJLog_x86.lib")
//#            pragma comment(lib, "..\\..\\github.KSJApiNew\\KSJApi.lib\\win32\\KSJApid.lib")
#            pragma comment(lib, "..\\..\\git.KSJApi\\KSJApi.lib\\KSJApid.lib")
#        else
#            pragma comment(lib, "..\\..\\git.KSJLog\\KSJLog.Lib\\win32\\MD_KSJLog_x86.lib")
//#            pragma comment(lib, "..\\..\\github.KSJApiNew\\KSJApi.lib\\win32\\KSJApi.lib")
#            pragma comment(lib, "..\\..\\git.KSJApi\\KSJApi.lib\\KSJApi.lib")
#        endif
#     else
#     endif
#endif
