/* SPIFFS filesystem example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <h_Webserver.h>
#include <h_SPIFFS.h>

static const char *SPIFFS_TAG = "SPIFFS";

void spiffs_init(void)
{
    ESP_LOGI(SPIFFS_TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(SPIFFS_TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(SPIFFS_TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(SPIFFS_TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    #ifdef CONFIG_EXAMPLE_SPIFFS_CHECK_ON_START
        ESP_LOGI(SPIFFS_TAG, "Performing SPIFFS_check().");
        ret = esp_spiffs_check(conf.partition_label);
        if (ret != ESP_OK) {
            ESP_LOGE(SPIFFS_TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
            return;
        } else {
            ESP_LOGI(SPIFFS_TAG, "SPIFFS_check() successful");
        }
    #endif

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(SPIFFS_TAG, "Failed to get SPIFFS partition information (%s). Formatting...", esp_err_to_name(ret));
        esp_spiffs_format(conf.partition_label);
        return;
    } else {
        ESP_LOGI(SPIFFS_TAG, "Partition size: total: %d, used: %d", total, used);
    }

    // Check consistency of reported partiton size info.
    if (used > total) {
        ESP_LOGW(SPIFFS_TAG, "Number of used bytes cannot be larger than total. Performing SPIFFS_check().");
        ret = esp_spiffs_check(conf.partition_label);
        // Could be also used to mend broken files, to clean unreferenced pages, etc.
        // More info at https://github.com/pellepl/spiffs/wiki/FAQ#powerlosses-contd-when-should-i-run-spiffs_check
        if (ret != ESP_OK) {
            ESP_LOGE(SPIFFS_TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
            return;
        } else {
            ESP_LOGI(SPIFFS_TAG, "SPIFFS_check() successful");
        }
    }

    // All done, unmount partition and disable SPIFFS
    // esp_vfs_spiffs_unregister(conf.partition_label);
    // ESP_LOGI(SPIFFS_TAG, "SPIFFS unmounted");
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