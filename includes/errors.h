#ifndef ERRORS_H
#define ERRORS_H

#include<string.h>
#include<errno.h>
#include<stdbool.h>

# define ERR_FATAL	"\033[31m[FATAL ERROR] \033[0m"
# define ERR_WARN	"\033[33m[WARNING] \033[0m"


enum
{
	ERR_SYS,                // syscall failure
	ERR_THROW,              // throw error form above function
	ERR_USAGE,              // bad usage
	ERR_CORRUPT,            // corrupt file
	ERR_SIZE,               // number of ERRs above, always last
};

# define errors(err, fmt, ...)	({					       \
	static const char	*msg[ERR_SIZE] =			       \
	{								       \
		[ERR_SYS]	= ERR_FATAL,				       \
		[ERR_THROW]	= "  -> in _",				       \
		[ERR_USAGE]	= ERR_WARN "Bad usage: ",		       \
		[ERR_CORRUPT]	= ERR_WARN "Corrupt file: ",		       \
	};								       \
	const char *sys_err = (err == ERR_SYS ? strerror(errno) : "");	       \
	const char *colon = (err == ERR_SYS ? ": " : "");		       \
	dprintf(2, "%s" fmt "%s%s\n", msg[err], ##__VA_ARGS__, colon, sys_err);\
	(false);							       \
})

#endif