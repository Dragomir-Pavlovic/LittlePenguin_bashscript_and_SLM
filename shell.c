#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

#define MAX_INPUT 512
#define MAX_HISTORY 50

void bootScreen(void);
void loadingScreen(void);
void Auth(void);

void setColor() {
#ifdef _WIN32
    // Windows kod po potrebi
#else
    printf("\033[34m");  // plavi tekst, default pozadina
#endif
}

void resetColor() {
#ifdef _WIN32
    // Windows kod po potrebi
#else
    printf("\033[0m");  // reset na default
#endif
}
int shell_interface(){
	printf("████████████████████████████████████████████████████████████████████████████████\n");
	printf("█▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒█\n");
        printf("█▒   ███    ███ ██ ███    ██ ██     ███████ ██   ██ ███████ ██      ██         █▒\n");
        printf("█▒   ████  ████ ██ ████   ██ ██     ██      ██   ██ ██      ██      ██         █▒\n");
        printf("█▒   ██ ████ ██ ██ ██ ██  ██ ██     ███████ ███████ █████   ██      ██         █▒\n");
        printf("█▒   ██  ██  ██ ██ ██  ██ ██ ██          ██ ██   ██ ██      ██      ██         █▒\n");
        printf("█▒   ██      ██ ██ ██   ████ ██     ███████ ██   ██ ███████ ███████ ███████    █▒\n");
        printf("█▒                                                                             █▒\n");
        printf("█▒       ██        ███████ ██      ███    ███                                  █▒\n"); 
        printf("█▒       ██        ██      ██      ████  ████                                  █▒\n"); 
        printf("█▒   ████████      ███████ ██      ██ ████ ██                                  █▒\n");
        printf("█▒   ██  ██             ██ ██      ██  ██  ██                                  █▒\n"); 
        printf("█▒   ██████        ███████ ███████ ██      ██                                  █▒\n"); 
        printf("█▒                                                                             █▒\n");
	printf("████████████████████████████████████████████████████████████████████████████████\n");
        printf("▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n");	
	setColor();
         
        printf("█ - LAVANDER SHELL -                                               █\n");
        printf("█  To continue to shell type 'help' for help or 'man' for tutorial █\n");
        printf("█  To open a snake game type 'snake' to leave game press 'q'       █\n");
        printf("█  To open an SLM type 'SLM' or 'slm' to chat with him             █\n");
        printf("█  To open an information on program type 'info' to leave type 'q' █\n");
        printf("█  To leave shell type 'leave'                                     █\n");
        resetColor();
        return 0;
}
void showHelp() {
    setColor();
    printf("COMMAND LIST:\n");
    printf("help              - Show command list\n");
    printf("clear             - Clear screen\n");
    printf("echo TEXT         - Print text\n");
    printf("version           - Show version\n");
    printf("system            - Show system info\n");
    printf("create FILE       - Create file\n");
    printf("write FILE TEXT   - Write text to file\n");
    printf("read FILE         - Read file\n");
    printf("delete FILE       - Delete file\n");
    printf("cd <dir>          - change directory\n");
    printf("mkdir <dir>       - create new folder in directory\n");
    printf("date / time       - show time\n");
    printf("calc              - calculate an expression (example: 2 + 3 * 4)\n");
    printf("hist              - show commands entered in last 5 minutes\n");
    printf("man               - manual for detail explanation of commands\n");
    printf("ktop              - shows what kernel it is and which version\n");
    printf("free              - showcase of free memory in partition\n");
    printf("panic             - Simulate kernel panic\n");
    printf("reboot            - reboot/restart\n");
    printf("exit              - Shutdown\n");
    resetColor();
}
static char command_history[MAX_HISTORY][MAX_INPUT];
static size_t command_history_count;
static void add_history(const char *command) {
    if (command[0] == '\0')
        return;
    if (command_history_count < MAX_HISTORY) {
        snprintf(command_history[command_history_count], MAX_INPUT, "%s", command);
        command_history_count++;
        return;
    }
    memmove(command_history, command_history + 1,
            (MAX_HISTORY - 1) * sizeof(command_history[0]));
    snprintf(command_history[MAX_HISTORY - 1], MAX_INPUT, "%s", command);
}
typedef struct {
    const char *current;
    int error;
    int division_by_zero;
} ExpressionParser;
static double parse_expression(ExpressionParser *parser);
static double parse_factor(ExpressionParser *parser) {
    char *end;
    double value;
    while (isspace((unsigned char)*parser->current))
        parser->current++;
    if (*parser->current == '+' || *parser->current == '-') {
        int negative = *parser->current == '-';
        parser->current++;
        value = parse_factor(parser);
        return negative ? -value : value;
    }
    if (*parser->current == '(') {
        parser->current++;
        value = parse_expression(parser);
        while (isspace((unsigned char)*parser->current))
            parser->current++;
        if (*parser->current != ')')
            parser->error = 1;
        else
            parser->current++;
        return value;
    }
    value = strtod(parser->current, &end);
    if (end == parser->current) {
        parser->error = 1;
        return 0.0;
    }
    parser->current = end;
    return value;
}
static double parse_term(ExpressionParser *parser) {
    double value = parse_factor(parser);
    while (!parser->error) {
        char operation;
        double next_value;
        while (isspace((unsigned char)*parser->current))
            parser->current++;
        operation = *parser->current;
        if (operation != '*' && operation != '/')
            break;
        parser->current++;
        next_value = parse_factor(parser);
        if (operation == '*') {
            value *= next_value;
        } else if (next_value == 0.0) {
            parser->division_by_zero = 1;
            parser->error = 1;
        } else {
            value /= next_value;
        }
    }
    return value;
}
static double parse_expression(ExpressionParser *parser) {
    double value = parse_term(parser);
    while (!parser->error) {
        char operation;
        double next_value;
        while (isspace((unsigned char)*parser->current))
            parser->current++;
        operation = *parser->current;
        if (operation != '+' && operation != '-')
            break;
        parser->current++;
        next_value = parse_term(parser);
        if (operation == '+')
            value += next_value;
        else
            value -= next_value;
    }
    return value;
}
static int evaluate_expression(const char *input, double *result,
                               int *division_by_zero) {
    ExpressionParser parser = {input, 0, 0};
    *result = parse_expression(&parser);
    while (isspace((unsigned char)*parser.current))
        parser.current++;
    *division_by_zero = parser.division_by_zero;
    return !parser.error && *parser.current == '\0';
}
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    setColor();
}
void ls(const char *path) {
    const char *directory_path = (path != NULL && path[0] != '\0') ? path : ".";
    DIR *directory = opendir(directory_path);
    struct dirent *entry;
    if (directory == NULL) {
        printf("ERROR: Cannot open directory '%s': %s\n", directory_path,
               strerror(errno));
        return;
    }
    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            printf("%s\n", entry->d_name);
    }
    closedir(directory);
}
void change_directory(const char *path) {
    char current_directory[PATH_MAX];
    if (path == NULL || path[0] == '\0') {
        printf("Usage: cd DIRECTORY\n");
        return;
    }
    if (chdir(path) != 0) {
        printf("ERROR: Cannot change directory: %s\n", strerror(errno));
        return;
    }
    if (getcwd(current_directory, sizeof(current_directory)) != NULL)
        printf("Current directory: %s\n", current_directory);
}
void make_directory(const char *name) {
    if (name == NULL || name[0] == '\0') {
        printf("Usage: mkdir DIRECTORY\n");
        return;
    }
    if (mkdir(name, 0755) != 0)
        printf("ERROR: Cannot create directory: %s\n", strerror(errno));
    else
        printf("Directory created: %s\n", name);
}
void date_and_time_command(int show_date, int show_time) {
    time_t current_time = time(NULL);
    struct tm *local = localtime(&current_time);
    if (local == NULL) {
        printf("ERROR: Cannot read system time.\n");
        return;
    }
    if (show_date)
        printf("Date: %04d-%02d-%02d\n", local->tm_year + 1900,
               local->tm_mon + 1, local->tm_mday);
    if (show_time)
        printf("Time: %02d:%02d:%02d\n", local->tm_hour, local->tm_min,
               local->tm_sec);
}
static void calculator_expression(void) {
	char input[MAX_INPUT];
    printf("Calculator\n");
    printf("Enter function(q for exit).\n");
    while (1) {
        double result;
        int division_by_zero;
        printf("calc> ");
        if (fgets(input, sizeof(input), stdin) == NULL)
            return;
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0 ||
            strcmp(input, "quit") == 0)
            return;
        if (input[0] == '\0')
            continue;
        if (evaluate_expression(input, &result, &division_by_zero))
            printf("= %.10g\n", result);
        else if (division_by_zero)
            printf("[ERROR]: deviding with 0 is not posible.\n");
        else
            printf("[ERROR]: bad inport. Use numbers and operators + - * /.\n");
    }
}
void calculator(void) {
    calculator_expression();
}
void history_of_last_five_minute_commands(void) {
    size_t index;
    if (command_history_count == 0) {
        printf("Command history is empty.\n");
        return;
    }
    for (index = 0; index < command_history_count; index++)
        printf("%zu  %s\n", index + 1, command_history[index]);
}
void showcase_of_working_proces_in_background(void) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    setColor();

    printf("\n");
    printf("     ██╗      █████╗ ██╗   ██╗ █████╗ ███╗   ██╗██████╗ ███████╗██████╗ \n");
    printf("     ██║     ██╔══██╗██║   ██║██╔══██╗████╗  ██║██╔══██╗██╔════╝██╔══██╗\n");
    printf("     ██║     ███████║██║   ██║███████║██╔██╗ ██║██║  ██║█████╗  ██████╔╝\n");
    printf("     ██║     ██╔══██║╚██╗ ██╔╝██╔══██║██║╚██╗██║██║  ██║██╔══╝  ██╔══██╗\n");
    printf("     ███████╗██║  ██║ ╚████╔╝ ██║  ██║██║ ╚████║██████╔╝███████╗██║  ██║\n");
    printf("     ╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ ╚══════╝╚═╝  ╚═╝\n\n");

    // OS
    FILE *f = fopen("/etc/os-release", "r");
    char line[256], os_name[128] = "Linux";
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
                char *p = line + 12;
                if (*p == '"') p++;
                strncpy(os_name, p, sizeof(os_name)-1);
                os_name[strcspn(os_name, "\"\n")] = 0;
                break;
            }
        }
        fclose(f);
    }
    printf("  OS:          %s\n", os_name);

    // Kernel
    f = fopen("/proc/version", "r");
    char kernel[128] = "Unknown";
    if (f) {
        if (fgets(line, sizeof(line), f)) {
            sscanf(line, "Linux version %127s", kernel);
        }
        fclose(f);
    }
    printf("  Kernel:      %s\n", kernel);

    printf("  Shell:       LAVANDER SHELL\n");

    // Architecture
    f = popen("uname -m", "r");
    char arch[64] = "Unknown";
    if (f) {
        if (fgets(arch, sizeof(arch), f)) arch[strcspn(arch, "\n")] = 0;
        pclose(f);
    }
    printf("  Architecture: %s\n", arch);

    // Uptime & Date
    printf("  Uptime:      %02d:%02d:%02d\n", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
    printf("  Date:        %02d-%02d-%d\n", tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);

    // Memory
    f = fopen("/proc/meminfo", "r");
    unsigned long long total = 0, available = 0;
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "MemTotal: %llu kB", &total) == 1) continue;
            if (sscanf(line, "MemAvailable: %llu kB", &available) == 1) continue;
        }
        fclose(f);
    }
    if (total > 0)
        printf("  Memory:      %llu MB / %llu MB\n", (total - available) / 1024, total / 1024);
    else
        printf("  Memory:      Unknown\n");

    // CPU
    f = fopen("/proc/cpuinfo", "r");
    char cpu[128] = "Unknown";
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            if (strncmp(line, "model name", 10) == 0) {
                char *p = strchr(line, ':');
                if (p) {
                    p++;
                    while (*p == ' ') p++;
                    strncpy(cpu, p, sizeof(cpu)-1);
                    cpu[strcspn(cpu, "\n")] = 0;
                    break;
                }
            }
        }
        fclose(f);
    }
    printf("  CPU:         %s\n", cpu);

    printf("  Status:      Running\n\n");
    resetColor();
}
void Emanuel(){
    setColor();
    printf(" This is an short and quick tutorial on how commands work inside Kernel. \n");
    printf(" \n");
    printf(" \n");
    printf(" 1. Command 'help' is a command that shows you list of all commands in\n");
    printf("\n");
    printf(" 2. Command 'clear' when its insert in, clears the screen and everything\n");
    printf(" that is on screen, that dosent mean that it deletes it, its just whipe\n");
    printf(" text from screen so screen can be again fresh clear, and have less to\n");
    printf(" none text on it.\n");
    printf(" \n");
    printf(" 3. Command 'echo' is a command that is added for fun, to repeat a sentence\n");
    printf(" added after. For example sentence: echo Hello, World! will repeat it self.\n");
    printf(" If you are qurious just like George you can try it.\n");
    printf(" \n");
    printf(" 4. Command 'version' will show the user latest version of the kernel.\n");
    printf(" \n");
    printf(" 5. Command 'system' function exactly like topfetch on Linux kernel.\n");
    printf(" It will show to user all specifications of the system, with kernel\n");
    printf(" that it is running on + on all of that can show an operating system.\n");
    printf(" That ofcourse means that it will show the user the details of the system.\n");
    printf(" If someone add this kernel inside his operating system. Ofcouse as i sayd\n");
    printf(" it in NOTICE.txt inside a download folder for this kernel. For adding this\n");
    printf(" kernel to your operating system. You need to contact me, the developer on\n");
    printf(" my email: dragomirpavlovic20@gmail.com and to do not change anything of 'man'\n");
    printf(" command, because it is important for me as a little trademark of this kernel. :D\n");
    printf(" \n");
    printf(" 6. Command 'create' is a command that allows you to create a new writeable file.\n");
    printf(" When you insert command create and after that add a name, as example: 'create myfile'\n");
    printf(" you will create a file named 'myfile' what you can use lated with other commands like\n");
    printf(" 'write', 'read' or 'delete' command. \n");
    printf(" \n");
    printf(" 7. Command 'write' is a command that allows the user to write a senteces or words inside\n");
    printf(" a file. For example writing 'write myFile Hello, World!' will write inside a file named \n");
    printf(" 'myFile' the sentence 'Hello, World!'. By typing again 'write myFile This is a grate world'\n");
    printf(" text will be added to the file 'myFile' and it will look like this: \n");
    printf(" Hello, World! This is a grate world \n");
    printf(" \n");
    printf(" 8. Command 'read' is a command that allows the user to read one of the files that he created\n");
    printf(" previously using command 'create'. For example if you type 'read myFile' it will show you the\n");
    printf(" context of the file 'myFile' that you created previously as: 'Hello World! This is a grate world'\n");
    printf(" If you previously created more that one file you can choose any one to read. \n");
    printf(" \n");
    printf(" 9. Command 'delete' is a very simple command. Typing in termial 'delete myFile' will delete the\n");
    printf(" existing one. Sadly after you once deleat it you can not get it back, it will be deleted forever.\n");
    printf(" Of course you can create it again using command 'create' and add same name as you add before but\n");
    printf(" that will not change the subject that file if forever gone. \n");
    printf(" \n");
    printf(" 10. Command 'cd' is the command that is most important for the usage of this kernel. It allows user\n");
    printf(" to change the directory of the files that he created in kernel as well as the files that are already\n");
    printf(" inside the kernel. Most inportant this is that he can simply 'screw around' with kernel and change \n");
    printf(" every file location if he wants. Ofcourse that will resort in kernel destucrion and kernel will enter\n");
    printf(" a 'panic' mode and it will be impossible to use it again. Unless you add 'reboot-da-system'. I will\n");
    printf(" explain it later in this tutorial what is diference between 'reboot', 'panic', 'reboot-da-system; and\n");
    printf(" 'exit' command. TO use this command type 'cd <directory_name>' in the terminal, and if you dont like\n");
    printf(" where your directory is add 'cd <directory_name1> replace <directory_name2>' or to add it type command\n");
    printf(" 'cd <directory_name1> add <directory_name2>' to add directory2 to directory1. \n");
    printf(" \n");
    printf(" 11. Command 'mkdir' is very small and simple command, it allows user to make new directory in kernel.\n");
    printf(" For example if user add 'mkdir myDirectory' it will create a new directory named 'myDirecory' inside\n");
    printf(" the kernels file system. If you read it to this point i want to tell you that at the end of tutorial there\n");
    printf(" will be a small explanation on how kernel function. So if you are interested stick to the end of tutorial.\n");
    printf(" \n");
    printf(" 12. Command 'date / time' is two command not one. Command 'date' will show you the date of the day for example '2020-20-01',\n");
    printf(" and command 'time' will show you the time of the day for example '00:30:01', (30 minutes and 1 second after midnight). \n");
    printf(" \n");
    printf(" 13. Command 'calc' opens a calculator for expressions with multiple numbers and operators +, -, * and /.\n");
    printf(" Enter an expression such as '2 + 3 * 4' and type 'q' to leave the calculator.\n");
    printf(" \n");
    printf(" 14. Command 'hist' is a command that will show you history of all of your command that are insert in the last 5 minutes of usage\n");
    printf(" of kernel. If you decide to be goofy and close kernel at 04:59 progress will not be saved because it saves after 5 minuts passed\n.");
    printf(" \n");
    printf(" 15. Command 'man' is this command, this will open and you will be able to read tutorial. Word 'man' is short for manual\n");
    printf(" \n");
    printf(" 16. Command 'ktop' shows what kernel version are u using.\n");
    printf(" It's used as a let's say, even if it's not 100%% true, as a inspiration from Linux.\n");
    printf(" I like the idea that when you type specific command just like 'ktop' to show you console top version. Console with a 'k'\n");
    printf(" like Konsole, because it is cool and why not. So command is konsole top (top version).\n");
    printf(" \n");
    printf(" 27. Command 'free' is a command that will show user free memory. Example: 'Free memory: is 20GB'. This is very useful command for tracking\n");
    printf(" your memory usage in kernel or if you want to delete something. \n");
    printf(" \n");
    printf(" 18. Command 'panic' is command that will or appear from nowhere if you make some problem inside a kernel or if you decide to goof around\n");
    printf(" and force kernel to go 'kuku crazy mode' and sudenly breake into peaces. DO NOT USE THIS COMMAND UNLESS YOU ARE SURE WHAT YOU ARE DOING.\n");
    printf(" OR IF YOU ARE A DEVELOPER WORKING ON THIS KERNEL BESIDE ME. OTHER WISE IF YOU ENTER THIS COMMAND IT WILL BREAKE YOU SYSTEM AND YOU WILL NEED\n");
    printf(" TO DO 'REBOOT-DA-SYSTEM' COMMAND TO RESTART OR REINSTALL KERNEL AGAIN DEPENDS FROM HOW MUCH YOU FUCK IT UP!!!!. YOUR DECISION FOR KERNEL ARE\n");
    printf(" YOUR OWN CONSEQUENCES AND ARE NOT MY PROBLEM. REINSTALL IT AGAIN.\n");
    printf(" \n");
    printf(" 19. Command 'reboot' is a command that will reboot (restart) the system.\n");
    printf(" \n");
    printf(" 21. Command 'exit' is a command that shuts down the kernel and closes it. Saving all process that you make on it before closing it.\n");
    printf(" \n");
    printf(" \n");
    printf(" EXPLANATION ON HOW KERNEL TREE FUNTION: \n");
    printf(" Instead of desktop you have main, so if you want to type desktop, you type 'cd main' to go to main screen of kernel. \n");
    printf(" Documments are called 'docs' in the kernel, and also you have 'data' folder that repressent all data that you have in kernel. There is\n");
    printf(" also 'bin' folder that repressent trashcan of kernel, where are deleted files and folders.\n");
    printf(" Tree looks like this: \n");
    printf(" Kernel \n");
    printf(" ├── main \n");
    printf(" │   ├── docs \n");
    printf(" │   ├── data \n");
    printf(" │   └── bin \n");
    printf(" └── user \n");
    printf("   ├── docs \n");
    printf("   ├── userData \n");
    printf("   └── UserBin \n");
    printf(" \n");
    printf(" To leave this tutorial press 'q' and then 'x' to confirm. \n");
   
    char firstKey, secondKey;
    printf("[MANUAL] Type q then x to leave: ");
    scanf(" %c %c", &firstKey, &secondKey);
    if ((firstKey == 'q' || firstKey == 'Q') && (secondKey == 'x' || secondKey == 'X')) {
        printf("\nLeaving tutorial...\n");
        return;
    }
    printf("\nContinuing tutorial...\n");
     resetColor();
}
void free_memory_in_well_memory_ofc(void) {
setColor();
#ifdef __linux__
    FILE *memory_file = fopen("/proc/meminfo", "r");
    char line[128];
    unsigned long long total = 0;
    unsigned long long available = 0;
    if (memory_file != NULL) {
        while (fgets(line, sizeof(line), memory_file) != NULL) {
            if (sscanf(line, "MemTotal: %llu kB", &total) == 1)
                continue;
            if (sscanf(line, "MemAvailable: %llu kB", &available) == 1)
                continue;
        }
        fclose(memory_file);
    }
    if (total > 0)
        printf("Memory: %llu MB total, %llu MB available\n", total / 1024,
               available / 1024);
    else
        printf("Memory information is unavailable.\n");
#else
    printf("Memory information is unavailable on this platform.\n");
#endif
 resetColor();
}
void rebooting_system(void) {
    setColor();
    printf("Restarting simulated Kernel...\n\n");
    clearScreen();
    bootScreen();
    resetColor();
}
void kernelPanic() {
    setColor();
    printf("\n!!! KERNEL PANIC !!!\n");
    printf("System halted.\n");
    printf("Posible error on user side\n");
    resetColor();
    exit(1);
}
void systemInfo() {
    setColor();
    printf("System Name: LAVANDER KERNEL\n");
    printf("Mode: CONSOLE-DUMB-TERMINAL\n");
    printf("Architecture: NN (Simulated)\n");
    printf("Memory: NONE\n");
    printf("Core Status: IN SIMULATION PROCESS\n");
    resetColor();
}
void createFile(const char *filename) {
    setColor();
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("ERROR: Cannot create file.\n");
        resetColor();
        return;
    }
    fclose(file);
    printf("File created successfully.\n");
    resetColor();
}
void writeFile(const char *filename, const char *text) {
    setColor();
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("ERROR: Cannot open file.\n");
        resetColor();
        return;
    }
    fprintf(file, "%s\n", text);
    fclose(file);
    printf("Data written to file.\n");
    resetColor();
}
void readFile(const char *filename) {
    setColor();
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("ERROR: Cannot open file.\n");
        resetColor();
        return;
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
    resetColor();
}
void deleteFile(const char *filename) {
    setColor();
    if (remove(filename) == 0) {
        printf("File deleted successfully.\n");
    } else {
        printf("ERROR: Cannot delete file.\n");
    }
    resetColor();
}
static char *command_argument(char *input, size_t command_length) {
    char *argument = input + command_length;
    while (isspace((unsigned char)*argument))
        argument++;
    return argument;
}
void loadingScreen(void) {
    setColor();
    printf("Loading...\n");
    resetColor();
}

