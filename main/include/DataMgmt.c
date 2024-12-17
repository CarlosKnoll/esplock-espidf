#include <h_DataMgmt.h>
#include <h_SPIFFS.h>

user Usuarios[20]; //Hardcoded 20 users as limit

char* getData(char* PATH) {
    memset((void *)dataSys, 0, sizeof(dataSys));

    struct stat st;
    if (stat(PATH, &st))
    {
        ESP_LOGE(SPIFFS_TAG, "file not found");
        return NULL;
    }

    FILE *fp = fopen(PATH, "rb+");
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

void setData(char* PATH) {
    struct stat st;
    if (stat(PATH, &st))
    {
        ESP_LOGE(SPIFFS_TAG, "file not found");
    }

    FILE *fp = fopen(PATH, "r+");
    if ( fp == NULL ){
        printf("could not open file.\n");
    
    }
    printf("Before trying to write: strFinal=%s\n",strFinal);
    if (fputs(strFinal, fp) == EOF){
          ESP_LOGE(SPIFFS_TAG, "fputs failed");
    }
    // if (fprintf(fp,"%s",strFinal) == EOF){
    //     ESP_LOGE(SPIFFS_TAG, "fprintf failed");
    // }
    fclose(fp);
}

char *getUser(uint64_t TAG){
    int i;
    char buff[12]; //How to determine dynamically?
    
    sprintf(buff, "%" PRIX64, TAG);
    ESP_LOGW("DEBUG", "TAG: %s", buff);

    populateUsersSt();

    for (i=0; i<sizeof(Usuarios) / sizeof(user); i++){
        if (Usuarios[i].id != NULL && (strcmp(Usuarios[i].tag,buff) == 0)){
            return Usuarios[i].user;
        }
    }
    return "FALSE";
}

void removeUser(char *id){
    int rc;
    populateUsersSt();
    rc = rebuildUsers(id); 
    if (rc == 0){
        printf("Before calling setData: strFinal=%s\n",strFinal);
        setData(USERS_DATA_PATH);
    }
    else{
        printf("Operação não realizada");
    }
}

int rebuildUsers(char *id){
    int i;
    strFinal[0] = '\0';
    for (i=0; i<sizeof(Usuarios) / sizeof(user); i++){
        if (Usuarios[i].id != NULL && (strcmp(Usuarios[i].id,id) != 0)){
            strcat(strFinal,Usuarios[i].id);
            strcat(strFinal,",");
            strcat(strFinal,Usuarios[i].user);
            strcat(strFinal,",");
            strcat(strFinal,Usuarios[i].tag);
            strcat(strFinal,";");
        }
    }
    return 0;
}

void populateUsersSt(){
    char *str = getData(USERS_DATA_PATH);
    int i, j;
    char *p, *sp, *temp1, *svP1, *temp2, *svP2;
    
    for(i=0, temp1=str; ; i++, temp1=NULL){
        p = strtok_r(temp1, ";", &svP1);
        if(p == NULL) {
            break;
        }
        printf("Divisão por cadastro: %s\n", p);
        
        for (j=0, temp2 = p; ; j++, temp2=NULL) {
            sp = strtok_r(temp2, ",", &svP2);
            if (sp == NULL){
                break;
            }
            switch (j){
                case 0:
                    printf("--> Divisão por id: %s\n", sp);
                    Usuarios[i].id = sp;
                    break;
                case 1:
                    printf("--> Divisão por usuario: %s\n", sp);
                    Usuarios[i].user = sp;
                    break;
                case 2:
                    printf("--> Divisão por tag: %s\n", sp);
                    Usuarios[i].tag = sp;
                    break;
            }
        }
    }
}