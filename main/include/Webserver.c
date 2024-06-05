#include <h_Webserver.h>
#include <h_SPIFFS.h>

#include <esp_heap_caps.h>

static char *SERVER_TAG = "Server";

static httpd_handle_t webserver_init(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 20;

    // Start the httpd server
    ESP_LOGI(SERVER_TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(SERVER_TAG, "Registering URI handlers");
        // Registering the ws handler

        httpd_register_uri_handler(server, &ws);
        httpd_register_uri_handler(server, &esplock_css);
        httpd_register_uri_handler(server, &esplock_acessos);
        httpd_register_uri_handler(server, &esplock_novoUsuario);
        httpd_register_uri_handler(server, &esplock_main);
        httpd_register_uri_handler(server, &esplock_novoAcesso);
        httpd_register_uri_handler(server, &esplock_usuarios);
        httpd_register_uri_handler(server, &esplock_js_acessos);
        httpd_register_uri_handler(server, &esplock_js_novoUsuario);
        httpd_register_uri_handler(server, &esplock_js_main);
        httpd_register_uri_handler(server, &esplock_js_novoAcesso);
        httpd_register_uri_handler(server, &esplock_js_usuarios);


        httpd_register_uri_handler(server, &sensor_test);
        httpd_register_uri_handler(server, &sensor_test_js);    

        //httpd_register_uri_handler(server, &database);    
        uint32_t freeHeapBytes = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
        ESP_LOGW("DEBUG", "Webserver.c, after registering all uri handlers: Free bytes: %lu", freeHeapBytes);
        return server;
    }

    ESP_LOGI(SERVER_TAG, "Error starting server!");
    return NULL;
}


static void initi_web_page_buffer(char* PATH)
{
    memset((void *)index_html, 0, sizeof(index_html));
    struct stat st;
    if (stat(PATH, &st))
    {
        ESP_LOGE(SPIFFS_TAG, "file not found");
        return;
    }

    FILE *fp = fopen(PATH, "rb");
    if (fread(index_html, st.st_size, 1, fp) == 0)
    {
        ESP_LOGE(SPIFFS_TAG, "fread failed");
    }
    fclose(fp);

    //Parsing any % characters

    int old_size = strlen(index_html), i=0,j=0;
    while ( i < old_size)
    {
        if (index_html[i] == '%')
        {
            final_html[j] = '%';
            j++;
            final_html[j] = index_html[i];    
        }
        final_html[j] = index_html[i];
        i++,j++;
    }
    final_html[j] = '\0';
    
}

esp_err_t send_web_page(httpd_req_t *req, char* PATH)
{
    initi_web_page_buffer(PATH);

    int response;
    sprintf(response_data, final_html);
    response = httpd_resp_send(req, response_data, HTTPD_RESP_USE_STRLEN);
    return response;
}

/* Handler to download a file kept on the server */
// esp_err_t download_get_handler(httpd_req_t *req)
// {
//     char filepath[50];
//     FILE *fd = NULL;
//     struct stat file_stat;

//     const char *filename = get_path_from_uri(filepath, ((struct file_server_data *)req->user_ctx)->base_path,
//                                              req->uri, sizeof(filepath));
//     if (!filename) {
//         ESP_LOGE(TAG, "Filename is too long");
//         /* Respond with 500 Internal Server Error */
//         httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Filename too long");
//         return ESP_FAIL;
//     }

//     /* If name has trailing '/', respond with directory contents */
//     if (filename[strlen(filename) - 1] == '/') {
//         return http_resp_dir_html(req, filepath);
//     }

//     if (stat(filepath, &file_stat) == -1) {
//         /* If file not present on SPIFFS check if URI
//          * corresponds to one of the hardcoded paths */
//         if (strcmp(filename, "/index.html") == 0) {
//             return index_html_get_handler(req);
//         } else if (strcmp(filename, "/favicon.ico") == 0) {
//             return favicon_get_handler(req);
//         }
//         ESP_LOGE(TAG, "Failed to stat file : %s", filepath);
//         /* Respond with 404 Not Found */
//         httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File does not exist");
//         return ESP_FAIL;
//     }

//     fd = fopen(filepath, "r");
//     if (!fd) {
//         ESP_LOGE(TAG, "Failed to read existing file : %s", filepath);
//         /* Respond with 500 Internal Server Error */
//         httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
//         return ESP_FAIL;
//     }

//     ESP_LOGI(TAG, "Sending file : %s (%ld bytes)...", filename, file_stat.st_size);
//     set_content_type_from_file(req, filename);

//     /* Retrieve the pointer to scratch buffer for temporary storage */
//     char *chunk = ((struct file_server_data *)req->user_ctx)->scratch;
//     size_t chunksize;
//     do {
//         /* Read file in chunks into the scratch buffer */
//         chunksize = fread(chunk, 1, SCRATCH_BUFSIZE, fd);

//         if (chunksize > 0) {
//             /* Send the buffer contents as HTTP response chunk */
//             if (httpd_resp_send_chunk(req, chunk, chunksize) != ESP_OK) {
//                 fclose(fd);
//                 ESP_LOGE(TAG, "File sending failed!");
//                 /* Abort sending file */
//                 httpd_resp_sendstr_chunk(req, NULL);
//                 /* Respond with 500 Internal Server Error */
//                 httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
//                return ESP_FAIL;
//            }
//         }

//         /* Keep looping till the whole file is sent */
//     } while (chunksize != 0);

//     /* Close file after sending complete */
//     fclose(fd);
//     ESP_LOGI(TAG, "File sending complete");

//     /* Respond with an empty chunk to signal HTTP response completion */
// #ifdef CONFIG_EXAMPLE_HTTPD_CONN_CLOSE_HEADER
//     httpd_resp_set_hdr(req, "Connection", "close");
// #endif
//     httpd_resp_send_chunk(req, NULL, 0);
//     return ESP_OK;
// }