#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define MAX_STR_LEN 1000

// Function prototypes
void process_key(char[]);
void make_matrix(char[], const char[]);
void process_msg(char[]);
unsigned get_position(const char[], const char);
void encrypt(char[], const char[]);
void decrypt(char[], const char[]);

int main()
{
	short mode = -1;
	char key[MAX_STR_LEN + 1], matrix[26], msg[2 * MAX_STR_LEN + 1];

	printf("Select mode\n1. Encrypt\n2. Decrypt\n>>> ");
	while (mode != 1 && mode != 2)
	{
		scanf("%hd", &mode);
		if (mode != 1 && mode != 2)
			printf("Incorrect choice, try again\n>>> ");
	}
	while (getchar() != '\n') {}

	printf("Input the key (Max %d characters)\n>>> ", MAX_STR_LEN);
	(void)fgets(key, MAX_STR_LEN, stdin);
	process_key(key);
	printf("\nDistinct letters in key\n%s\n", key);

	make_matrix(matrix, key);
	printf("\nMatrix\n");
	for (unsigned char i = 0; i < 25; i += 5)
	{
		for (unsigned char j = i; j < i + 5; j++)
			putchar(matrix[j]);
		putchar('\n');
	}

	if (mode == 1)
	{
		printf("\nInput the plaintext (Max %d characters)\n>>> ", MAX_STR_LEN);
		(void)fgets(msg, MAX_STR_LEN, stdin);
		process_msg(msg);
		printf("\nProcessed plaintext\n%s\n", msg);

		encrypt(msg, matrix);
		printf("\nCiphertext\n%s\n", msg);
	}
	else
	{
		printf("\nInput the ciphertext (Max %d characters)\n>>> ", MAX_STR_LEN);
		(void)fgets(msg, MAX_STR_LEN, stdin);
		char *nl = strrchr(msg, '\n');
		if (nl != NULL)
			*nl = '\0';
		decrypt(msg, matrix);
		printf("\nPlaintext\n%s\n", msg);
	}
	return 0;
}

void process_key(char key[])
{
	unsigned j = 0;
	char tmp[MAX_STR_LEN];
	bool exist[26] = {false};
	exist['J' - 'A'] = true;

	for (unsigned i = 0; i < strlen(key); i++)
	{
		if (isalpha(key[i]))
		{
			if (toupper(key[i]) == 'J')
				key[i] = 'I';

			if (!exist[toupper(key[i]) - 'A'])
			{
				tmp[j++] = toupper(key[i]);
				exist[toupper(key[i]) - 'A'] = true;
			}
		}
	}
	tmp[j] = '\0';
	strcpy(key, tmp);
}

void make_matrix(char matrix[], const char key[])
{
	char j = 'A';
	bool exist[26] = {false};
	exist['J' - 'A'] = true;

	strcpy(matrix, key);
	for (unsigned i = 0; i < strlen(key); i++)
		if (!exist[key[i] - 'A'])
			exist[key[i] - 'A'] = true;

	for (unsigned i = strlen(key); i < 25; i++)
	{
		while (exist[j - 'A'])
			j++;
		matrix[i] = j;
		exist[j - 'A'] = true;
	}
	matrix[25] = '\0';
}

void process_msg(char msg[])
{
	unsigned i, j;
	char tmp[MAX_STR_LEN], lhs, rhs, *nl;

	if ((nl = strrchr(msg, '\n')) != NULL)
		*nl = '\0';

	for (i = 0; i < strlen(msg); i++)
	{
		if (isalpha(msg[i]))
			if (toupper(msg[i]) == 'J')
				msg[i] = isupper(msg[i]) ? 'I' : 'i';
	}

	for (i = 0; i < strlen(msg); i = j + 1)
	{
		while (!isalpha(msg[i]))
			i++;
		lhs = msg[i];

		if (i + 1 >= strlen(msg))
			break;
		j = i + 1;
		while (!isalpha(msg[j]))
			j++;
		rhs = msg[j];

		if (toupper(lhs) == toupper(rhs))
		{
			strcpy(tmp, msg + j);
			msg[j] = 'X';
			memmove(msg + j + 1, tmp, strlen(tmp) + 1);
		}
	}

	for (i = 0, j = 0; i < strlen(msg); i++)
		if (isalpha(msg[i]))
			j++;
	if (j % 2 == 1)
		strcat(msg, "X");
}

unsigned get_position(const char matrix[], const char c)
{
	int i;
	for (i = 0; i < 25; i++)
		if (toupper(c) == matrix[i])
			break;
	return i;
}

