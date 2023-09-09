#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int create_leading_word_cross(const char *first, const char *second, char ***result);
int create_double_leading_word_cross(
        const char *first, const char *second, const char *third, const char *fourth,
        char ***result, char ***first_cross, char ***second_cross);
void destroy(char **words);

int create_leading_word_cross(const char *first, const char *second, char ***result) {
    if(first == NULL || second == NULL || result == NULL) {
        return -1;
    }

    int index_of_letter_first = -1, index_of_letter_second = -1;

    for(unsigned int i = 0; i < strlen(first); i++) {
        if(!isupper(*(first + i))) {
            *result = NULL;
            return -1;
        }
    }

    for(unsigned int i = 0; i < strlen(second); i++) {
        if(!isupper(*(second + i))) {
            *result = NULL;
            return -1;
        }
    }

    for(int i = 0; *(second + i) != '\0'; i++) {
        if(index_of_letter_first != -1) break;

        for(int j = 0; *(first + j) != '\0'; j++) {
            if(*(second + i) == *(first + j)) {
                index_of_letter_first = j;
                index_of_letter_second = i;
                break;
            }
        }
    }

    if(index_of_letter_first == -1) {
        *result = NULL;
        return -2;
    }

    *result = calloc(strlen(first) + 1, sizeof(char *));
    if(*result == NULL) {
        return -3;
    }

    for(unsigned int i = 0; i < strlen(first); i++) {
        if(i == (unsigned int)index_of_letter_first) *((*result) + i)  = calloc(strlen(second) + 1, sizeof(char));
        else *((*result) + i) = calloc((index_of_letter_second + 2),  sizeof(char));

        if(*((*result) + i) == NULL) {
            for(unsigned int j = 0; j < i; j++) {
                free(*((*result) + j));
                *((*result) + j) = NULL;
            }
            free(*result);
            *result = NULL;
            return -3;
        }
    }

    int row = 0, col = 0;

    while(*((*result) + row) != NULL) {
        if(row == index_of_letter_first) {
            *(*((*result) + row) + col) = ' ';
            col++;
            if((unsigned int)col == strlen(second)) {
                col = 0;
                row++;
            }
        }
        else {
            *(*((*result) + row) + col) = ' ';
            col++;
            if(col == (index_of_letter_second + 1)) {
                col = 0;
                row++;
            }
        }
    }

    int index_word_first = 0, index_word_second, index_word_cross_y = 0, index_word_cross_x = index_of_letter_second;

    while((unsigned int)index_word_cross_y != strlen(first)) {
        *(*((*result) + index_word_cross_y) + index_word_cross_x) = *(first + index_word_first);
        index_word_cross_y++;
        index_word_first++;
    }

    index_word_second = 0, index_word_cross_y = index_of_letter_first, index_word_cross_x = 0;

    while((unsigned int)index_word_cross_x != strlen(second)) {
        *(*((*result) + index_word_cross_y) + index_word_cross_x) = *(second + index_word_second);
        index_word_cross_x++;
        index_word_second++;
    }

    *((*result) + strlen(first)) = NULL;

    return index_of_letter_first;
}

