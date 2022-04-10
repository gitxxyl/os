/** 
 * TMPFS, or RAMFS, is a filesystem that stores all of its files directly on volatile RAM.
 * I'm implementing it now as it doesn't need a disk driver to work and has the added benefit of being very fast.
 * This is volatile though, so changes won't be preserved on restart. 
 * 
 * functions: as specified by the VFS interface
 **/

