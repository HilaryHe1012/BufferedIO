#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

FILE fname_in;
FILE fname_out;
int fd_in = -1;
int fd_out = -1;
char buffer_in[16];
char buffer_out[16];
int readIdx = 0;
int inserted = 0;

char buf_in();
void buf_out(char data);
void buf_flush();


char buf_in() {
    int endchecker;
    // checks emptiness
    if (buffer_in[0] == '\0') endchecker = read(fd_in, &buffer_in, 16);
    char toReturn = buffer_in[readIdx];
    //check if the read index reaches the end of the buffer_in and read another 16 bytes
    if (readIdx == 15) {
        readIdx = 0;
        endchecker = read(fd_in, &buffer_in, 16);
    } else readIdx++;
    if (endchecker < 16) buffer_in[endchecker] = EOF;
    return toReturn;
}

void buf_out(char data) {
    buffer_out[inserted] = data;  
    // buffer_out is full
    if (inserted == 15) {
        // resets inserted after writing all contents into the files
        inserted = 0;
        write(fd_out, &buffer_out, 16);
    } else inserted++;    
}

void buf_flush() {
    // write unfilled buffer_out, writing per character to avoid null to be written into files
    for (int i = 0; i < inserted; i++) {
        write(fd_out, &buffer_out[i], 1);
    }
    inserted = 0; // resets inserted
}

int main(int argc, char *argv[]) {

    fd_in = open("foo.txt", O_RDONLY);
    fd_out = open(argv[1], O_WRONLY);

    while (buffer_in[readIdx] != EOF) {
        char b = buf_in();
        putchar(b);
        buf_out(b);
    }
    buf_flush();
    close(fd_in);
    close(fd_out);
    return 0;
}
