#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *fp = fopen("main.pak", "rb");

	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	rewind(fp);

	char *ptr = malloc(size);
	fread(ptr, 1, size, fp);
	fclose(fp);

	for (int i = 0; i < size; i++)
		ptr[i] = (ptr[i]) ^ 0xF7; // 'Decrypt'

	FILE *wp = fopen("main.unpak", "wb");
	fwrite(ptr, 1, size, wp);
	fclose(wp);
	free(ptr);
	return 0;
}
