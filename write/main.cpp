#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cctype>
#include <iostream>
#include <cstring>


using std::cout;
using std::endl;
using std::islower;
using std::tolower;
using std::toupper;

constexpr auto BUFFER_SIZE = 256;

int main (int argc, char *argv[]) {
    int input_fd, read_output_fd;
    ssize_t in, out, result;
    char buffer[BUFFER_SIZE];

    // Open the input and output file
    input_fd = open (argv[1], O_RDONLY);
    if (input_fd == -1) {
        cout << "Error opening file" << endl;
    }

    read_output_fd = open ("read.txt", O_WRONLY | O_CREAT);
    if (read_output_fd == -1) {
        cout << "Error opening read output" << endl;
    }

    // Read into the buffer, flip the case, then write to the new file
    while ((in = read (input_fd, &buffer, BUFFER_SIZE)) > 0) {
        for (size_t i = 0; i < strlen(buffer); ++i) {
          if (islower(buffer[i])) {
              buffer[i] = toupper(buffer[i]);
          } else {
              buffer[i] = tolower(buffer[i]);
          }
        }
        out = write (read_output_fd, &buffer, in);
        if (out != in) {
            cout << "Error occured during write" << endl;
        }
    }

    close(input_fd);
    close(read_output_fd);
}