#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/evp.h>

int main(int argc, char** argv) {
  char* password = argv[1];
  EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
  unsigned char salt_header[8];
  unsigned char salt[8] = {0};
  read(STDIN_FILENO, salt_header, sizeof(salt_header));
  read(STDIN_FILENO, salt, sizeof(salt));

  unsigned char key[32] = {0};
  unsigned char iv[16] = {0};
  EVP_BytesToKey(
      EVP_aes_256_cbc(),    // алгоритм шифрования
      EVP_sha256(),         // алгоритм хеширования пароля
      salt,                 // соль
      password, strlen(password), // пароль
      1,                    // количество итераций хеширования
      key,          // результат: ключ нужной длины
      iv            // результат: начальный вектор нужной длины
  );

  EVP_DecryptInit(
      ctx,                  // контекст для хранения состояния
      EVP_aes_256_cbc(),    // алгоритм шифрования
      key,                  // ключ нужного размера
      iv                    // начальное значение нужного размера
  );

  const size_t buffer_size = 16;
  unsigned char encoded[buffer_size];
  unsigned char decoded[buffer_size];

  int bytes_read;
  int bytes_written = 0;
  while ((bytes_read = read(STDIN_FILENO, encoded, buffer_size)) > 0) {
    EVP_DecryptUpdate(ctx, decoded, &bytes_written, encoded, bytes_read);
    write(STDOUT_FILENO, decoded, bytes_written);
  }

  EVP_DecryptFinal_ex(ctx, decoded, &bytes_written);
  write(STDOUT_FILENO, decoded, bytes_written);

  EVP_CIPHER_CTX_free(ctx);
}

