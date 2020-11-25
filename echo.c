#include "headers.h"
#include "echo.h"

void echo (char * commands) {
	char space[2] = " ";
	while (commands) {
		printf("%s ", commands);
		commands = strtok(NULL, space);
	}
	printf("\n");
}