#include <h_DataMgmt.h>
#include <h_SPIFFS.h>


char* getData(char* PATH) {
    char data[512];
    char data_final[512];

    memset((void *)data, 0, sizeof(data));

    struct stat st;
    if (stat(PATH, &st))
    {
        ESP_LOGE(SPIFFS_TAG, "file not found");
        return NULL;
    }

    FILE *fp = fopen(PATH, "rb");
    if (fread(data, st.st_size, 1, fp) == 0)
    {
        ESP_LOGE(SPIFFS_TAG, "fread failed");
    }
    fclose(fp);

    //Parsing any % characters

    int old_size = strlen(data), i=0,j=0;
    while ( i < old_size)
    {
        if (data[i] == '%')
        {
            data_final[j] = '%';
            j++;
            data_final[j] = data[i];    
        }
        data_final[j] = data[i];
        i++,j++;
    }
    data_final[j] = '\0';

    return data_final;
}