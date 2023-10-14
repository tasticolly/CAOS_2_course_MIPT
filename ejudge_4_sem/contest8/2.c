#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

typedef struct {
  char   *data;
  size_t length;
} buffer_t;

static size_t
callback_function(
    char *ptr, // буфер с прочитанными данными
    size_t chunk_size, // размер фрагмента данных
    size_t nmemb, // количество фрагментов данных
    void *user_data // произвольные данные пользователя
)
{
  buffer_t *buffer = user_data;
  size_t total_size = chunk_size * nmemb;
  memcpy(buffer->data, ptr, total_size);
  buffer->length += total_size;
  return total_size;
}

int main(int argc, char *argv[]) {
  CURL *curl = curl_easy_init();
  if(curl) {
    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_function);

    buffer_t buffer;
    buffer.data = calloc(100*1024*1024, 1);
    buffer.length = 0;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
    res = curl_easy_perform(curl);

    char* start = strstr(buffer.data,"<title>") + strlen("<title>");
    char* end = strstr(buffer.data,"</title>");
    *(end) = '\0';
    printf("%s", start);
    free(buffer.data);
    curl_easy_cleanup(curl);
  }
}
