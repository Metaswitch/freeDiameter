/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Sebastien Decugis <sdecugis@freediameter.net>							 *
*													 *
* Copyright (c) 2013, WIDE Project and NICT								 *
* All rights reserved.											 *
* 													 *
* Redistribution and use of this software in source and binary forms, with or without modification, are  *
* permitted provided that the following conditions are met:						 *
* 													 *
* * Redistributions of source code must retain the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer.										 *
*    													 *
* * Redistributions in binary form must reproduce the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer in the documentation and/or other						 *
*   materials provided with the distribution.								 *
* 													 *
* * Neither the name of the WIDE Project or NICT nor the 						 *
*   names of its contributors may be used to endorse or 						 *
*   promote products derived from this software without 						 *
*   specific prior written permission of WIDE Project and 						 *
*   NICT.												 *
* 													 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED *
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 	 *
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 	 *
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR *
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF   *
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.								 *
*********************************************************************************************************/

#include "fdproto-internal.h"

DECLARE_FD_DUMP_PROTOTYPE(fd_sa_dump, sSA * sa, int flags) 
{
	char addrbuf[INET6_ADDRSTRLEN+2];
	int rc;
	FD_DUMP_HANDLE_OFFSET();
	
	if (sa) {
        rc = fd_addr_to_string(sa, addrbuf);
		if (rc) {
			CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "%s", gai_strerror(rc)), return NULL);
		} else {
			CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "%s", &addrbuf[0]), return NULL);
		}
	} else {
		CHECK_MALLOC_DO( fd_dump_extend( FD_DUMP_STD_PARAMS, "(NULL / ANY)"), return NULL);
	}
	return *buf;
}

void fd_sa_sdump_numeric(char * buf /* must be at least sSA_DUMP_STRLEN */, sSA * sa)
{
	char addrbuf[INET6_ADDRSTRLEN+2];
	if (sa) {
        int rc = fd_addr_to_string(sa, addrbuf);
  		if (rc) {
			snprintf(buf, sSA_DUMP_STRLEN, "%s", gai_strerror(rc));
		} else {
			snprintf(buf, sSA_DUMP_STRLEN, "%s", addrbuf);
		}
	} else {
		snprintf(buf, sSA_DUMP_STRLEN, "(NULL / ANY)");
	}
	
}

int  fd_addr_to_string(struct sockaddr *sa, char *buf) {
        const char *addrstr;
        int rc;

		strcpy(buf, "[");
		if (((struct sockaddr *)sa)->sa_family == AF_INET) {
			struct sockaddr_in *s = (struct sockaddr_in *)sa;
			addrstr = inet_ntop(AF_INET, &(s->sin_addr), &buf[1], INET6_ADDRSTRLEN);
		} else {
			struct sockaddr_in6 *s = (struct sockaddr_in6 *)sa;
			addrstr = inet_ntop(AF_INET6, &(s->sin6_addr), &buf[1], INET6_ADDRSTRLEN);
		}
		if (addrstr == NULL)
		{
			rc = errno;
		}
		strcat(buf, "]");
  	return rc;
}