int create_double_leading_word_cross(
        const char *first, const char *second, const char *third, const char *fourth,
        char ***result, char ***first_cross, char ***second_cross) {
    if(first == NULL || second == NULL || third == NULL || fourth == NULL ||
       result == NULL || first_cross == NULL || second_cross == NULL) return 1;

    int horizontal_row_first, horizontal_row_second;

    int index_of_letter_first_first = -1, index_of_letter_second_first = -1, index_of_letter_first_second = -1,
            index_of_letter_second_second = -1;

    for(int i = 0; *(first + i) != '\0'; i++) {
        if(index_of_letter_first_first != -1 || index_of_letter_second_first != -1) break;

        for(int j = 0; *(second + j) != '\0'; j++) {
            if(*(second + j) == *(first + i)) {
                index_of_letter_first_first = i;
                index_of_letter_second_first = j;
                break;
            }
        }
    }

    for(int i = 0; *(third + i) != '\0'; i++) {
        if(index_of_letter_first_second != -1 || index_of_letter_second_second != -1) break;

        for(int j = 0; *(fourth + j) != '\0'; j++) {
            if(*(fourth + j) == *(third + i)) {
                index_of_letter_first_second = i;
                index_of_letter_second_second = j;
                break;
            }
        }
    }

    horizontal_row_first = create_leading_word_cross(second, first, first_cross);
    if(horizontal_row_first == -1 || horizontal_row_first == -2 || horizontal_row_first == -3) {
        *result = NULL;
        *first_cross = NULL;
        *second_cross = NULL;
        if(horizontal_row_first == -1) return 1;
        else if(horizontal_row_first == -2) return 2;
        else return 3;
    }

    horizontal_row_second = create_leading_word_cross(fourth, third, second_cross);
    if(horizontal_row_second == -1 || horizontal_row_second == -2 || horizontal_row_second == -3) {
        destroy(*first_cross);
        *result = NULL;
        *second_cross = NULL;
        if(horizontal_row_second == -1) return 1;
        else if(horizontal_row_second == -2) return 2;
        else return 3;
    }

    int index_y_cross_first = -1, index_y_cross_second = -1;

    for(int i = 0; *(first + i) != '\0'; i++) {
        if(index_y_cross_first != -1) break;

        for(int j = 0; *(second + j) != '\0'; j++) {
            if(*(second + j) == *(first + i)) {
                index_y_cross_first = j;
                break;
            }
        }
    }

    for(int i = 0; *(third + i) != '\0'; i++) {
        if(index_y_cross_second != -1) break;

        for(int j = 0; *(fourth + j) != '\0'; j++) {
            if(*(fourth + j) == *(third + i)) {
                index_y_cross_second = j;
                break;
            }
        }
    }

    unsigned int height, first_cross_height = 0, second_cross_height = 0, mut_level_max;

    if(horizontal_row_first > horizontal_row_second) height = horizontal_row_first;
    else height = horizontal_row_second;

    mut_level_max = height;


    int row = 0;

    while(*((*first_cross) + row) != NULL) {
        first_cross_height++;
        row++;
    }

    row = 0;

    while(*((*second_cross) + row) != NULL) {
        second_cross_height++;
        row++;
    }

    unsigned int diff_height_first, diff_height_second, diff;

    diff_height_first = first_cross_height - horizontal_row_first;
    diff_height_second = second_cross_height - horizontal_row_second;

    if(diff_height_first > diff_height_second) diff = diff_height_first;
    else diff = diff_height_second;
    height += diff;

    *result = calloc(height + 1,sizeof(char *));
    if(*result == NULL) {
        destroy(*first_cross);
        destroy(*second_cross);
        return 3;
    }

    unsigned int mut_level_start = 0;

    if(strlen(first) != 1 && strlen(third) != 1) {
        if(horizontal_row_first == horizontal_row_second) {
            mut_level_start = 0;
        }
        else {
            mut_level_start = abs(index_y_cross_first - index_y_cross_second);
        }
    }
    else if(strlen(first) == 1 || strlen(third) == 1) {
        if(index_y_cross_second > index_y_cross_first) {
            mut_level_max = index_y_cross_second;
            mut_level_start = index_of_letter_second_second - index_of_letter_second_first;
        }
        else if(index_y_cross_first > index_y_cross_second) {
            mut_level_max = index_y_cross_first;
            mut_level_start = index_of_letter_second_first - index_of_letter_second_second;
        }
        else if(index_y_cross_first == index_y_cross_second) {
            mut_level_max = 0;
            mut_level_start = 0;
        }
    }

    int second_word_index = 0, fourth_word_index = 0;

    for (unsigned int i = 0; i < height; i++) {
        unsigned int size;

        if (i == mut_level_max) {
            size = strlen(first) + 3 + strlen(third) + 1;
            *((*result) + i) = calloc(size, sizeof(char));
        }
        else {
            if(i < mut_level_start) {
                if(horizontal_row_first > horizontal_row_second) {
                    size = index_of_letter_first_first + 2;
                    *((*result) + i) = calloc(size, sizeof(char));
                }
                else if(horizontal_row_second > horizontal_row_first) {
                    size = strlen(first) + 3 + index_of_letter_first_second + 2;
                    *((*result) + i) = calloc(size, sizeof(char));;
                }
            }
            else if(i >= mut_level_start && i != mut_level_max && second_word_index < ((int)strlen(second))
                    && fourth_word_index < ((int)strlen(fourth))) {
                size = strlen(first) + 3 + index_of_letter_first_second + 2;
                *((*result) + i) = calloc(size, sizeof(char));
            }
            else {
                if(fourth_word_index < ((int)strlen(fourth))) {
                    size = strlen(first) + 3 + index_of_letter_first_second + 2;
                    *((*result) + i) = calloc(size, sizeof(char));
                }
                else if(second_word_index < ((int)strlen(second))) {
                    size = index_of_letter_first_first + 2;
                    *((*result) + i) = calloc(size, sizeof(char));
                }
            }
        }

        if (*((*result) + i) == NULL) {
            for (unsigned int j = 0; j < i; j++) {
                free(*((*result) + j));
                *((*result) + j) = NULL;
            }
            free(*result);
            *result = NULL;
            destroy(*first_cross);
            destroy(*second_cross);
            return 3;
        }
        else {
            if (i == mut_level_max) {
                strcat(*((*result) + i), first);
                strcat(*((*result) + i), "   ");
                strcat(*((*result) + i), third);
                fourth_word_index++;
                second_word_index++;
            }
            else if(i < mut_level_start) {
                if(horizontal_row_first > horizontal_row_second) {
                    unsigned int col = 0;

                    for(int j = 0; j < index_of_letter_first_first; j++) {
                        *(*((*result) + i) + col) = ' ';
                        col++;
                    }

                    *(*((*result) + i) + col) = *(second + second_word_index);
                    second_word_index++;
                }
                else if(horizontal_row_second > horizontal_row_first) {
                    unsigned int col = 0;

                    for(unsigned int j = 0; j < (strlen(first) + 3 + index_of_letter_first_second); j++) {
                        *(*((*result) + i) + col) = ' ';
                        col++;
                    }

                    *(*((*result) + i) + col) = *(fourth + fourth_word_index);
                    fourth_word_index++;
                }
            }
            else if(i >= mut_level_start && i != mut_level_max && second_word_index < ((int)strlen(second))
                    && fourth_word_index < ((int)strlen(fourth))) {
                unsigned int col = 0;

                for(int j = 0; j < index_of_letter_first_first; j++) {
                    *(*((*result) + i) + col) = ' ';
                    col++;
                }

                *(*((*result) + i) + col) = *(second + second_word_index);
                col++;
                second_word_index++;

                for(unsigned int j = 0; j < (strlen(first) - index_of_letter_first_first + 3 + index_of_letter_first_second - 1); j++) {
                    *(*((*result) + i) + col) = ' ';
                    col++;
                }

                *(*((*result) + i) + col) = *(fourth + fourth_word_index);
                fourth_word_index++;
            }
            else {
                if(fourth_word_index < ((int)strlen(fourth))) {
                    unsigned int col = 0;

                    for(unsigned int j = 0; j < (strlen(first) + 3 + index_of_letter_first_second); j++) {
                        *(*((*result) + i) + col) = ' ';
                        col++;
                    }

                    *(*((*result) + i) + col) = *(fourth + fourth_word_index);
                    fourth_word_index++;
                }
                else if(second_word_index < ((int)strlen(second))) {
                    unsigned int col = 0;

                    for(int j = 0; j < index_of_letter_first_first; j++) {
                        *(*((*result) + i) + col) = ' ';
                        col++;
                    }

                    *(*((*result) + i) + col) = *(second + second_word_index);
                    second_word_index++;
                }
            }
        }
    }


    *((*result) + height) = NULL;

    return 0;
}

