# fun-projects
Fun Projects.

## bmpcry - a tool for hiding and finding messages in bitmaps, or any other file.
* encode - write the message bit by bit into the last bit of every byte of the file. to prevent from overwriting inportant data in the The program starts writing from the 128th byte of the file.
* decode - reads the last bit from every byte, starting from the 128th byte of the file, and combine it into a string
* uses utf-8, or anything with a char size of 1 byte.
