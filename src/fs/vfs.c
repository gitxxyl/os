#include <include/vfs.h>

fs_node_t* root = 0;

uint64_t read_fs(fs_node_t* node, uint8_t* buffer, uint64_t size, uint64_t offset) {
    if(node->read)
        return node->read(node, buffer, size, offset);
    else
        return 0;
}
uint64_t write_fs(fs_node_t* node, uint8_t* buffer, uint64_t size, uint64_t offset) {
    if(node->write)
        return node->write(node, buffer, size, offset);
    else
        return 0;
}
uint64_t open_fs(fs_node_t* node, uint32_t flags) {
    if(node->open)
        return node->open(node);
    else
        return 0;
}
uint64_t close_fs(fs_node_t* node) {
    if(node->close)
        return node->close(node);
    else
        return 0;
}
dirent_t* readdir_fs(fs_node_t* node, uint64_t index) {

    if(node->readdir && (node->flags&0x7) == FS_DIRECTORY)
        return node->readdir(node, index);
    else
        return 0;
}
fs_node_t* finddir_fs(fs_node_t* node, char* name) {
    if(node->finddir && (node->flags&0x7) == FS_DIRECTORY)
        return node->finddir(node, name);
    else
        return 0;
}