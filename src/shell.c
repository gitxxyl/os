#include <stdbool.h>
#include <lib/string.h>
#include <lib/printf.h>
#include <lib/alloc.h>
#include <include/cmos.h>
#include <include/timer.h>
#include <thirdparty/stivale2.h>
#include <lib/assert.h>
#include <include/vfs.h>
#include <include/pci.h>
#include <include/init.h>
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
    if(!strcmp(argc, "echo")){
        if(strlen(tmp) > strlen(argc) + 1){
            printf("%s", tmp + strlen(argc) + 1);
        }
    }
    else if (!strcmp(argc, "lspci")){
        pci_device_header_t* device_header = pci_enumerate(NULL, 0x8086, NULL);
        assert(device_header != NULL);
        printf("PCI DEVICE: %s / %s / %s / %s / %s\n",
            GetVendorName(device_header->vendor_id),
            GetDeviceName(device_header->vendor_id, device_header->device_id),
            DeviceClasses[device_header->class],
            GetSubclassName(device_header->class, device_header->subclass),
            GetProgIFName(device_header->class, device_header->subclass, device_header->progif));
    }
    else if (!strcmp(argc, "clear")){
        // fb_changebg(0x00);
        tga_header_t* tga_header = (tga_header_t*) wpaper;
        dprintf("h = %d, w = %d\n", tga_header->h, tga_header->w);
        display_bmp(0, 0, tga_header->h, tga_header->w, wpaper, true);
    }
    else if (!strcmp(argc, "time")){
        uint64_t ticks = get_ticks();
        printf("Current date and time: %s\n", rtc_datetime());
        printf("%dms, or %d.%ds have passed since startup.", ticks, ticks / 1000, (ticks / 10) % 100);
    }
    else if (!strcmp(argc, "cat")){
        cat(split(tmp, ' ')[1]);
    }
    else if (!strcmp(argc, "tga")){
        tga(split(tmp, ' ')[1]);
    }
    else if (!strcmp(argc, "ls")){
        fs_node_t* node = root;
        uint64_t i = 0;
        while(node != 0){
            if(node->flags & 0x7 == FS_DIRECTORY){
                printf("/%s ", node->name);
            }
            else{
                printf("%s ", node->name);
            }
            node = readdir_fs(root, i++);
        }
    }
    else if (!strcmp(argc, "help")){
        printf("Commands:\n====================\n");
        printf("clear\n");
        printf("time\n");
        printf("echo\n");
        printf("ls\n");
        printf("lspci\n");
        printf("cat <filename>\n");
        printf("tga <filename>\n");
        printf("help\n====================");
    }
    else{
        fs_node_t* node = finddir_fs(root, argc);
        if(node == 0){
            printf("%s: File or command not found", argc);
        }
        else if(node->flags & 0x7 == FS_DIRECTORY){
            printf("%s: is a directory", argc);
            return;
        }
        else {
            printf("%s: is a file", argc);
        }
    }
    kfree(tmp);
    printf_c(0xFF00FF00, "\nksh> ");
}

void shell_init(struct stivale2_struct *stivale2_struct){
    fs_node_t* tga = finddir_fs(root, "wpaper.tga");
    assert(tga != NULL);
    wpaper = kmalloc(tga->length);
    read_fs(tga, wpaper, tga->length, 0);
    tga_header_t* tga_header = (tga_header_t*) wpaper;
    dprintf("h = %d, w = %d\n", tga_header->h, tga_header->w);

    display_bmp(0, 0, tga_header->h, tga_header->w, wpaper, true);
    // fb_clear();


    printf_c(0xFF00FF00, "\n\n[REDACTED]OS v0.3 booted successfully on Limine v%s\n", stivale2_struct->bootloader_version);
    printf_c(0xFF00FF00, "Type help for a list of commands.\n");
    printf_c(0xFF00FF00, "\nksh> ");
}