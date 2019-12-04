#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

using std::cout;
using std::endl;
using std::islower;
using std::tolower;
using std::toupper;

int main(int argc, char *argv[]) {
    char *read_addr;
    char *write_addr;
    int read_fd, write_fd;
    struct stat sb;
    size_t length;
    ssize_t s;

    // Open and map input file
    read_fd = open(argv[1], O_RDONLY);
    if (read_fd == -1) {
        cout << "Error opening read file" << endl;
    }

    // Get the length of the input file
    fstat(read_fd, &sb);
    length = sb.st_size;

    read_addr = static_cast<char*>(mmap(NULL, length, PROT_READ, MAP_PRIVATE, read_fd, 0));
    if(read_addr == MAP_FAILED) {
        cout << "Error creating read mmap" << endl;
    }

    // Open the output file
    write_fd = open("mmio.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (write_fd == -1) {
        cout << "Error opening write file" << endl;
    }

    // Make the output the same size as the input file
    lseek(write_fd, length - 1, SEEK_SET);
    write(write_fd, "", 1);

    // Map the output file
    write_addr = static_cast<char*>(mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, write_fd, 0));

    // Flip the case and write to the mmap
    for (size_t i = 0; read_addr[i] != '\0'; ++i) {
        if (islower(read_addr[i])) {
            write_addr[i] = toupper(read_addr[i]);
        } else {
            write_addr[i] = tolower(read_addr[i]);
        }
    }

    // Unmap the memory and close the files
    munmap(read_addr, length);
    munmap(write_addr, length);
    close(read_fd);
    close(write_fd);
}