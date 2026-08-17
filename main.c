#define _CRT_SECURE_NO_WARNINGS // Отключаем предупреждения VS на старые функции
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// Объявляем тип нашей ассемблерной функции из DLL
typedef void (*XorFunc)(char*, long long, char);

int main() {
    char filename[260];
    char key;

    // 1. Запрашиваем данные у пользователя
    printf("=== Ultrasmall C/MASM Cryptor ===\n");
    printf("Enter file path (for example, test.txt): ");
    scanf("%259s", filename);

    printf("Enter one symbol key: ");
    scanf(" %c", &key); // Пробел перед %c нужен, чтобы пропустить символ переноса строки

    // 2. Открываем файл в бинарном режиме ("rb" - read binary)
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("ERROR: Cannot open file!\n");
        system("pause");
        return 1;
    }

    // 3. Узнаем размер файла с помощью стандартных функций Си
    fseek(file, 0, SEEK_END);      // Прыгаем в конец файла
    long long fileSize = ftell(file); // Получаем текущую позицию (это и есть размер)
    fseek(file, 0, SEEK_SET);      // Возвращаемся в начало файла

    printf("File size: %lld byte.\n", fileSize);

    // 4. Выделяем память через комфортный malloc
    // Выделяем на 1 байт больше для нулевого символа в конце
    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        printf("ERROR: Not enough RAM!\n");
        fclose(file);
        return 1;
    }

    // 5. Читаем весь файл в память и закрываем его
    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0'; // Важно для нашего цикла в MASM!
    fclose(file);

    // 6. Динамически подгружаем твою ассемблерную DLL
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

    // 7. ЗАПУСКАЕМ ТВОЙ АССЕМБЛЕРНЫЙ ДВИЖОК!
    printf("Crypting/Encrypting data...\n");
    MyXor(buffer, fileSize, key); // Передаем точный размер файла!

    // 8. Открываем этот же файл на ЗАПИСЬ ("wb" - write binary) и сохраняем изменения
    file = fopen(filename, "wb");
    if (file == NULL) {
        printf("ERROR: Failed rewrite file!\n");
        FreeLibrary(hLib);
        free(buffer);
        return 1;
    }
    fwrite(buffer, 1, fileSize, file);
    fclose(file);

    // 9. Очищаем ресурсы
    FreeLibrary(hLib);
    free(buffer);

    printf("SUCCESS! File rewritten.\n");
    system("pause");
    return 0;
}
