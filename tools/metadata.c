#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define FILEFLAGS_END 0x80

typedef struct PakDef {
	uint8_t  aFlags;
	uint8_t  aNameWidth;
	char    *aName;
	int      aSrcSize;
	uint64_t aFileTime;
	size_t   offset;
} PakDef;

void PakDefPrint(PakDef* def) {
	printf("Flags: %d, Name Width: %d, Name: %s, File Size: %d, Filetime: %ld, Offset: %d\n", def->aFlags, def->aNameWidth, def->aName, def->aSrcSize, def->aFileTime, def->offset);
}

int main() {
	FILE *fp = fopen("../main.unpak", "rb");

	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	rewind(fp);

	char *b_ptr = malloc(size);
	fread(b_ptr, 1, size, fp);
	fclose(fp);

	char *ptr = b_ptr + 8;
	int aPos;

	PakDef defs[4096];
	int idx = 0;
	for (;;++idx)
	{
		if (ptr > (b_ptr + size)) break;
		PakDef* def = &defs[idx];
		def->aFlags = *(uint8_t*)ptr;
		ptr += 1;
		if (def->aFlags & FILEFLAGS_END) break;


		def->aNameWidth = *(uint8_t*)ptr;
		ptr += 1;
		def->aName = malloc(def->aNameWidth + 1);
		memcpy(def->aName, ptr, def->aNameWidth);
		def->aName[def->aNameWidth] = 0;

		int nameIdx = 0;
		while(def->aName[nameIdx])
		{     
		     if(def->aName[nameIdx] == '\\')
		        def->aName[nameIdx] = '/';
		     nameIdx++;
		}
		ptr += def->aNameWidth;

		def->aSrcSize = *(int*)ptr;
		ptr += sizeof(int);
		def->aFileTime = *(uint64_t*)ptr;
		ptr += sizeof(uint64_t);

		def->offset = aPos;
		aPos += def->aSrcSize;
	}

	size_t offset = ptr - b_ptr;

	printf("%ld\n", offset);

	for (int i = 0; i < idx; ++i) {
		FILE *fd = fopen(defs[i].aName, "wb+");

		if (!fd) {
			printf("Failed on %s\n", defs[i].aName);
			break;
		}

		defs[i].offset += offset;
		fwrite(b_ptr + defs[i].offset, 1, defs[i].aSrcSize, fd);
		fclose(fd);
		PakDefPrint(&defs[i]);
	}

	for (int i = 0; i < idx; ++i) {
		free(defs[i].aName);
	}
	free(b_ptr);
	return 0;
}
