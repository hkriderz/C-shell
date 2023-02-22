#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string>
#include <iostream>

using namespace std;

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported
#define GetCurrentDir getcwd


// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

// Greeting shell during startup
void start_shell() {
    clear();
    cout << "\n\n\n\n------------------------------------------";
    cout << "\n|                Welcome                 |";
    cout << "\n------------------------------------------";
    char* username = getenv("USER");
    printf("\n\n\n\tUSER: %s", username);
    cout << '\n';
    sleep(1);
    clear();
}

// Function to take input
int takeInput(char* str) {
    char* buf;
    string input;
    cout << "\n>>> ";
    getline(cin, input);
    buf = const_cast<char *>(input.c_str());
    if (strlen(buf) != 0) {
        strcpy(str, buf);
        return 0;
    } else {
        return 1;
    }
}

// print current working directory.
void printDir() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    cout << "\nDir: " << cwd;
}

// Help command
void openHelp() {
    puts("\n   WELCOME TO THE SHELL HELP   \n"
         "-------------------------------\n"
         "\t1. cd: Change directory\n"
         "\t2. pwd: Present Working directory\n"
         "\t3. mkdir: Make a directory (Alerts if already exists)\n"
         "\t4. rmdir: Remove the directory (Alerts if no such file or directory)\n"
         "\t5. ls: List contents of pwd\n"
         "\t6. cp: Copy contents from one file to another\n"
         "\t7. exit: Exit the shell\n"
         "\t8. exec: Run executables\n"
         "\t9. help: Display this help window. Congrats you already know how to get here\n");
}

// execute the command
int executeCommand(char **parsed) {
    int NoOfOwnCmds = 9, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];

    ListOfOwnCmds[0] = const_cast<char *>("cd");
    ListOfOwnCmds[1] = const_cast<char *>("pwd");
    ListOfOwnCmds[2] = const_cast<char *>("mkdir");
    ListOfOwnCmds[3] = const_cast<char *>("rmdir");
    ListOfOwnCmds[4] = const_cast<char *>("ls");
    ListOfOwnCmds[5] = const_cast<char *>("cp");
    ListOfOwnCmds[6] = const_cast<char *>("exit");
    ListOfOwnCmds[7] = const_cast<char *>("exec");
    ListOfOwnCmds[8] = const_cast<char *>("help");


    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    if(switchOwnArg == 1){
        //cd
        if(chdir(parsed[1]) < 0) {
            cout << "\nCould not change directory..";
        }
        return 1;
    }else if(switchOwnArg == 2){
        //pwd
        char buff[FILENAME_MAX];
        GetCurrentDir(buff, FILENAME_MAX );
        string current_working_dir(buff);
        cout << current_working_dir;
        return 1;
    }else if(switchOwnArg == 3){
        //mkdir
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));

        if(chdir(parsed[1]) > 0) {
            cout << "\nCould not create directory. *Already Exists*\n";
            chdir(cwd);
        }else if(mkdir(parsed[1], 0777) < 0) {
            printf("\nFailed for make new directory: %s", parsed[1]);
        }
        return 1;

    }else if(switchOwnArg == 4){
        //rmdir
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));

        if(chdir(parsed[1]) < 0) {
            cout << "\nCould not delete directory. *Directory Doesn't Exists*\n";
            chdir(cwd);
        }else{
            chdir(cwd);
            int removed = rmdir(parsed[1]);
            if(removed < 0)
                cout << "\nCould not remove directory.." << '\n';
            else
                cout << "\nRemoved: " << parsed[1] << '\n';
        }
        return 1;
    }else if(switchOwnArg == 5){
        //ls
        //https://www.tutorialspoint.com/How-can-I-get-the-list-of-files-in-a-directory-using-C-Cplusplus
        char command[3] = "ls";
        system(command);
    }else if(switchOwnArg == 6){
        //cp
        FILE *fptr1, *fptr2;
        char ch;
        fptr1 = fopen(parsed[1], "r");
        if (fptr1 == NULL) {
            printf("Cannot open file %s \n", parsed[1]);
            return 1;
        }

        fptr2 = fopen(parsed[2], "w");
        if (fptr2 == NULL) {
            printf("Cannot open file %s \n", parsed[2]);
            return 1;
        }

        ch = fgetc(fptr1);
        while (ch != EOF) {
            fputc(ch, fptr2);
            ch = fgetc(fptr1);
        }

        printf("\nContents copied to %s", parsed[2]);

        fclose(fptr1);
        fclose(fptr2);

        return 1;
    }else if(switchOwnArg == 7){
        //exit
        printf("\nThank you for using our shell.\n");
        exit(0);
    }else if(switchOwnArg == 8){
        //exec
        try {
            system(parsed[1]);
        }catch(char* parsed[1]){
            cout << parsed[1] << "\t is not a valid executable.\n";
        }
        return 1;
    }else if(switchOwnArg == 9){
        //help
        openHelp();
        return 1;
    }

    return 0;
}

// function to parse by spaces
void parseSpace(char* str, char** parsed) {
    for (int i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");
        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int main() {
    char inputString[MAXCOM];
    char* parsedArgs[MAXLIST];
    start_shell();

    while (1) {
        // print current shell directory
        printDir();

        if (takeInput(inputString))
            continue;

        // process
        parseSpace(inputString, parsedArgs);
        executeCommand(parsedArgs);
    }
}