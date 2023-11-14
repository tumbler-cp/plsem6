//
// Created by JALOL on 14.11.2023.
//

#include <inttypes.h>

#include "vector.h"

int main() {

    printf("ТЕСТ 1. Добавление элемента в конец массива\n");
    VECTOR v = vector_allocate();
    add_to_vector(v, 1);
    add_to_vector(v, 2);
    add_to_vector(v, 3);
    vector_print(v, stdout);

    printf("\nТЕСТ 2. Получение доступа к любому элементу\n");
    printf("Получение второго элемента (По индексу): %" PRId64 "\n", get_from_vector(v, 1));
    printf("Изменение третьего элемента (По индексу):");
    set_in_vector(v, 2, 555);
    vector_print(v, stdout);

    printf("\nТЕСТ 3. Добавить другой массив в конец массива: ");
    VECTOR v1 = vector_allocate();
    add_to_vector(v1, 11);
    add_to_vector(v1, 12);
    add_to_vector(v1, 13);
    add_to_vector(v1, 14);
    add_to_vector(v1, 15);
    vector_print(v, stdout);
    vector_print(v1, stdout);
    vector_to_vector(v1, v);
    vector_print(v, stdout);

    printf("\nТЕСТ 4. Изменить размер на меньший:\n");
    vector_print(v, stdout);
    vector_reallocate(v, 2);
    vector_print(v, stdout);

    printf("\nТЕСТ 5. Освободить память\n");
    vector_print(v, stdout);
    vector_free(v);
    vector_print(v, stdout);
    return 0;
}