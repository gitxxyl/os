#include <stdbool.h>
#include <lib/string.h>
#include <lib/printf.h>
#include <lib/alloc.h>
#include <include/timer.h>
#include <include/vfs.h>
#include <include/targa.h>
char cmd[256];

void shell_getchar(char c){
    if(c == '\n'){
        shell_exec(cmd);
        memset(cmd, 0, 256);
    }
    else if (c == '\b'){
        if(strlen(cmd) > 0){
            cmd[strlen(cmd) - 1] = '\0';
        }
        fb_printchar(c);
    } else {
        fb_printchar(c);
        cmd[strlen(cmd)] = c;
    }
}

void cat(char* filename){
    fs_node_t* node = finddir_fs(root, filename);
    if(node == 0){
        printf("cat: File not found\n");
        return;
    }
    if(node->flags & 0x7 == FS_DIRECTORY){
        printf("cat: %s is a directory\n", filename);
        return;
    }
    uint8_t* buffer = (uint8_t*)kmalloc(node->length);
    read_fs(node, buffer, node->length, 0);
    for(int i = 0; i < node->length; i++){
        fb_printchar(buffer[i]);
    }
    kfree(buffer);
}

void tga(char* filename){
    char* ext = split(filename, '.')[1];
    if(strcmp(ext, "tga") != 0){
        dprintf("%s, %d", ext, strcmp(ext, "tga"));
        printf("tga: File is not a TGA\n");
        return;
    }
    fs_node_t* node = finddir_fs(root, filename);
    if(node == 0){
        printf("tga: File not found\n");
        return;
    }
    char* img = kmalloc(node->length);
    read_fs(node, img, node->length, 0);
    tga_header_t* tga_header = (tga_header_t*) img;
    dprintf("h = %d, w = %d\n", tga_header->h, tga_header->w);

    display_bmp(1280 - tga_header->w * 2, 0, tga_header->h, tga_header->w, img, false);
}

extern char* wpaper; 

void shell_exec(char* cmd){
    char* tmp = kmalloc(256);
    strcpy(tmp, cmd);
    char* argc = split(tmp, ' ')[0];
    printf("\n");
    dprintf("arg: %s\n", argc);
    if(!strcmp(argc, "test")){
        printf("test");
    }
    else if (!strcmp(argc, "clear")){
        // fb_changebg(0x00);
        tga_header_t* tga_header = (tga_header_t*) wpaper;
        dprintf("h = %d, w = %d\n", tga_header->h, tga_header->w);
        display_bmp(0, 0, tga_header->h, tga_header->w, wpaper, true);
    }
    else if (!strcmp(argc, "time")){
        printf("%d ticks have passed since startup.", get_ticks());
    }
    else if (!strcmp(argc, "cat")){
        cat(split(tmp, ' ')[1]);
    }
    else if (!strcmp(argc, "tga")){
        tga(split(tmp, ' ')[1]);
    }
    else if (!strcmp(argc, "help")){
        printf("Commands:\n");
        printf("    clear\n");
        printf("    time\n");
        printf("    cat <filename>\n");
        printf("    test\n");
        printf("    help");
    }
    else{
        fs_node_t* node = finddir_fs(root, argc);
        if(node == 0){
            printf("%s: File or command not found\n", argc);
        }
        else if(node->flags & 0x7 == FS_DIRECTORY){
            printf("%s: is a directory\n", argc);
            return;
        }
        else {
            printf("%s: is a file\n", argc);
        }
    }
    kfree(tmp);
    printf_c(0xFF00FF00, "\nish> ");
}
