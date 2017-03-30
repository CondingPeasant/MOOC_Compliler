#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LINE_BUFFER_SIZE 1024
#define ID_SIZE 256

const char* FILE_NAME = "/home/chen/Documents/MOOC_Compliler/src/lexer-I.txt";
unsigned int gCurLine = 0;
unsigned int gCurCol = 0;
FILE* gFd;

char gLineBuffer[LINE_BUFFER_SIZE] = {0};

enum Token_Kind_t {TOKEN_ID, TOKEN_NUM, TOKEN_IF};

typedef struct
{
  enum Token_Kind_t kind;
  unsigned int line;
  unsigned int col;
} Token_t;

typedef struct
{
  enum Token_Kind_t kind;
  unsigned int line;
  unsigned int col;
} Token_IF_t;

typedef struct
{
  enum Token_Kind_t kind;
  unsigned int line;
  unsigned int col;
  char* content;
} Token_ID_t;

typedef struct
{
  enum Token_Kind_t kind;
  unsigned int line;
  unsigned int col;
  char* content;
} Token_NUM_t;

char* getLineBuffer();
Token_t* nextToken();
void printToken(const Token_t* token);
Token_t* newToken(enum Token_Kind_t kind);
void forgetToken(Token_t* token);
int isUpperCase(char c);
int isLowerCase(char c);
int isNumber(char c);
int isBlankChar(char c);

int main() {
    gFd = fopen(FILE_NAME, "r");
    if (!gFd) {
        printf("Error occurs at fopen, errno = %d\n", errno);
    }

    while (NULL != getLineBuffer()) {
        gCurLine ++;
        gCurCol = 1;
        while ('\0' != gLineBuffer[gCurCol - 1]) {
            Token_t* token = nextToken();
            printToken(token);
            forgetToken(token);
        }
    }

    if (gFd)
        fclose(gFd);
    return 0;
}

char* getLineBuffer() {
    if (gLineBuffer != NULL && gFd != NULL) 
        return fgets(gLineBuffer, LINE_BUFFER_SIZE, gFd);
    else
        return NULL;
}

Token_t* nextToken() {
    unsigned int ptr = gCurCol;
    Token_t* token = NULL;
    Token_ID_t* tokenId = NULL;
    Token_NUM_t* tokenNum = NULL;
    Token_IF_t* tokenIf = NULL;
    if ('\0' == gLineBuffer[ptr - 1])
        return NULL;
    while (' ' == gLineBuffer[ptr - 1] 
            || '\n' == gLineBuffer[ptr - 1]) {
        ptr ++;
        gCurCol++;
    }

    if ('i' == gLineBuffer[ptr - 1]) {
        ptr++;
        if ('f' == gLineBuffer[ptr - 1]) {
            ptr++;
            if (isBlankChar(gLineBuffer[ptr - 1])) {
                tokenIf = (Token_IF_t*)newToken(TOKEN_IF);
                token = (Token_t*)tokenIf;
                gCurCol = ptr;
            } else if (isUpperCase(gLineBuffer[ptr - 1])
                    || isLowerCase(gLineBuffer[ptr - 1])
                    || '_' == gLineBuffer[ptr - 1]) {
                ptr++;
                while(isUpperCase(gLineBuffer[ptr - 1])
                       || isLowerCase(gLineBuffer[ptr - 1])
                       || '_' == gLineBuffer[ptr - 1]) {
                    ptr++;
                }
                tokenId = (Token_ID_t*)newToken(TOKEN_ID);
                memcpy(tokenId->content, &gLineBuffer[gCurCol - 1], ptr - gCurCol);
                token = (Token_t*)tokenId;
                gCurCol = ptr;
            }
        } else if (isUpperCase(gLineBuffer[ptr - 1])
                || isLowerCase(gLineBuffer[ptr - 1])
                || '_' == gLineBuffer[ptr - 1]) {
            ptr++;
            while(isUpperCase(gLineBuffer[ptr - 1])
                   || isLowerCase(gLineBuffer[ptr - 1])
                   || '_' == gLineBuffer[ptr - 1]) {
                ptr++;
            }
            tokenId = (Token_ID_t*)newToken(TOKEN_ID);
            memcpy(tokenId->content, &gLineBuffer[gCurCol - 1], ptr - gCurCol);
            token = (Token_t*)tokenId;
            gCurCol = ptr;
        }
    } else if (isUpperCase(gLineBuffer[ptr - 1])
            || isLowerCase(gLineBuffer[ptr - 1])
            || '_' == gLineBuffer[ptr - 1]) {
        ptr++;
        while(isUpperCase(gLineBuffer[ptr - 1])
               || isLowerCase(gLineBuffer[ptr - 1])
               || '_' == gLineBuffer[ptr - 1]) {
            ptr++;
        }
            tokenId = (Token_ID_t*)newToken(TOKEN_ID);
            memcpy(tokenId->content, &gLineBuffer[gCurCol - 1], ptr - gCurCol);
            token = (Token_t*)tokenId;
            gCurCol = ptr;
    } else if ('0' == gLineBuffer[ptr - 1]) {
        ptr++;
        if (isBlankChar(gLineBuffer[ptr - 1])) {
            tokenNum = (Token_NUM_t*)newToken(TOKEN_NUM);
            memcpy(tokenNum->content, &gLineBuffer[gCurCol - 1], 1);
            token = (Token_t*)tokenNum;
            gCurCol = ptr;
        }
    } else if (isNumber(gLineBuffer[ptr - 1])) {
        ptr++;
        while(isNumber(gLineBuffer[ptr - 1])) {
            ptr++;
        }
        tokenNum = (Token_NUM_t*)newToken(TOKEN_NUM);
        memcpy(tokenNum->content, &gLineBuffer[gCurCol - 1], ptr - gCurCol);
        token = (Token_t*)tokenNum;
        gCurCol = ptr;
    }
    return token;
}

