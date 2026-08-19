#define _CRT_SECURE_NO_WARNINGS // disable warnings
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*XorFunc)(char*, long long, char);

int main() {
    char filename[260];
    char key;

    printf("=== Ultrasmall C/MASM Cryptor ===\n");
    printf("Enter file path (for example, test.txt): ");
    scanf("%259s", filename);

    printf("Enter one symbol key: ");
    scanf(" %c", &key); 

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("ERROR: Cannot open file!\n");
        system("pause");
        return 1;
    }

    fseek(file, 0, SEEK_END);      
    long long fileSize = ftell(file); 
    fseek(file, 0, SEEK_SET);      

    printf("File size: %lld byte.\n", fileSize);

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        printf("ERROR: Not enough RAM!\n");
        fclose(file);
        return 1;
    }

    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0'; 
    fclose(file);

    HMODULE hLib = LoadLibrary(L"CipherDLL.dll");
    if (hLib == NULL) {
        printf("ERROR: Not found CipherDLL.dll nearby .exe!\n");
        free(buffer);
        system("pause");
        return 1;
    }

    XorFunc MyXor = (XorFunc)GetProcAddress(hLib, "XorCipher");
    if (MyXor == NULL) {
        printf("ERROR: Function XorCipher not found in DLL!\n");
        FreeLibrary(hLib);
        free(buffer);
        system("pause");
        return 1;
    }

 
    printf("Crypting/Encrypting data...\n");
    MyXor(buffer, fileSize, key); // file size


    file = fopen(filename, "wb");
    if (file == NULL) {
        printf("ERROR: Failed rewrite file!\n");
        FreeLibrary(hLib);
        free(buffer);
        return 1;
    }
    fwrite(buffer, 1, fileSize, file);
    fclose(file);

    // clear resources
    FreeLibrary(hLib);
    free(buffer);

    printf("SUCCESS! File rewritten.\n");
    system("pause");
    return 0;
}
