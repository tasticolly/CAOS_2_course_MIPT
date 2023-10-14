#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  SHA512_CTX ctx;
  if (!SHA512_Init(&ctx)) {
    exit(EXIT_FAILURE);
  }

  size_t bytes_read = 0;
  char buffer[SHA512_DIGEST_LENGTH];
  while ((bytes_read = read(STDIN_FILENO, buffer, SHA512_DIGEST_LENGTH)) > 0) {
    if (!SHA512_Update(&ctx, buffer, bytes_read)) {
      exit(EXIT_FAILURE);
    }
  }

  unsigned char hash[SHA512_DIGEST_LENGTH];
  if (!SHA512_Final(hash, &ctx)) {
    exit(EXIT_FAILURE);
  }

  printf("0x");
  for (size_t i = 0; i < SHA512_DIGEST_LENGTH; ++i) {
    printf("%02x", hash[i]);
  }
  printf("\n");

  return 0;
}