void printToken(const Token_t* token) {
    if (NULL == token)
        return;

    switch(token->kind) {
    case TOKEN_IF:
        printf("IF ");
        break;
    case TOKEN_ID:
        printf("ID(%s) ", ((Token_ID_t*)token)->content);
        break;
    case TOKEN_NUM:
        printf("NUM(%s) ", ((Token_NUM_t*)token)->content);
        break;
    default:
        break;
    }
    printf("(%d, %d)\n", token->line, token->col);
}

Token_t* newToken(enum Token_Kind_t kind) {
    Token_t* token = NULL;
    switch(kind) {
    case TOKEN_NUM:
        if (token = (Token_t*)malloc(sizeof(Token_NUM_t)))
            memset(token, 0, sizeof(Token_NUM_t));
        if (((Token_NUM_t*)token)->content = (char*)malloc(ID_SIZE * sizeof(char)))
            memset(((Token_NUM_t*)token)->content, 0, ID_SIZE);
        break;
    case TOKEN_ID:
        if (token = (Token_t*)malloc(sizeof(Token_ID_t)))
            memset(token, 0, sizeof(Token_ID_t));
        if (((Token_ID_t*)token)->content = (char*)malloc(ID_SIZE * sizeof(char)))
            memset(((Token_ID_t*)token)->content, 0, ID_SIZE);
        break;
    case TOKEN_IF:
        if (token = (Token_t*)malloc(sizeof(Token_IF_t)))
            memset(token, 0, sizeof(Token_IF_t));
        break;
    default:
        break;
    }
    if (token) {
        token->kind = kind;
        token->line = gCurLine;
        token->col = gCurCol;
    }
    return token;
}

void forgetToken(Token_t* token) {
    if (token) {
        switch(token->kind) {
        case TOKEN_NUM:
            if (((Token_NUM_t*)token)->content)
                free(((Token_NUM_t*)token)->content);
            break;
        case TOKEN_ID:
            if (((Token_ID_t*)token)->content)
                free(((Token_ID_t*)token)->content);
            break;
        default:
            break;
        }
        free((void*)token);
    }
}

int isUpperCase(char c) {
    if (c >= 'A' && c <= 'Z')
        return 1;
    else
        return 0;
}

int isLowerCase(char c) {
    if (c >= 'a' && c <= 'z')
        return 1;
    else
        return 0;
}

int isNumber(char c) {
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}

int isBlankChar(char c) {
    if (c == ' ' || c == '\n')
        return 1;
    else
        return 0;
}
