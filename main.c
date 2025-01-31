#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_filename(char* path, char* output);

int main(int argc, char* argv[]) {
    // Accepts 3 or 4 arguments
    // 4 if specifying name of variable
    if (argc != 3 && argc != 4) {
        printf("Your use of this program is incorrect\nCorrect format is as such: 'f2c example.png output.h' or 'f2c example.png output.h variable_name'\n");
        return EXIT_FAILURE;
    }
    // First is input file
    char* input = argv[1];
    // Second is output file
    char* output = argv[2];
    size_t output_name_len;
    // Size will be different depending on if variable name is specified
    if (argc == 4) {
        output_name_len = strlen(argv[3]) + 1;
    } else {
        // Maximum size it could be if not specified
        output_name_len = strlen(output) + 1;
    }
    char output_name[output_name_len];
    if (argc == 4) {
        strcpy(output_name, argv[3]);
    } else {
        get_filename(output, output_name);
    }

    // Only need read perms
    FILE* input_f = fopen(input, "r");
    if (!input_f) {
        fprintf(stderr, "File could not be opened for read access. File may not exist.\n");
        return EXIT_FAILURE;
    }
    fseek(input_f, 0, SEEK_END);  
    // Getting file size
    int bytes = ftell(input_f);
    fseek(input_f, 0, SEEK_SET);
    // Needs to be unsigned so it int conversion later doesn't try to convert from two's complement
    unsigned char file_bytes[bytes];
    fread(file_bytes, 1, bytes, input_f);
    fclose(input_f);

    // Only need write perms
    FILE* output_f = fopen(output, "w");
    if (!output_f) {
        fprintf(stderr, "Output File could not be creted.\n");
        return EXIT_FAILURE;
    }
    // Formatting for header file
    char beginning[] = "unsigned char ";
    char beginning_next[] = "[] = {";
    char end[] = "};\nunsigned long ";
    char end_mid[] = "_length = ";
    char end_end[] = ";\n";
    // Definitely big enough. 100% overkill
    char len_str[sizeof(unsigned long)];
    // This res var is used for responses later in a few places
    int res = sprintf(len_str, "%d", bytes);
    // <0 if error and 0 if no bytes written. I consider both a fail
    if (res <= 0) {
        fprintf(stderr, "Couldnt get length of bytes\n");
        return EXIT_FAILURE;
    }
    // Adding up all the header formatting text, the output_name twice, once for the unsigned char array and once for the length name.
    // bytes*4 due to a byte being 3 characters long MAX eg. 255, and the comma seperation.
    // +3 due to null term, and anything misc I may have forgotten.
    char output_str[strlen(beginning) + (strlen(output_name) * 2) + (bytes * 4) + strlen(end) + strlen(end_end) + strlen(len_str) + strlen(end_mid) + 3];
    strcpy(output_str, beginning);
    strcat(output_str, output_name);
    strcat(output_str, beginning_next);
    unsigned long ind = strlen(output_str);
    // Byte max 3 long +1 for \0
    char buf[4];
    for (size_t i = 0; i < bytes; i++) {
        // Definitely a better way to do this...I don't want to think about it.
        int byte = file_bytes[i];
        res = sprintf(buf, "%d",byte);
        if (res <= 0) {
            fprintf(stderr, "Error writing bytes to file\n");
            return EXIT_FAILURE;
        }
        for (int j = 0; j < res; j++) {
            output_str[ind++] = buf[j]; 
        }
        // Dont need the comma for the last element
        if (i + 1 != bytes) {
            output_str[ind++] = ',';
        }
    }
    strcat(output_str, end);
    strcat(output_str, output_name);
    strcat(output_str, end_mid);
    strcat(output_str, len_str);
    strcat(output_str, end_end);
    // Writing to file
    fwrite(output_str, strlen(output_str), 1, output_f);
    // Closing file
    fclose(output_f);
    return EXIT_SUCCESS;
}

void get_filename(char* path, char* output) {
    size_t len = strlen(path);
    size_t output_pos = 0;
    for (size_t i = 0; i < len; i++) {
        if (path[i] == '\\' || path[i] == '/') {
            output_pos = 0; 
        } else {
            output[output_pos++] = path[i];
        }
    }
    output[output_pos] = '\0';
    // Remove the header extension if present
    // Could use a lot more checking here. For non-allowed characters in c variable names for example. Maybe one day I'll add it.
    if (output_pos > 2 && strcmp(output + output_pos - 2, ".h") == 0) {
        output[len - 2] = '\0';
    }
}

