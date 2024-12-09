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