void Auth(void) {
    setColor();
    printf("Auth skipped\n");
    resetColor();
}

void bootScreen(void) {
    clearScreen();
    shell_interface();
}
int main() {
    setColor();
    char input[MAX_INPUT];
    loadingScreen();
    Auth();
    bootScreen();
    while (1) {
        setColor();
        printf("[CORE-16] >> ");
        fflush(stdout);
        if (fgets(input, MAX_INPUT, stdin) == NULL)
            break;
        input[strcspn(input, "\n")] = 0;
        add_history(input);
        if (strcmp(input, "help") == 0) {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            showHelp();
            resetColor();
        }
        else if (strcmp(input, "clear") == 0) {
            clearScreen();
        }
        else if (strcmp(input, "version") == 0) {
        setColor();
            printf("VERSION 0.2.1\n");
	    printf("\n");
	    printf("--This is strictly inserted as printf command--\n");
	    printf("--No purpose to be created as a function--\n");
	    printf("--when it's just a text showing version--\n");
        resetColor();
        }
        else if (strcmp(input, "system") == 0) {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            systemInfo();
        }
        else if (strcmp(input, "man") == 0) {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            Emanuel();
        }
        else if (strcmp(input, "calc") == 0) {
            calculator();
        }
        else if (strncmp(input, "cd ", 3) == 0) {
            change_directory(command_argument(input, 3));
        }
        else if (strncmp(input, "mkdir ", 6) == 0) {
            make_directory(command_argument(input, 6));
        }
        else if (strcmp(input, "date") == 0) {
            date_and_time_command(1, 0);
        }
        else if (strcmp(input, "time") == 0) {
            date_and_time_command(0, 1);
        }
        else if (strcmp(input, "hist") == 0) {
            history_of_last_five_minute_commands();
        }
        else if (strcmp(input, "ktop") == 0) {
            showcase_of_working_proces_in_background();
        }
        else if (strcmp(input, "free") == 0) {
            free_memory_in_well_memory_ofc();
        }
        else if (strcmp(input, "reboot") == 0 ||
                 strcmp(input, "reboot-da-system") == 0) {
            rebooting_system();
        }
        else if (strncmp(input, "echo ", 5) == 0) {
            printf("%s\n", input + 5);
        }
        else if (strncmp(input, "create ", 7) == 0) {
            createFile(input + 7);
        }
        else if (strncmp(input, "write ", 6) == 0) {
            char *filename = strtok(input + 6, " ");
            char *text = strtok(NULL, "");
            if (filename && text)
                writeFile(filename, text);
            else
                printf("Usage: write FILE TEXT\n");
        }
        else if (strncmp(input, "read ", 5) == 0) {
            readFile(input + 5);
        }
        else if (strncmp(input, "delete ", 7) == 0) {
            deleteFile(input + 7);
        }
	else if (strcmp(input, "info") == 0) {
    setColor();
    printf("Project made by: Dragomir Pavlovic\n");
    printf("To contact creator: dragomirpavlovic20@gmail.com\n");
    printf("Project: CLI SHELL & SLM MODEL\n");
    printf("To exit press 'q'\n");
    char q[8];
    while (1) {
        if (fgets(q, sizeof(q), stdin) == NULL) break;
        q[strcspn(q, "\n")] = 0;
        if (strcmp(q, "q") == 0 || strcmp(q, "Q") == 0) break;
    }
    resetColor();
}
        else if (strcmp(input, "panic") == 0) {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            kernelPanic();
        }
        else if (strcmp(input, "exit") == 0) {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            setColor();
            printf("Shutting down Kernel\n");
	    printf("Or stoping it for safety reasons\n");
            break;
            resetColor();
        }
        else {
        setColor();
            printf("INVALID COMMAND. Type 'help'.\n");
        resetColor();
        }
    }
    resetColor();
    return 0;
}
