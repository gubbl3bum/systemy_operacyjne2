#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <direct.h>  // do tworzenia folderu w Windows

#define BUFFER_SIZE 4096
#define CD_SIZE 734003200LL
#define DVD_SIZE 5046586573LL
#define BLURAY_SIZE 26843545600LL

typedef struct {
    char input_file[256];
    char output_dir[256];
    long long part_size;
    int part_number;
} split_params;


// funkcja tworząca katalog
int create_directory(const char *dir_path) {
    #ifdef _WIN32
        return _mkdir(dir_path);
    #else
        return mkdir(dir_path, 0755);
    #endif
}

// funkcja wyodrębniająca nazwę pliku bez ścieżki
const char* get_filename(const char* path) {
    const char *last_slash = strrchr(path, '/');
    const char *last_backslash = strrchr(path, '\\');
    const char *last_separator = last_slash > last_backslash ? last_slash : last_backslash;
    return last_separator ? last_separator + 1 : path;
}

// funkcja generująca pełną ścieżkę do pliku wyjściowego
void generate_output_filename(char *output, const split_params *params, int part_num) {
    char base_name[256];
    char extension[64] = "";
    const char* input_filename = get_filename(params->input_file);
    
    // szukanie nazwy pliku (bez rozszerzenia)
    strncpy(base_name, input_filename, sizeof(base_name) - 1);
    base_name[sizeof(base_name) - 1] = '\0';
    
    char *last_dot = strrchr(base_name, '.');
    if (last_dot != NULL) {
        strcpy(extension, last_dot);
        *last_dot = '\0';
    }
    
    // generowanie pełnej ścieżki
    snprintf(output, 256, "%s/%s_part%d%s", 
             params->output_dir, base_name, part_num, extension);
}

// funkcja przygotowująca katalog wyjściowy
int prepare_output_directory(split_params *params, int argc, char *argv[]) {
    // utworzenie katalogu na podstawie nazwy pliku
    const char* filename = get_filename(params->input_file);
    char base_name[256];
    strncpy(base_name, filename, sizeof(base_name) - 1);
        
    // usunięcie rozszerzenie dla nazwy katalogu
    char *dot = strrchr(base_name, '.');
    if (dot != NULL) *dot = '\0';
        
    snprintf(params->output_dir, sizeof(params->output_dir), "%s_parts", base_name);

    // tworzenie katalogu
    if (create_directory(params->output_dir) != 0 && errno != EEXIST) {
        perror("Nie mozna utworzyc katalogu wyjsciowego");
        return -1;
    }

    return 0;
}

long long check_predefined_size(const char *size_str) {
    if (strcasecmp(size_str, "cd") == 0) return CD_SIZE;
    if (strcasecmp(size_str, "dvd") == 0) return DVD_SIZE;
    if (strcasecmp(size_str, "blue-ray") == 0) return BLURAY_SIZE;
    return -1;
}

int split_file(split_params *params) {
    int input_fd = open(params->input_file, O_RDONLY | O_BINARY);
    if (input_fd == -1) {
        perror("Blad otwarcia pliku wejsciowego");
        return -1;
    }

    struct stat st;
    if (fstat(input_fd, &st) == -1) {
        perror("Nie mozna okreslic rozmiaru pliku");
        close(input_fd);
        return -1;
    }
    long long total_size = st.st_size;
    long long remaining_size = total_size;

    char buffer[BUFFER_SIZE];
    char output_filename[512];
    int current_part = 1;
    
    printf("Rozpoczecie podzialu pliku na czesci w katalogu: %s\n", params->output_dir);
    
    while (remaining_size > 0) {
        generate_output_filename(output_filename, params, current_part);
        
        int output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
        if (output_fd == -1) {
            perror("Blad utworzenia pliku wyjsciowego");
            close(input_fd);
            return -1;
        }

        long long current_part_size = (remaining_size > params->part_size) ? 
                                    params->part_size : remaining_size;
        long long bytes_written = 0;

        while (bytes_written < current_part_size) {
            long long bytes_to_read = BUFFER_SIZE;
            if (bytes_written + bytes_to_read > current_part_size) {
                bytes_to_read = current_part_size - bytes_written;
            }

            ssize_t bytes_read = read(input_fd, buffer, bytes_to_read);
            if (bytes_read <= 0) {
                if (bytes_read == -1) perror("Blad odczytu");
                break;
            }

            ssize_t write_result = write(output_fd, buffer, bytes_read);
            if (write_result == -1) {
                perror("Blad zapisu");
                close(output_fd);
                close(input_fd);
                return -1;
            }

            bytes_written += write_result;
        }

        close(output_fd);
        remaining_size -= current_part_size;
        current_part++;

    }

    printf("Utworzono %d czesci \n", current_part);
    close(input_fd);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Uzycie: %s <plik_wejściowy> <rozmiar_części> [katalog_wyjściowy]\n", argv[0]);
        return 1;
    }

    split_params params;
    strncpy(params.input_file, argv[1], sizeof(params.input_file) - 1);
    params.input_file[sizeof(params.input_file) - 1] = '\0';
    
    // Przygotuj katalog wyjściowy
    if (prepare_output_directory(&params, argc, argv) != 0) {
        return 1;
    }

    // Sprawdzenie predefiniowanych rozmiarów
    long long predefined = check_predefined_size(argv[2]);
    if (predefined != -1) {
        params.part_size = predefined;
    } else {
        char *endptr;
        params.part_size = strtoll(argv[2], &endptr, 10);
        if (*endptr != '\0' || params.part_size <= 0) {
            fprintf(stderr, "Nieprawidlowy rozmiar czesci\n");
            return 1;
        }
    }

    return split_file(&params);
}