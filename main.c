#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void wrong_commands();
void get_filename(char* path, char* output);

int main(int argc, char* argv[]) {
    if (argc != 3 && argc != 4) {
       wrong_commands(); 
       return EXIT_FAILURE;
    }
    char* input = argv[1];
    char* output = argv[2];
    size_t output_name_len;
    if (argc == 4) {
        output_name_len = strlen(argv[3]) + 1;
    } else {
        output_name_len = strlen(output) + 1;
    }
    char output_name[output_name_len];
    if (argc == 4) {
        strcpy(output_name, argv[3]);
    } else {
        get_filename(output, output_name);
    }

    FILE* input_f = fopen(input, "r");
    if (!input_f) {
        fprintf(stderr, "File could not be opened for read access. File may not exist.\n");
        return EXIT_FAILURE;
    }
    fseek(input_f, 0, SEEK_END);  
    int bytes = ftell(input_f);
    fseek(input_f, 0, SEEK_SET);
    unsigned char file_bytes[bytes];
    fread(file_bytes, 1, bytes, input_f);
    fclose(input_f);
    
    FILE* output_f = fopen(output, "w");
    if (!output_f) {
        fprintf(stderr, "Output File could not be creted.\n");
        return EXIT_FAILURE;
    }
    char beginning[] = "unsigned char ";
    char beginning_next[] = "[] = {";
    char end[] = "};\nunsigned long ";
    char end_mid[] = "_length = ";
    char end_end[] = ";\n";
    char len_str[32];
    int res;
    res = sprintf(len_str, "%d", bytes);
    if (res <= 0) {
        fprintf(stderr, "Couldnt get length of bytes\n");
        return EXIT_FAILURE;
    }
    char output_str[strlen(beginning) + (strlen(output_name) * 2) + (bytes * 4) + strlen(end) + strlen(end_end) + 3 + strlen(len_str)];
    strcpy(output_str, beginning);
    strcat(output_str, output_name);
    strcat(output_str, beginning_next);
    unsigned long ind = strlen(output_str);
    char buf[4];
    for (size_t i = 0; i < bytes; i++) {
        int byte = file_bytes[i];
        res = sprintf(buf, "%d",byte);
        if (res <= 0) {
            fprintf(stderr, "Error writing bytes to file\n");
            return EXIT_FAILURE;
        }
        for (int j = 0; j < res; j++) {
            output_str[ind++] = buf[j]; 
        }
        if (i + 1 != bytes) {
            output_str[ind++] = ',';
        }
    }
    strcat(output_str, end);
    strcat(output_str, output_name);
    strcat(output_str, end_mid);
    strcat(output_str, len_str);
    strcat(output_str, end_end);
    fwrite(output_str, strlen(output_str), 1, output_f);
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
    if (output_pos > 2 && strcmp(output + output_pos - 2, ".h") == 0) {
        output[len - 2] = '\0';
    }
}

void wrong_commands() {
    printf("Your use of this program is incorrect\nCorrect format is as such: 'f2c example.png output.h' or 'f2c example.png output.h variable_name'\n");
}
