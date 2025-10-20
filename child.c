#include <unistd.h>
#include <stdlib.h>

int main() {
    char buff[256];
    ssize_t bytesRead;
    float sm;
    char *curr;

    while ((bytesRead = read(STDIN_FILENO, buff, sizeof(buff) - 1)) > 0) {
        buff[bytesRead] = '\0';
        curr = buff;
        sm = 0.0f;

        while (*curr) {
            while (*curr == ' ' || *curr == '\t' || *curr == '\n')
                curr++; 

            int sign = 1;
            if (*curr == '-') {
                sign = -1;
                curr++;
            }

            float number = 0.0f;
            float fractional = 0.1f;
            int has_dig = 0;

            while (*curr >= '0' && *curr <= '9') {
                number = number * 10 + (*curr - '0');
                curr++;
                has_dig = 1;
            }

            if (*curr == '.') {
                curr++;
                while (*curr >= '0' && *curr <= '9') {
                    number += (*curr - '0') * fractional;
                    fractional *= 0.1f;
                    curr++;
                    has_dig = 1;
                }
            }

            if (has_dig)
                sm += number * sign;

            while (*curr && *curr != ' ' && *curr != '\t' && *curr != '\n')
                curr++;
        }

        char output[64];
        int out_i = 0;
        int integerPart = (int)sm;
        float fractionPart = sm - integerPart;
        if (fractionPart < 0) fractionPart = -fractionPart;

        char tmpDigits[32];
        int tmp_i = 0;
        int abs_num;
        if (integerPart < 0)
            abs_num = -integerPart;
        else
            abs_num = integerPart;

        if (abs_num == 0) {
            tmpDigits[tmp_i++] = '0';
        } else {
            while (abs_num > 0) {
                tmpDigits[tmp_i++] = '0' + (abs_num % 10);
                abs_num = abs_num / 10;
            }
        }

        if (integerPart < 0)
            output[out_i++] = '-';

        while (tmp_i > 0) {
            output[out_i++] = tmpDigits[tmp_i - 1];
            tmp_i--;
        }

        output[out_i++] = '.';

        for (int i = 0; i < 2; i++) {
            fractionPart = fractionPart * 10;
            int digit = (int)fractionPart;
            output[out_i++] = '0' + digit;
            fractionPart = fractionPart - digit;
        }

        output[out_i++] = '\n';
        write(STDOUT_FILENO, output, out_i);
    }

    return 0;
}