#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    char filename[256];
    const char prompt[] = "введите имя файла: ";
    write(STDOUT_FILENO, prompt, sizeof(prompt) - 1);

    ssize_t nameLength = read(STDIN_FILENO, filename, sizeof(filename) - 1);
    if (nameLength <= 0) {
        const char err[] = "ошибка ввода имени файла\n";
        write(STDERR_FILENO, err, sizeof(err) - 1);
        _exit(EXIT_FAILURE);
    }

    if (filename[nameLength - 1] == '\n')
        filename[nameLength - 1] = '\0';
    else
        filename[nameLength] = '\0';

    int fileDescriptor = open(filename, O_RDONLY);
    if (fileDescriptor == -1) {
        const char err[] = "ошибка открытия файла\n";
        write(STDERR_FILENO, err, sizeof(err) - 1);
        _exit(EXIT_FAILURE);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        const char err[] = "ошибка создания pipe\n";
        write(STDERR_FILENO, err, sizeof(err) - 1);
        _exit(EXIT_FAILURE);
    }

    pid_t childPid = fork();
    if (childPid == -1) {
        const char err[] = "ошибка fork()\n";
        write(STDERR_FILENO, err, sizeof(err) - 1);
        _exit(EXIT_FAILURE);
    }

    if (childPid == 0) {
        close(pipefd[0]);
        dup2(fileDescriptor, STDIN_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);
        close(fileDescriptor);
        close(pipefd[1]);
        execl("./child", "child", NULL);
        const char err[] = "ошибка execl()\n";
        write(STDERR_FILENO, err, sizeof(err) - 1);
        _exit(EXIT_FAILURE);
    } else {
        close(fileDescriptor);
        close(pipefd[1]);
        char buffer[256];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytesRead);
        }
        close(pipefd[0]);
        wait(NULL);
    }

    return 0;
}
