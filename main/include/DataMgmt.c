#include <h_DataMgmt.h>
#include <h_SPIFFS.h>

char* getData(char* PATH) {

    memset((void *)dataSys, 0, sizeof(dataSys));

    struct stat st;
    if (stat(PATH, &st))
    {
        ESP_LOGE(SPIFFS_TAG, "file not found");
        return NULL;
    }

    FILE *fp = fopen(PATH, "rb");
    if (fread(dataSys, st.st_size, 1, fp) == 0)
    {
        ESP_LOGE(SPIFFS_TAG, "fread failed");
    }
    fclose(fp);

    //Parsing any % characters

    int old_size = strlen(dataSys), i=0,j=0;
    while ( i < old_size)
    {
        if (dataSys[i] == '%')
        {
            dataSys_final[j] = '%';
            j++;
            dataSys_final[j] = dataSys[i];    
        }
        dataSys_final[j] = dataSys[i];
        i++,j++;
    }
    dataSys_final[j] = '\0';

    return dataSys_final;
}

char *getUser(uint64_t TAG){
    char *temp = getData(USERS_DATA_PATH);
    char *p, *str1, *str2, *sp;
    char *svP, *svP2;
    char *lastsp = "";
    int j;

    char buff[12]; //How to determine dynamically?
    sprintf(buff, "%" PRIX64, TAG);

    ESP_LOGW("DEBUG", "TAG: %s", buff);

    for (j = 1, str1 = temp; ; j++, str1 = NULL) {
        p = strtok_r(str1, ";", &svP);
        if(p == NULL) {
            break;
        }
        printf("%s\n", p);

        for (str2 = p; ; str2 = NULL) {
               sp = strtok_r(str2, ",", &svP2);
               if (sp == NULL){
                    break;
               }
               printf(" --> %s\n", sp);
               printf("DEBUG: lastsp =  %s\n", lastsp);
               printf("DEBUG: buff =  %s\n", buff);
               //ESP_LOGW("DEBUG", "TAG: %s", lastsp);
               if (strcmp(sp,buff) == 0){
                    return lastsp;
               }
               lastsp = sp;
                printf("DEBUG: new lastsp =  %s\n", lastsp);
           }
    }
    return "FALSE";
}