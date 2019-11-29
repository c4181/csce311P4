#include <cstdio>
#include <cstring>
#include <iostream>

using std::cout;
using std::endl;
using std::islower;
using std::tolower;
using std::toupper;

int main (int argc, char *argv[]) {
    size_t result;
    char *fbuffer;
    long lSize;

    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen("fwrite.txt", "w");
    if (input_file && output_file != nullptr) {
        // Get file size
        fseek(input_file, 0, SEEK_END);
        lSize = ftell(input_file);
        rewind(input_file);

        // Allocate memory for the entire file
        fbuffer = (char*)malloc(lSize + 1);

        // Read into the buffer
        result = fread(fbuffer, sizeof(char), lSize, input_file);
        if (result != lSize) {
            cout << "Error reading file" << endl;
        }
    }
    
    for (size_t i = 0; i < lSize; ++i) {
        if (islower(fbuffer[i])) {
            fbuffer[i] = toupper(fbuffer[i]);
        } else {
            fbuffer[i] = tolower(fbuffer[i]);
        }
    }

    fwrite(fbuffer, sizeof(char), lSize, output_file);

    fclose(input_file);
    fclose(output_file);
    free(fbuffer);
}