void destroy(char **words) {
    if(words == NULL) return;

    int row = 0;
    while(*(words + row) != NULL) {
        free(*(words + row));
        *(words + row) = NULL;
        row++;
    }

    free(words);
    words = NULL;
}

int main(void) {
    char *word1, *word2, *word3, *word4;

    word1 = calloc(11, sizeof(char));
    if(word1 == NULL) {
        printf("Failed to allocate memory");
        return 8;
    }

    word2 = calloc(11, sizeof(char));
    if(word2 == NULL) {
        free(word1);
        word1 = NULL;
        printf("Failed to allocate memory");
        return 8;
    }

    word3 = calloc(11, sizeof(char));
    if(word3 == NULL) {
        free(word1);
        word1 = NULL;
        free(word2);
        word2 = NULL;
        printf("Failed to allocate memory");
        return 8;
    }

    word4 = calloc(11, sizeof(char));
    if(word4 == NULL) {
        free(word1);
        word1 = NULL;
        free(word2);
        word2 = NULL;
        free(word3);
        word3 = NULL;
        printf("Failed to allocate memory");
        return 8;
    }

    char c;

    printf("Please input four words after spaces:");
    scanf("%10s %10s %10s %10s", word1, word2, word3, word4);
    while((c = (char)getchar()) && c != EOF && c != '\n');

    char **first_cross, **second_cross, **third_cross;

    int res;

    res = create_double_leading_word_cross(word1, word2, word3, word4, &third_cross, &first_cross, &second_cross);
    if(res == 2) {
        printf("Unable to make two crosses");
        free(word1);
        word1 = NULL;
        free(word2);
        word2 = NULL;
        free(word3);
        word3 = NULL;
        free(word4);
        word4 = NULL;
        return 0;
    }
    else if(res == 1) {
        printf("Incorrect input");
        free(word1);
        word1 = NULL;
        free(word2);
        word2 = NULL;
        free(word3);
        word3 = NULL;
        free(word4);
        word4 = NULL;
        return 1;
    }
    else if(res == 3) {
        free(word1);
        word1 = NULL;
        free(word2);
        word2 = NULL;
        free(word3);
        word3 = NULL;
        free(word4);
        word4 = NULL;
        printf("Failed to allocate memory");
        return 8;
    }

    int row = 0, col = 0;

    while(*(first_cross + row) != NULL) {
        printf("%c", *(*(first_cross + row) + col));
        col++;
        if (*(*(first_cross + row) + col) == '\0') {
            printf("\n");
            row++;
            col = 0;
        }
    }

    row = 0, col = 0;

    printf("\n");

    while(*(second_cross + row) != NULL) {
        printf("%c", *(*(second_cross + row) + col));
        col++;
        if (*(*(second_cross + row) + col) == '\0') {
            printf("\n");
            row++;
            col = 0;
        }
    }

    row = 0, col = 0;

    printf("\n");

    while(*(third_cross + row) != NULL) {
        printf("%c", *(*(third_cross + row) + col));
        col++;
        if (*(*(third_cross + row) + col) == '\0') {
            printf("\n");
            row++;
            col = 0;
        }
    }

    destroy(first_cross);
    destroy(second_cross);
    destroy(third_cross);
    free(word1);
    word1 = NULL;
    free(word2);
    word2 = NULL;
    free(word3);
    word3 = NULL;
    free(word4);
    word4 = NULL;

    return 0;
}
