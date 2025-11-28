#include "functions.h"
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <string.h>

volatile sig_atomic_t keep_running = 1;

void handle_signal(int sig) {
    (void)sig;
    keep_running = 0;
}

void msleep(long milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

int has_undelivered_letters(const PostSystem *system) {
    for (int i = 0; i < system->letters_count; i++) {
        if (system->letters[i].state == 0) {
            return 1;
        }
    }
    return 0;
}

void print_system_status(const PostSystem *system) {
    printf("\n=== Текущее состояние системы ===\n");
    
    int in_transit = 0, delivered = 0, undelivered = 0;
    for (int i = 0; i < system->letters_count; i++) {
        switch (system->letters[i].state) {
            case 0: in_transit++; break;
            case 1: delivered++; break;
            case 2: undelivered++; break;
        }
    }
    
    printf("Отделения: %d\n", system->offices_count);
    printf("Письма: %d (в пути: %d, доставлено: %d, недоставлено: %d)\n", 
           system->letters_count, in_transit, delivered, undelivered);

    for (int i = 0; i < system->offices_count; i++) {
        size_t current_size;
        heap_size(&system->offices[i].letters_heap, &current_size);
        printf("  Отделение %d: %zu/%zu писем, соседи: ", 
               system->offices[i].id, current_size, system->offices[i].max_letters);
        
        for (int j = 0; j < system->offices[i].neighbors_count; j++) {
            printf("%d", system->offices[i].neighbors[j]);
            if (j < system->offices[i].neighbors_count - 1) printf(", ");
        }
        printf("\n");
    }
}

StatusCode read_network_config(PostSystem *system, const char *filename, size_t default_capacity) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return ERROR_FILE_OPERATION;
    }
    
    printf("Чтение конфигурации сети из %s...\n", filename);

    #define MAX_OFFICES 100
    int offices[MAX_OFFICES] = {0};
    int office_count = 0;
    int connections[MAX_OFFICES][MAX_OFFICES] = {0};

    int office1, office2;
    while (fscanf(file, "%d %d", &office1, &office2) == 2) {
        int found1 = 0, found2 = 0;
        for (int i = 0; i < office_count; i++) {
            if (offices[i] == office1) found1 = 1;
            if (offices[i] == office2) found2 = 1;
        }
        
        if (!found1) {
            offices[office_count++] = office1;
        }
        if (!found2) {
            offices[office_count++] = office2;
        }

        for (int i = 0; i < office_count; i++) {
            if (offices[i] == office1) {
                for (int j = 0; j < office_count; j++) {
                    if (offices[j] == office2) {
                        connections[i][j] = 1;
                        connections[j][i] = 1;
                        break;
                    }
                }
                break;
            }
        }
    }
    
    fclose(file);

    for (int i = 0; i < office_count; i++) {
        int neighbors[MAX_OFFICES];
        int neighbor_count = 0;
        
        for (int j = 0; j < office_count; j++) {
            if (connections[i][j] && i != j) {
                neighbors[neighbor_count++] = offices[j];
            }
        }

        StatusCode status = post_office_add(system, offices[i], default_capacity, neighbors, neighbor_count);
        if (status != SUCCESS) {
            printf("Предупреждение: не удалось добавить отделение %d\n", offices[i]);
        }
    }
    
    printf("Создано %d почтовых отделений\n", office_count);
    return SUCCESS;
}

void automatic_processing(PostSystem *system, int max_cycles) {
    printf("\n=== Автоматическая обработка ===\n");
    printf("Обработка каждые 0.2 секунды...\n");
    printf("Нажмите Ctrl+C для остановки\n\n");
    
    int processing_cycles = 0;
    
    while (keep_running && processing_cycles < max_cycles && has_undelivered_letters(system)) {
        processing_cycles++;

        letters_process_delivery(system);

        for (int i = 0; i < system->letters_count; i++) {
            int success;
            letter_try_take(system, system->letters[i].id, system->letters[i].to_office_id, &success);
        }
        
        if (processing_cycles % 5 == 0) {
            printf("Цикл %d: ", processing_cycles);
            int in_transit = 0, delivered = 0, undelivered = 0;
            for (int i = 0; i < system->letters_count; i++) {
                switch (system->letters[i].state) {
                    case 0: in_transit++; break;
                    case 1: delivered++; break;
                    case 2: undelivered++; break;
                }
            }
            printf("в пути: %d, доставлено: %d, недоставлено: %d\n", 
                   in_transit, delivered, undelivered);
        }
        
        msleep(200);
    }
    
    printf("\nАвтоматическая обработка завершена после %d циклов\n", processing_cycles);
    print_system_status(system);
}

