#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


int  string_to_int(const char *text){
    int number = 0;

    for(int i = 0; text[i] != '\0'; ++i) {

        if(text[i] >= '0' && text[i] <= '9') {
            number = number * 10 + (text[i] - '0');
        }

    }
    return number;
}

bool is_digit(char c){
    return '0' <= c && c <= '9';
}

bool is_lower(char c){
    return 'a' <= c && c <= 'z';
}

bool is_upper(char c){
    return 'A' <= c && c <= 'Z';
}
bool is_special(char c){
    return 32<=c && c<=126 && !is_digit(c) && !is_lower(c) && !is_upper(c) ;
}
int string_length(const char *array){
    int size = 0;
    for (int i = 0; array[i] != '\0' ; ++i) {
        ++size;

    }
    return size;
}
bool ContainsChar(const char *str, char c)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == c)
        {
            return true;
        }
        i++;
    }
    return false;
}
bool contains_digit(const char* text){
    int i=0;
    while( text[i] != '\0') {

        if (is_digit(text[i])) {
            return 1;
        }
        ++i;
    }
    return 0;
}

bool contains_upper(const char* text){
    int i=0;
    while( text[i] != '\0') {

        if (is_upper(text[i])) {
            return 1;
        }
        ++i;
    }
    return 0;
}


bool contains_lower(const char* text){
    int i=0;
    while( text[i] != '\0') {

        if (is_lower(text[i])) {
            return 1;
        }
        ++i;
    }
    return 0;
}


bool contains_special(const char* text)
    {
        int i=0;
        while( text[i] != '\0') {

            if (is_special(text[i]) ) {
                return 1;
            }
            ++i;
        }
        return 0;
    }

bool is_equal(const char* first, const char* second){
    if(string_length(first)!= string_length(second)){
        return false;
    }
    for (int i = 0; i < string_length(first); ++i) {
        if(first[i]!=second[i]){
            return false;
        }
    }
    return true;
}
/*bool check_previous(const char* arr, size_t position){
    if (position == 0){
        return false;
    }
    else if (arr[position] == arr[position - 1]) {
        return true;
    } else {
        return false;
    }
}*/
bool contain_same_chars(const char* array, int param){
    char previous = array[0];
    int repetition = 0;
    for (int i = 1; i < string_length(array) ; ++i) {
        if (array[i] == ' ') {
            continue;
        }
        if(array[i] == previous)
         repetition++;
        else{
         repetition = 1;
         previous = array[i];
        }
        if (repetition == param) {
            return true;
        }
    }
    return false;
}
bool contains_same_substring(const char* array, int param){
    for (int i = 0; i < string_length(array) - param; ++i)
        for (int j = i + param; j < string_length(array) - param + j; ++j)
            if (is_equal(&array[i + param], &array[j]))
                return true;

    return false;
}
bool input_arguments_invalid(const int argc, const char **argv, int* level, int* param, bool* is_stats ) {
    /*for (int i = 0; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }*/
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "Invalid amount of arguments\n");

        return true;
    }
    for (int i = 0; argv[1][i] != '\0'; ++i) {
        if(!contains_digit(argv[1]) || !(argv[1][0]>='1'  && argv[1][0]<='4'))  {

            fprintf(stderr, "Level must be between 1 and 4\n");
            return true;
        }
    }

    *level = string_to_int(argv[1]);

    for(int i = 0; argv[2][i] !='\0'; ++i){
        if(!is_digit(argv[2][i])){
            fprintf(stderr,"parametr X must be a positive integer number\n" );
            return  true;
        }

    }
    *param = string_to_int(argv[2]);

    if(argc == 4){
       if( !is_equal(argv[3], "--stats")){
            fprintf(stderr, "Invalid fourth argument\n");
            return true;
        }
       else{
           *is_stats = true;
       }

    }
    else{
        *is_stats = false;
    }
    return  false;
}
bool password_check(char const *text,  int level, int param) {
    switch (level) {

        case 1:
            return (contains_lower(text) && contains_upper(text));
            break;
        case 2:
            if(param>4){
                return(password_check(text, 1, param) && (contains_upper(text) + contains_lower(text) + contains_digit(text) + contains_special(text)) >= 4);
            }
            else{
                return(password_check(text, 1, param) && (contains_upper(text) + contains_lower(text) + contains_digit(text) + contains_special(text))>= param);
            }
            break;
        case 3:
            return (password_check(text, 2, param) && !contain_same_chars(text, param));
            break;
        case 4:
            return (password_check(text, 3, param) && contains_same_substring(text, param));
            break;
        default:
            return false;
            break;
    }
}


    int main(int argc, char *argv[]) {
     const int capacity = 102;   // Maximal length of one password is 100 characters, plus 2 characters to account for \n and \0
     const int maxLength = 100;
     char  heslo[capacity]; // Array of characters to temporarily store the password that is worked with
      int level ;
      int param ;
      bool is_stats = false ;

      input_arguments_invalid(argc, (const char **) argv, &level, &param, &is_stats );



      int number_of_passwords = 0;
      bool uniqueCharacters[256] = {false};
      int numberOfUniqueCharacters = 0;
      int minimalLength = 100;
      int  lengthOfAll = 0;
      float averageLength = 0;
        while (fgets(heslo, capacity, stdin)) // Go through all the passwords, line by line
        {
            number_of_passwords++; // For every pass, raise the number of passwords

            if(string_length(heslo) > maxLength){
                fprintf(stderr,"The password has more than 100 chars\n");
                // TODO: find ending of this line
                continue;
            }
            for (int i = 0; heslo[i] != '\0' && heslo[i] != '\n'; i++) {

                uniqueCharacters[(int) heslo[i]] = true;
            }

            if (string_length(heslo) < minimalLength) // Store the shortest password
            {
                minimalLength = string_length(heslo);
            }

            lengthOfAll += string_length(heslo);
            if (password_check(heslo, level, param)) // Check the password by the specified criteria
            {
                printf("%s", heslo); // If conditions are met, print out the password
            }

        }
        for (int i = 0; i < 256; i++) // Count the unique characters by adding up all true values in the array
        {
            numberOfUniqueCharacters += uniqueCharacters[i];
        }

        averageLength = ((float )lengthOfAll / (float)number_of_passwords); // Get the average length of all passwords

        if (is_stats) // Prints the statistics at the end if enabled
        {
            printf("\n\nStatistika:\n");
            printf("Ruznych znaku: %d\n", numberOfUniqueCharacters);
            printf("Minimalni delka: %d\n", minimalLength);
            printf("Prumerna delka: %.1f\n", averageLength); // Average length is rounded up to one decimal point
        }

        return 0;
    }