#include <h_Webserver.h>
#include <h_SPIFFS.h>

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

        // httpd_register_uri_handler(server, &chart_js);
        // httpd_register_uri_handler(server, &rtSensor);
        // httpd_register_uri_handler(server, &rtSensor_js);    
        
        return server;
    }

    ESP_LOGI(SERVER_TAG, "Error starting server!");
    return NULL;
}