void interactive_menu(PostSystem *system) {
    int choice;
    
    do {
        printf("\n=== ПОЧТОВЫЙ СЕРВИС ===\n");
        printf("1. Добавить почтовое отделение\n");
        printf("2. Удалить почтовое отделение\n");
        printf("3. Добавить письмо\n");
        printf("4. Пометить письмо как недоставленное\n");
        printf("5. Попытка взять письмо\n");
        printf("6. Показать все письма\n");
        printf("7. Показать состояние системы\n");
        printf("8. Автоматическая обработка\n");
        printf("0. Выход\n");
        printf("Выберите действие: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода. Попробуйте снова.\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1: {
                int id, capacity, neighbor_count;
                printf("ID нового отделения: ");
                scanf("%d", &id);
                printf("Вместимость: ");
                scanf("%d", &capacity);
                printf("Количество соседних отделений: ");
                scanf("%d", &neighbor_count);
                
                int neighbors[100];
                if (neighbor_count > 0) {
                    printf("Введите ID соседних отделений через пробел: ");
                    for (int i = 0; i < neighbor_count; i++) {
                        scanf("%d", &neighbors[i]);
                    }
                }
                
                StatusCode status = post_office_add(system, id, capacity, neighbors, neighbor_count);
                if (status == SUCCESS) {
                    printf("Отделение %d успешно добавлено\n", id);
                } else {
                    printf("Ошибка при добавлении отделения\n");
                }
                break;
            }
            
            case 2: {
                int id;
                printf("ID отделения для удаления: ");
                scanf("%d", &id);
                
                StatusCode status = post_office_remove(system, id);
                if (status == SUCCESS) {
                    printf("Отделение %d успешно удалено\n", id);
                } else {
                    printf("Ошибка при удалении отделения\n");
                }
                break;
            }
            
            case 3: {
                char type[20], tech_data[100];
                int priority, from_office, to_office, letter_id;
                
                printf("Тип письма (ordinary/urgent/express): ");
                scanf("%s", type);
                printf("Приоритет: ");
                scanf("%d", &priority);
                printf("Отделение отправителя: ");
                scanf("%d", &from_office);
                printf("Отделение получателя: ");
                scanf("%d", &to_office);
                printf("Технические данные: ");
                scanf("%s", tech_data);
                
                StatusCode status = letter_add(system, type, priority, from_office, to_office, tech_data, &letter_id);
                if (status == SUCCESS) {
                    printf("Письмо %d успешно добавлено\n", letter_id);
                } else {
                    printf("Ошибка при добавлении письма\n");
                }
                break;
            }
            
            case 4: {
                int letter_id;
                printf("ID письма для отметки как недоставленного: ");
                scanf("%d", &letter_id);
                
                StatusCode status = letter_mark_undelivered(system, letter_id);
                if (status == SUCCESS) {
                    printf("Письмо %d помечено как недоставленное\n", letter_id);
                } else {
                    printf("Ошибка при отметке письма\n");
                }
                break;
            }
            
            case 5: {
                int letter_id, office_id, success;
                printf("ID письма: ");
                scanf("%d", &letter_id);
                printf("ID отделения: ");
                scanf("%d", &office_id);
                
                StatusCode status = letter_try_take(system, letter_id, office_id, &success);
                if (status == SUCCESS) {
                    if (success) {
                        printf("Письмо %d успешно доставлено в отделение %d\n", letter_id, office_id);
                    } else {
                        printf("Письмо %d не может быть доставлено в отделение %d\n", letter_id, office_id);
                    }
                } else {
                    printf("Ошибка при попытке доставки\n");
                }
                break;
            }
            
            case 6: {
                char filename[100];
                printf("Имя файла для сохранения: ");
                scanf("%s", filename);
                
                StatusCode status = letters_print_all(system, filename);
                if (status == SUCCESS) {
                    printf("Список писем сохранен в %s\n", filename);
                } else {
                    printf("Ошибка при сохранении списка писем\n");
                }
                break;
            }
            
            case 7: {
                print_system_status(system);
                break;
            }
            
            case 8: {
                int cycles;
                printf("Максимальное количество циклов обработки: ");
                scanf("%d", &cycles);
                automatic_processing(system, cycles);
                break;
            }
            
            case 0:
                printf("Выход из программы...\n");
                break;
                
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
                break;
        }
        
    } while (choice != 0);
}

void print_usage(const char *program_name) {
    printf("Использование: %s <файл_конфигурации> [лог_файл]\n", program_name);
    printf("  файл_конфигурации - файл с связями между отделениями (формат: id1 id2)\n");
    printf("  лог_файл          - необязательный файл для логов (по умолчанию: post_system.log)\n");
}

int main(int argc, char *argv[]) {
    printf("=== СИСТЕМА ПОЧТОВОЙ ДОСТАВКИ ===\n\n");

    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char *config_file = argv[1];
    const char *log_file = (argc > 2) ? argv[2] : "post_system.log";
    
    signal(SIGINT, handle_signal);

    printf("Инициализация почтовой системы...\n");
    PostSystem system;
    StatusCode status = post_system_create(&system, log_file);
    if (status != SUCCESS) {
        printf("Ошибка при создании системы\n");
        return 1;
    }

    status = read_network_config(&system, config_file, 5);
    if (status != SUCCESS) {
        printf("Ошибка при чтении конфигурации\n");
        post_system_delete(&system);
        return 1;
    }
    
    printf("Система успешно инициализирована. Логирование в %s\n", log_file);

    interactive_menu(&system);
    
    printf("Завершение работы...\n");
    post_system_delete(&system);
    
    return 0;
}