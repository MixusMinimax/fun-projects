#include <stdio.h>
#include <stdlib.h>

#define OFFSET 128
#define MAX_STRING 1024
#define STRIDE 1

void encode(char *path, char *msg);
void decode(char *path, char *msg);

int main(int argc, char **argv)
{
	if (argc < 2 || argc > 3)
	{
		fprintf(stderr, "Usage: %s <path> [<message>]\n", argv[0]);
		printf("if you include a message, the program encodes the message into the supplied\n"
		       "file and outputs the encoded file into out.bmp, this only works with bmps.\n"
		       "if you don't supply a message, the program decodes the supplied file and\n"
		       "prints the decoded message into stdout.\n");
		return EXIT_FAILURE;
	}

	if (argc == 2)
	{
		char msg[MAX_STRING];
		decode(argv[1], msg);
		printf("%s\n", msg);
	}

	else if (argc == 3)
	{
		encode(argv[1], argv[2]);
	}

	return EXIT_SUCCESS;
}

void encode(char *path, char *msg)
{
	FILE *f_src = fopen(path, "r+");

	FILE *f_des = fopen("out.bmp", "w");

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

void decode(char *path, char *msg)
{
	FILE *fp = fopen(path, "r");

	int fileIndex = 0;

	int charOffset = 0;
	int msgIndex = 0;

	int stride = 0;

	while (!feof(fp))
	{
		char c = fgetc(fp);

		if (!stride && fileIndex >= OFFSET)
		{
			if(!charOffset)
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
