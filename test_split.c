#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <direct.h>
#include <sys/stat.h>
#include <windows.h>

#define TEST_FILE_NAME "test_file.dat"
#define BUFFER_SIZE (1024 * 1024)  // 1MB buffer

typedef struct {
    const char* test_name;
    const char* input_file;
    const char* split_size;
    int expected_result;  // 0 = sukces, inny = oczekiwany błąd
    const char* description;
} TestCase;

// Rozszerzona tablica przypadków testowych, zawierająca również przypadki błędów
TestCase test_cases[] = {
    // Pozytywne przypadki testowe
    {
        "Test 1: Maly plik (1MB), podzial na 100KB", 
        "small_file.dat",
        "102400",
        0,
        "Prawidlowy podzial pliku"
    },
    {
        "Test 2: Podzial na CD", 
        "cd_split.dat",
        "cd",
        0,
        "Prawidlowy podzial pliku na czesci CD"
    },
    {
        "Test 3: Duzy plik (50MB) z czesciami 3MB", 
        "big_file_small_parts.dat",
        "3145728",  
        0,
        "Prawidlowy podzial duzego pliku na czesci 3MB"
    },
    
    // Negatywne przypadki testowe
    {
        "Error Test 1: Nieistniejacy plik", 
        "nonexistent.txt",
        "1048576",
        1,
        "Program powinien zglosic blad otwarcia pliku"
    },
    {
        "Error Test 2: Nieprawidlowy rozmiar (ujemny)", 
        "nonexistent_size.dat",
        "-1024",
        1,
        "Program powinien odrzucic ujemny rozmiar czesci"
    },
    {
        "Error Test 3: Nieprawidlowy rozmiar (zero)", 
        "zero_size.dat",
        "0",
        1,
        "Program powinien odrzucic zerowy rozmiar czesci"
    },
    {
        "Error Test 4: Nieprawidlowy rozmiar (tekst)", 
        "text_test.dat",
        "abc",
        1,
        "Program powinien odrzucic nieprawidlowy format rozmiaru"
    },
    {
        "Error Test 5: Nieprawidlowy predefiniowany rozmiar", 
        "wrong_format.dat",
        "dvd-wrong",
        1,
        "Program powinien odrzucic nieprawidlowy predefiniowany rozmiar"
    }
};

// Funkcja generująca plik testowy o zadanym rozmiarze
int generate_test_file(const char* filename, long long size) {

    printf("Generowanie pliku testowego '%s' o rozmiarze %lld bajtow...\n", filename, size);
    
    // Dla testów z nieistniejącymi plikami, nie generujemy pliku
    if (strcmp(filename, "nonexistent.txt") == 0) {
        return 1;
    }

    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Blad: Nie mozna utworzyc pliku testowego!\n");
        return 0;
    }

    unsigned char* buffer = (unsigned char*)malloc(BUFFER_SIZE);
    if (!buffer) {
        printf("Blad: Nie mozna zaalokować bufora!\n");
        fclose(file);
        return 0;
    }

    srand((unsigned)time(NULL));
    long long remaining = size;
    
    while (remaining > 0) {
        int chunk_size = (remaining > BUFFER_SIZE) ? BUFFER_SIZE : (int)remaining;
        for (int i = 0; i < chunk_size; i++) {
            buffer[i] = (unsigned char)rand();
        }

        size_t written = fwrite(buffer, 1, chunk_size, file);
        if (written != chunk_size) {
            printf("Blad: Nie mozna zapisac danych do pliku!\n");
            free(buffer);
            fclose(file);
            return 0;
        }

        remaining -= chunk_size;
        printf("\rPostep: %.1f%%", (100.0 * (size - remaining)) / size);
    }
    printf("\n");

    free(buffer);
    fclose(file);
    return 1;
}

// Funkcja sprawdzająca wynik testu
void verify_test_result(TestCase* test, int actual_result) {
    printf("Oczekiwany wynik: %s (kod: %d)\n", 
           test->expected_result == 0 ? "Sukces" : "Blad",
           test->expected_result);
    printf("Otrzymany wynik: %s (kod: %d)\n", 
           actual_result == 0 ? "Sukces" : "Blad",
           actual_result);
    
    if ((test->expected_result == 0 && actual_result == 0) ||
        (test->expected_result != 0 && actual_result != 0)) {
        printf("Test zakonczony sukcesem: %s\n", test->description);
    } else {
        printf("Test nieudany: %s\n", test->description);
    }
}

// Funkcja wykonująca pojedynczy test
void run_test(TestCase* test) {
    printf("\n=== %s ===\n", test->test_name);
    
    // Dla pozytywnych testów generujemy plik testowy
    if (test->expected_result == 0) {
        if (!generate_test_file(test->input_file, BUFFER_SIZE)) {
            printf("Test nieudany - nie można wygenerowac pliku testowego!\n");
            return;
        }
    }

    // Wykonaj komendę split
    char command[512];
    snprintf(command, sizeof(command), "split.exe \"%s\" %s", 
             test->input_file, test->split_size);
    
    printf("Wykonywanie komendy: %s\n", command);
    int result = system(command);
    
    // Weryfikuj wynik
    verify_test_result(test, result);

    // Sprzątanie - usuń plik testowy dla pozytywnych testów
    if (test->expected_result == 0) {
        if (remove(test->input_file) != 0) {
            printf("Ostrzezenie: Nie można usunac pliku testowego!\n");
        }
    }
}

int main() {
    printf("=== Rozpoczecie testow programu split ===\n");
    
    // Sprawdź czy program split.exe istnieje
    FILE* test = fopen("split.exe", "rb");
    if (!test) {
        printf("Blad: Nie znaleziono programu split.exe!\n");
        return 1;
    }
    fclose(test);

    // Wykonaj wszystkie testy
    int num_tests = sizeof(test_cases) / sizeof(TestCase);
    int successful_tests = 0;
    int failed_tests = 0;

    for (int i = 0; i < num_tests; i++) {
        run_test(&test_cases[i]);
        if ((test_cases[i].expected_result == 0 && system("echo %errorlevel%") == 0) ||
            (test_cases[i].expected_result != 0 && system("echo %errorlevel%") != 0)) {
            successful_tests++;
        } else {
            failed_tests++;
        }
    }

    printf("\n=== Podsumowanie testow ===\n");
    printf("Wykonano testow: %d\n", num_tests);
    printf("Udanych: %d\n", successful_tests);
    printf("Nieudanych: %d\n", failed_tests);

    return failed_tests > 0 ? 1 : 0;
}