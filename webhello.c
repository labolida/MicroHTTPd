
#include <microhttpd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PAGE "<html><head><title>libmicrohttpd demo</title>"\
			 "</head><body>libmicrohttpd demo</body></html>"

static enum MHD_Result ahc_echo(
		void * cls,
		struct MHD_Connection * connection,
		const char * url,
		const char * method,
		const char * version,
		const char * upload_data,
		size_t * upload_data_size,
		void ** ptr
){
		static int dummy;
		const char * page = cls;
		struct MHD_Response * response;
		int ret;

		if (0 != strcmp(method, "GET"))
		return MHD_NO; /* unexpected method */
		if (&dummy != *ptr)
		{
			/* The first time only the headers are valid,
			do not respond in the first round... */
			*ptr = &dummy;
			return MHD_YES;
		}
		if (0 != *upload_data_size)
		return MHD_NO; /* upload data in a GET!? */
		*ptr = NULL; /* clear context pointer */
		response = MHD_create_response_from_buffer (strlen(page),
													(void*) page,
									MHD_RESPMEM_PERSISTENT);
		ret = MHD_queue_response(connection,
					MHD_HTTP_OK,
					response);
		MHD_destroy_response(response);
		return ret;
}



int main(int argc, char ** argv) {
	struct MHD_Daemon * d;
	
	if (argc != 2) {
		printf("%s PORT\n",argv[0]);
		return 1;
	}

	d = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
				 atoi(argv[1]),
				 NULL,
				 NULL,
				 &ahc_echo,
				 PAGE,
				 MHD_OPTION_END);
	if (d == NULL)
	return 1;
	(void) getc (stdin);
	MHD_stop_daemon(d);
	return 0;
}

