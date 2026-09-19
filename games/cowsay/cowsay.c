/*
 * FreeLinX cowsay / cowthink
 * Non-GNU, BSD-licensed C implementation of cowsay.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_LINES 1024
#define MAX_LINE_LEN 256

static void print_bubble(char lines[MAX_LINES][MAX_LINE_LEN], int count, int max_len, int think) {
    int i;
    printf(" ");
    for (i = 0; i < max_len + 2; i++) putchar('_');
    putchar('\n');

    if (count == 1) {
        if (think) {
            printf("( %s )\n", lines[0]);
        } else {
            printf("< %s >\n", lines[0]);
        }
    } else {
        for (i = 0; i < count; i++) {
            char left = '|', right = '|';
            if (!think) {
                if (i == 0) { left = '/'; right = '\\'; }
                else if (i == count - 1) { left = '\\'; right = '/'; }
            } else {
                left = '('; right = ')';
            }
            printf("%c %-*s %c\n", left, max_len, lines[i], right);
        }
    }

    printf(" ");
    for (i = 0; i < max_len + 2; i++) putchar('-');
    putchar('\n');
}

static void print_cow(const char *eyes, const char *tongue, int think) {
    char slash = think ? 'o' : '\\';
    printf("        %c   ^__^\n", slash);
    printf("         %c  (%s)\\_______\n", slash, eyes);
    printf("            (__)\\       )\\/\\\n");
    printf("             %s ||----w |\n", tongue[0] ? tongue : "  ");
    printf("                ||     ||\n");
}

int main(int argc, char **argv) {
    int think = 0;
    char eyes[3] = "oo";
    char tongue[3] = "  ";
    int wrap = 40;
    int opt;

    if (strstr(argv[0], "cowthink") != NULL) {
        think = 1;
    }

    while ((opt = getopt(argc, argv, "e:T:W:bdgpstwyh")) != -1) {
        switch (opt) {
            case 'e':
                strncpy(eyes, optarg, 2);
                eyes[2] = '\0';
                break;
            case 'T':
                strncpy(tongue, optarg, 2);
                tongue[2] = '\0';
                break;
            case 'W':
                wrap = atoi(optarg);
                if (wrap < 1) wrap = 40;
                break;
            case 'b': strcpy(eyes, "=="); break;
            case 'd': strcpy(eyes, "XX"); strcpy(tongue, "U "); break;
            case 'g': strcpy(eyes, "$$"); break;
            case 'p': strcpy(eyes, "@@"); break;
            case 's': strcpy(eyes, "**"); strcpy(tongue, "U "); break;
            case 't': strcpy(eyes, "--"); break;
            case 'w': strcpy(eyes, "OO"); break;
            case 'y': strcpy(eyes, ".."); break;
            case 'h':
            default:
                fprintf(stderr, "Usage: %s [-bdgpstwy] [-e eyes] [-T tongue] [-W wrap] [message]\n", argv[0]);
                return (opt == 'h' ? 0 : 1);
        }
    }

    char message[65536] = "";
    if (optind < argc) {
        for (int i = optind; i < argc; i++) {
            if (i > optind) strcat(message, " ");
            strcat(message, argv[i]);
        }
    } else {
        char buf[1024];
        while (fgets(buf, sizeof(buf), stdin)) {
            strcat(message, buf);
        }
        size_t len = strlen(message);
        while (len > 0 && (message[len - 1] == '\n' || message[len - 1] == '\r')) {
            message[--len] = '\0';
        }
    }

    if (strlen(message) == 0) {
        strcpy(message, "FreeLinX: 100% independent non-GNU OS");
    }

    char lines[MAX_LINES][MAX_LINE_LEN];
    int line_count = 0;
    int max_len = 0;

    char *token = strtok(message, "\n");
    while (token != NULL && line_count < MAX_LINES) {
        size_t tlen = strlen(token);
        while (tlen > (size_t)wrap && line_count < MAX_LINES) {
            int split = wrap;
            while (split > 0 && !isspace((unsigned char)token[split])) split--;
            if (split == 0) split = wrap;
            strncpy(lines[line_count], token, split);
            lines[line_count][split] = '\0';
            if ((int)strlen(lines[line_count]) > max_len) max_len = strlen(lines[line_count]);
            line_count++;
            token += split;
            while (*token && isspace((unsigned char)*token)) token++;
            tlen = strlen(token);
        }
        if (line_count < MAX_LINES) {
            strncpy(lines[line_count], token, MAX_LINE_LEN - 1);
            lines[line_count][MAX_LINE_LEN - 1] = '\0';
            if ((int)strlen(lines[line_count]) > max_len) max_len = strlen(lines[line_count]);
            line_count++;
        }
        token = strtok(NULL, "\n");
    }

    print_bubble(lines, line_count, max_len, think);
    print_cow(eyes, tongue, think);

    return 0;
}
