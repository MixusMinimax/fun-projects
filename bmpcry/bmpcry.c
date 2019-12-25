#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OFFSET 128
#define MAX_STRING 1024
#define STRIDE 1


void fail(char *progname)
{
	printf("usage: %s (-help | -decode | -encode) <srcPath> [<desPath> <msg>]\n", progname);
	exit(EXIT_FAILURE);
}

void encode(char *srcPath, char *desPath, char *msg);
void decode(char *srcPath, char *msg);

int main(int argc, char **argv)
{
	if (argc == 1)
		fail(argv[0]);

	if (!strcmp(argv[1], "-help"))
	{
		printf("File encoder/decoder\nWorks best on bitmaps\nEncode:\n%s -encode <srcPath> <desPath> <msg>\nDecode:\n%s -decode <srcPath>\n", argv[0], argv[0]);
		return EXIT_SUCCESS;
	}

	else if (!strcmp(argv[1], "-encode"))
	{
		if (argc != 5)
			fail(argv[0]);

		encode(argv[2], argv[3], argv[4]);

		return EXIT_SUCCESS;
	}

	else if (!strcmp(argv[1], "-decode"))
	{
		if (argc != 3)
			fail(argv[0]);

		char msg[MAX_STRING];

		decode(argv[2], msg);
		
		printf("%s\n", msg);

		return EXIT_SUCCESS;
	}

	fail(argv[0]);
}

void encode(char *srcPath, char *desPath, char *msg)
{
	FILE *f_src = fopen(srcPath, "r+");

	FILE *f_des = fopen(desPath, "w");

	int msgIndex = 0;
	int charOffset = 0;
	int fileIndex = 0;
	int over = 0;
	int stride = 0;

	while (!feof(f_src))
	{
		char c = fgetc(f_src);

		if (!stride && !over && fileIndex >= OFFSET)
		{
			char msgc = msg[msgIndex];

			c &= 0b11111110;
			c |= (msgc >> (7 - charOffset)) & 0x1;

			++charOffset;
			if (charOffset == 8)
			{
				if (msgc == '\0')
					over = 1;
				charOffset = 0;
				++msgIndex;
			}
		}

		stride = (stride + 1) % STRIDE;

		fwrite(&c, 1, sizeof(c), f_des);

		++fileIndex;
	}
}

void decode(char *srcPath, char *msg)
{
	FILE *f_src = fopen(srcPath, "r");

	int fileIndex = 0;

	int charOffset = 0;
	int msgIndex = 0;

	int stride = 0;

	while (!feof(f_src))
	{
		char c = fgetc(f_src);

		if (!stride && fileIndex >= OFFSET)
		{
			if (!charOffset)
				msg[msgIndex] = 0;

			msg[msgIndex] |= (c & 0x1) << (7 - charOffset);

			++charOffset;

			if (charOffset == 8)
			{
				charOffset = 0;
				if (msg[msgIndex] == '\0')
					return;
				++msgIndex;
				if (msgIndex == MAX_STRING - 1)
				{
					msg[MAX_STRING - 1] = '\0';
					return;
				}
			}
		}

		stride = (stride + 1) % STRIDE;

		++fileIndex;
	}
}
