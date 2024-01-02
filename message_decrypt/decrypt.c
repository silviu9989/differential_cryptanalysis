#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ()
{
    FILE *fisier1;
    fisier1 = fopen("message.xyz", "rb");
    fseek(fisier1, 0, SEEK_END);
    int size=ftell(fisier1);
    fseek(fisier1, 0, SEEK_SET);

    char* vec;
    vec=(char*)malloc(size*sizeof(char));

    fread(vec, 1, size, fisier1);

    int i=0;

    for(i=0; i < size; i++)
        printf("%x ", vec[i]);
    free(vec);
    return 0;
}