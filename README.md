# MicroHTTPd

	Library with embedded HTTPServer for GNU C.
	Compile and install.  LINK-@: /usr/local/lib/libmicrohttpd.so

				All software list:
				https://www.gnu.org/software/software.html


## Library libmicrohttpd

	https://www.gnu.org/software/libmicrohttpd/

	https://www.gnu.org/software/libmicrohttpd/manual/libmicrohttpd.html#toc-Introduction

	https://git.gnunet.org/libmicrohttpd.git

### Download source code:

	cd /area6/gcc/dev/MicroHTTPd

	git clone https://git.gnunet.org/libmicrohttpd.git

### Compile library

	./bootstrap
	./configure --prefix=/usr/local
	make
	sudo make install

	/usr/local/lib
		-rw-r--r--  1 root root   2249572 2024-12-09 15:38:47.938290927 +0100 libmicrohttpd.a
		-rwxr-xr-x  1 root root       999 2024-12-09 15:38:47.922291078 +0100 libmicrohttpd.la
		lrwxrwxrwx  1 root root        24 2024-12-09 15:38:47.922291078 +0100 libmicrohttpd.so -> libmicrohttpd.so.12.62.1
		lrwxrwxrwx  1 root root        24 2024-12-09 15:38:47.918291116 +0100 libmicrohttpd.so.12 -> libmicrohttpd.so.12.62.1
		-rwxr-xr-x  1 root root   1256744 2024-12-09 15:38:47.918291116 +0100 libmicrohttpd.so.12.62.1
	...


### compile with static library

	gcc -o hello.bin hello.c /usr/local/lib/libmicrohttpd.a -lpthread

	gcc -o webhello.bin webhello.c /usr/local/lib/libmicrohttpd.a -lpthread

### compile program dynamic-lib

	gcc -o my_program my_program.c -lmicrohttpd

### Testing

	/usr/local/bin/microhttpd-config --version

### testing

	curl http://localhost:8080


---------------------------------------------------------------------------------------------------------------------------
# Program hello.c
---------------------------------------------------------------------------------------------------------------------------

	#include <microhttpd.h>
	#include <string.h>
	#include <stdio.h>

	#define PORT 8080

	const char *response_text = "Hello, World!";

	enum MHD_Result handle_request(void *cls, struct MHD_Connection *connection,
								const char *url, const char *method, const char *version,
								const char *upload_data, size_t *upload_data_size, void **ptr)
	{
		static int aptr;
		if (&aptr != *ptr)
		{
			*ptr = &aptr;
			return MHD_YES;
		}

		if (*upload_data_size != 0)
		{
			*upload_data_size = 0;
			return MHD_YES;
		}

		struct MHD_Response *response = MHD_create_response_from_buffer(strlen(response_text),
																		(void *)response_text,
																		MHD_RESPMEM_PERSISTENT);
		int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
		MHD_destroy_response(response);
		return ret;
	}

	int main()
	{
		struct MHD_Daemon *daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
													&handle_request, NULL, MHD_OPTION_END);
		if (NULL == daemon)
			return 1;

		printf("Server is running on http://localhost:%d\n", PORT);
		getchar();
		MHD_stop_daemon(daemon);
		return 0;
	}
