#include <h_Database.h>
#include <h_SPIFFS.h>

#include <esp_heap_caps.h>


static const char *DB_TAG = "sqlite3";

sqlite3 *db;

const char* dbData = "Output:";

char *zErrMsg = 0;
char db_file_name[MAX_FILE_NAME_LEN] = "\0";
char str[MAX_STR_LEN];

int rc;
char* sql;
char* message;

// db operation functions
static int callback(void *data, int argc, char **argv, char **azColName) {
   int i;
   printf("%s: ", (const char*)dbData);
   for (i = 0; i<argc; i++){
       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int db_open(const char *db_file_name) {
  if (db != NULL)
    sqlite3_close(db);
  int rc = sqlite3_open(db_file_name, &db);
  if (rc) {
    printf("Can't open database: ");
    printf(sqlite3_errmsg(db));
    printf("\n");
    return rc;
  } else
    printf("Opened database successfully\n");
  return rc;
}

int db_exec(const char *sql) {
  if (db == NULL) {
    printf("No database open\n");
    return 0;
  }
  long start = esp_timer_get_time();
  int rc = sqlite3_exec(db, sql, callback, (void*)dbData, &zErrMsg);
  //int rc = sqlite3_prepare_v2(db, sql, strlen(sql), )
  if (rc != SQLITE_OK) {
    printf("SQL error: ");
    printf(zErrMsg);
    sqlite3_free(zErrMsg);
  } else
  printf("Operation done successfully\n");
  printf("Time taken:");
  printf("%lld", esp_timer_get_time() - start);
  printf(" us\n");
  return rc;
}

//Starts db
void db_init() {
  sqlite3_initialize();
}

//Check if tag read is within users table
void dbCheck(uint64_t tag){
    message = "";
    db_open(DATABASE_PATH);

    char * query_1 = "SELECT IFNULL((SELECT name FROM users WHERE tag == '";
    char * query_2 = "'),'FALSE');";
    char buff[100]; //How to determine dynamically?

    sprintf(buff, "%" PRIX64, tag);

    sql = malloc(strlen(query_1)+strlen(buff)+strlen(query_2));
    strcpy(sql,query_1);
    strcat(sql,buff);
    strcat(sql,query_2);

    ESP_LOGI(DB_TAG, "Query: %s", sql);

    rc = db_exec(sql);

    free(sql);

    // if (message.equals("FALSE")){
    //     printf("Usuário não cadastrado");
    // }  
    // else{
    //     printf("Bem vindo(a) " + message);
    // }
    tag_INT=0;
    sqlite3_close(db);
    
    uint32_t freeHeapBytes = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    ESP_LOGW("DEBUG", "Database.c after doing a query in db: Free bytes: %lu", freeHeapBytes);
}
