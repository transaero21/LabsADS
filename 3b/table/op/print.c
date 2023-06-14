#include <stdlib.h>

#include "../table.h"

void print(Table *table, void (*println)(const char *, const char *, int))  {
    for (int i = 0; i < table->tData.csize; i++) {
        NData nData;
        KSData ksData = getKSData(table, i);
        char *key = getKey(table, i);
        fseek(table->file, ksData.ptr, SEEK_SET);
        do {
            fread(&nData, sizeof(NData), 1, table->file);
            char *info = calloc(nData.length + 1, sizeof(char));
            fread(info, sizeof(char), nData.length, table->file);
            println(key, info, nData.release);
            if (nData.ptr) fseek(table->file, nData.ptr, SEEK_SET);
            free(info);
        } while (nData.ptr);
        free(key);
    }
}
