#include <unistd.h>
#include <stdlib.h>

int main() {
    char buffer[256];
    ssize_t bytesRead;
    float totalSum;
    char *currentChar;

    while ((bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        currentChar = buffer;
        totalSum = 0.0f;

        while (*currentChar) {
            while (*currentChar == ' ' || *currentChar == '\t' || *currentChar == '\n')
                currentChar++;

            int sign = 1;
            if (*currentChar == '-') {
                sign = -1;
                currentChar++;
            }

            float number = 0.0f;
            float fractional = 0.1f;
            int hasDigits = 0;

            while (*currentChar >= '0' && *currentChar <= '9') {
                number = number * 10 + (*currentChar - '0');
                currentChar++;
                hasDigits = 1;
            }

            if (*currentChar == '.') {
                currentChar++;
                while (*currentChar >= '0' && *currentChar <= '9') {
                    number += (*currentChar - '0') * fractional;
                    fractional *= 0.1f;
                    currentChar++;
                    hasDigits = 1;
                }
            }

            if (hasDigits)
                totalSum += number * sign;

            while (*currentChar && *currentChar != ' ' && *currentChar != '\t' && *currentChar != '\n')
                currentChar++;
        }

        char output[64];
        int outIndex = 0;
        int integerPart = (int)totalSum;
        float fractionPart = totalSum - integerPart;
        if (fractionPart < 0) fractionPart = -fractionPart;

        char tmpDigits[32];
        int tmpIndex = 0;
        int absInteger = integerPart < 0 ? -integerPart : integerPart;

        do {
            tmpDigits[tmpIndex++] = '0' + (absInteger % 10);
            absInteger /= 10;
        } while (absInteger > 0);

        if (integerPart < 0)
            output[outIndex++] = '-';

        while (tmpIndex--)
            output[outIndex++] = tmpDigits[tmpIndex];

        output[outIndex++] = '.';

        for (int i = 0; i < 2; i++) {
            fractionPart *= 10;
            int digit = (int)fractionPart;
            output[outIndex++] = '0' + digit;
            fractionPart -= digit;
        }

        output[outIndex++] = '\n';
        write(STDOUT_FILENO, output, outIndex);
    }

    return 0;
}