void encrypt(char msg[], const char matrix[])
{
	unsigned i, j, lhs, rhs;
	for (i = 0; i < strlen(msg); i = j + 1)
	{
		while (!isalpha(msg[i]))
			i++;
		lhs = get_position(matrix, msg[i]);

		j = i + 1;
		while (!isalpha(msg[j]))
			j++;
		rhs = get_position(matrix, msg[j]);

		// Check for each letters position in a digram
		if ((lhs % 5) == (rhs % 5))
		{
			// The letters is in the same column
			if (lhs + 5 > 24)
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs % 5] : tolower(matrix[lhs % 5]);
				msg[j] = isupper(msg[j]) ? matrix[rhs + 5] : tolower(matrix[rhs + 5]);
			}
			else if (rhs + 5 > 24)
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs + 5] : tolower(matrix[lhs + 5]);
				msg[j] = isupper(msg[j]) ? matrix[rhs % 5] : tolower(matrix[rhs % 5]);
			}
			else
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs + 5] : tolower(matrix[lhs + 5]);
				msg[j] = isupper(msg[j]) ? matrix[rhs + 5] : tolower(matrix[rhs + 5]);
			}
		}
		else if ((lhs / 5) == (rhs / 5))
		{
			// The letters is in the same row
			if ((lhs + 1) % 5 == 0)
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs - 4] : tolower(matrix[lhs - 4]);
				msg[j] = isupper(msg[j]) ? matrix[rhs + 1] : tolower(matrix[rhs + 1]);
			}
			else if ((rhs + 1) % 5 == 0)
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs + 1] : tolower(matrix[lhs + 1]);
				msg[j] = isupper(msg[j]) ? matrix[rhs - 4] : tolower(matrix[rhs - 4]);
			}
			else
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs + 1] : tolower(matrix[lhs + 1]);
				msg[j] = isupper(msg[j]) ? matrix[rhs + 1] : tolower(matrix[rhs + 1]);
			}
		}
		else
		{
			// The letters is in a different column and row
			if ((lhs % 5) > (rhs % 5))
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs - (lhs % 5 - rhs % 5)] : tolower(matrix[lhs - (lhs % 5 - rhs % 5)]);
				msg[j] = isupper(msg[j]) ? matrix[rhs + (lhs % 5 - rhs % 5)] : tolower(matrix[rhs + (lhs % 5 - rhs % 5)]);
			}
			else
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs + (rhs % 5 - lhs % 5)] : tolower(matrix[lhs + (rhs % 5 - lhs % 5)]);
				msg[j] = isupper(msg[j]) ? matrix[rhs - (rhs % 5 - lhs % 5)] : tolower(matrix[rhs - (rhs % 5 - lhs % 5)]);
			}
		}
	}
}

void decrypt(char msg[], const char matrix[])
{
	unsigned i, j, lhs, rhs;
	for (i = 0; i < strlen(msg); i = j + 1)
	{
		while (!isalpha(msg[i]))
			i++;
		lhs = get_position(matrix, msg[i]);

		j = i + 1;
		while (!isalpha(msg[j]))
			j++;
		rhs = get_position(matrix, msg[j]);

		// Check for each letters position in a digram
		if ((lhs % 5) == (rhs % 5))
		{
			// The letters is in the same column
			if (lhs < 5)
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs + 20] : tolower(matrix[lhs + 20]);
				msg[j] = isupper(msg[j]) ? matrix[rhs - 5] : tolower(matrix[rhs - 5]);
			}
			else if (rhs < 5)
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs - 5] : tolower(matrix[lhs - 5]);
				msg[j] = isupper(msg[j]) ? matrix[rhs + 20] : tolower(matrix[rhs + 20]);
			}
			else
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs - 5] : tolower(matrix[lhs - 5]);
				msg[j] = isupper(msg[j]) ? matrix[rhs - 5] : tolower(matrix[rhs - 5]);
			}
		}
		else if ((lhs / 5) == (rhs / 5))
		{
			// The letters is in the same row
			if ((lhs - 1) % 5 == 4)
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs + 4] : tolower(matrix[lhs + 4]);
				msg[j] = isupper(msg[j]) ? matrix[rhs - 1] : tolower(matrix[rhs - 1]);
			}
			else if ((rhs - 1) % 5 == 4)
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs - 1] : tolower(matrix[lhs - 1]);
				msg[j] = isupper(msg[j]) ? matrix[rhs + 4] : tolower(matrix[rhs + 4]);
			}
			else
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs - 1] : tolower(matrix[lhs - 1]);
				msg[j] = isupper(msg[j]) ? matrix[rhs - 1] : tolower(matrix[rhs - 1]);
			}
		}
		else
		{
			// The letters is in a different column and row
			if ((lhs % 5) > (rhs % 5))
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs - (lhs % 5 - rhs % 5)] : tolower(matrix[lhs - (lhs % 5 - rhs % 5)]);
				msg[j] = isupper(msg[j]) ? matrix[rhs + (lhs % 5 - rhs % 5)] : tolower(matrix[rhs + (lhs % 5 - rhs % 5)]);
			}
			else
			{
				msg[i] = isupper(msg[i]) ? matrix[lhs + (rhs % 5 - lhs % 5)] : tolower(matrix[lhs + (rhs % 5 - lhs % 5)]);
				msg[j] = isupper(msg[j]) ? matrix[rhs - (rhs % 5 - lhs % 5)] : tolower(matrix[rhs - (rhs % 5 - lhs % 5)]);
			}
		}
	}
}
