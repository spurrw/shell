#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define TOKEN_COUNT 10
#define TOKEN_SIZE 80

int main () {

	char *prompt = "prompt$ "; //TODO make this dynamic with pwd

	// main execution and input loop
	while (1) {
		// init input string and get a user input
		char input[TOKEN_COUNT * TOKEN_SIZE];
		printf("%s", prompt);
		fgets(input, TOKEN_COUNT * TOKEN_SIZE, stdin);
		// remove trailing '\n'
		input[strcspn(input, "\n")] = '\0';
		printf("the input was %s\n", input); // TODO debug delete

		// test for different inputs
		// 'exit' shell -> break loop and quit
		if (strncmp(input, "exit", 4) == 0) {
			break;
		}
		// TODO add checks for showpid and cd
		
		// command -> fork -> execvp & waitpid -> return
		else {
			pid_t pid = fork();
			if (pid == 0) {
				printf("I am child process with pid %i\n", pid);
				sleep(2);
				break;
			}
			else {
				waitpid(-1, NULL, 0);
				printf("I am parent process with child pid %i and am done waiting\n", pid);
			}
		} // END command if statement
		
	} // END while

	return 0;
}
