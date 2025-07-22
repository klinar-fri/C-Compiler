#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define STB_DS_IMPLEMENTATION
#include "./stb_ds.h"
#include "types.h"

// print to txt file, print to stdout, or default save to dynamic array 
#define PRINT_TO_FILE 0
#define PRINT_TO_STDOUT 0

typedef struct{
    TokenType tokenType;
    char* tokenValue; 
} Token;

typedef struct{
    Token* items;
    int size;
} Tokens;

void printTokens(Tokens tokens){
    for(int i = 0; i < tokens.size; i++){
        const char* currTokenTypeStr = tokenStrings[tokens.items[i].tokenType];
        char* currTokenValue = tokens.items[i].tokenValue;
        printf("%s[%s]\n", currTokenTypeStr, currTokenValue);
    }
}

void freeStringsTokens(Tokens tokens){
    for(int i = 0; i < tokens.size; i++){
        free(tokens.items[i].tokenValue);
    }
}

void printZnak(TokenType tok, char znak, FILE* output){
    fprintf(output, "%s[%c]\n", tokenStrings[tok], znak);
}


int main(){
    // FILE* inputFile = stdin; // uncomment this to read from stdin
    FILE* inputFile = fopen("program.c", "r");
    #if PRINT_TO_FILE
    FILE* output = fopen("tokens.txt", "w");
    #else
    FILE* output = stdout;
    #endif
    char znak = '~';
    char curr;
    char* newStr;
    Token currToken;
    int i = 0; // index v currWord
    char currWord[1001];
    
    Tokens tokens = {0}; // dynamic array of tokens
    
    while(znak != EOF){
        znak = getc(inputFile);

        switch(znak){
            // operatorji
            case '=' : 
            case '+' : 
            case '-' : 
            case '*' : 
                curr = znak;
                znak = getc(inputFile);
                if(znak == '='){
                    #if PRINT_TO_FILE
                    fprintf(output, "%s[%c=]\n", tokenStrings[tok_operator], znak);
                    #elif PRINT_TO_STDOUT
                    printf("%s[%c=]\n", tokenStrings[tok_operator], znak);
                    #else
                    char* newStr = malloc(2 * sizeof(char));
                    newStr[0] = znak;
                    newStr[1] = '\0';
                    Token currToken = {.tokenType = tok_operator, .tokenValue = newStr};
                    arrput(tokens.items, currToken);
                    tokens.size += 1;
                    #endif
                }else{
                    ungetc(znak, inputFile);
                    #if PRINT_TO_STDOUT || PRINT_TO_TXT
                    printZnak(tok_operator, curr, output);
                    #else
                    char* newStr = malloc(2 * sizeof(char));
                    newStr[0] = znak;
                    newStr[1] = '\0';
                    Token currToken = {.tokenType = tok_operator, .tokenValue = newStr};
                    arrput(tokens.items, currToken);
                    tokens.size += 1;
                    #endif
                }
            break;
            
            // vrstični komentar in blocni komentar
            case '/' : 
                curr = znak;
                znak = getc(inputFile);
                if(znak == '='){
                    #if PRINT_TO_FILE
                    fprintf(output, "%s[/=]\n", tokenStrings[tok_operator]);
                    #elif PRINT_TO_STDOUT
                    printf("%s[/=]\n", tokenStrings[tok_operator]);
                    #else
                    char* newStr = malloc(2 * sizeof(char));
                    newStr[0] = znak;
                    newStr[1] = '\0';
                    Token currToken = {.tokenType = tok_operator, .tokenValue = newStr};
                    arrput(tokens.items, currToken);
                    tokens.size += 1;
                    #endif
                }else if(znak == '/'){
                    znak = getc(inputFile);
                    while(znak != '\n' && znak != EOF){
                        znak = getc(inputFile);
                    } 
                }else if(znak == '*'){
                    int dolzinaKomentarja = 2;
                    char prev = '~';
                    while(1){
                        if(prev == '*' && znak == '/' && dolzinaKomentarja >= 4){
                           break; 
                        }
                        dolzinaKomentarja++;
                        prev = znak;
                        znak = getc(inputFile);
                    } 
                }else{
                    ungetc(znak, inputFile);
                    #if PRINT_TO_STDOUT || PRINT_TO_TXT
                    printZnak(tok_operator, curr, output);
                    #else
                    char* newStr = malloc(2 * sizeof(char));
                    newStr[0] = znak;
                    newStr[1] = '\0';
                    Token currToken = {.tokenType = tok_operator, .tokenValue = newStr};
                    arrput(tokens.items, currToken);
                    tokens.size += 1;
                    #endif
                }
            break;
            
            case '>' : 
            case '<' : 
                curr = znak;
                znak = getc(inputFile);
                if(znak == '='){
                    #if PRINT_TO_FILE
                    fprintf(output, "%s[%c=]\n", tokenStrings[tok_operator], znak);
                    #elif PRINT_TO_STDOUT
                    printf("%s[%c=]\n", tokenStrings[tok_operator], znak);
                    #else
                    newStr = malloc(2 * sizeof(char));
                    newStr[0] = znak;
                    newStr[1] = '\0';
                    Token currToken = {.tokenType = tok_operator, .tokenValue = newStr};
                    arrput(tokens.items, currToken);
                    tokens.size += 1;
                    #endif
                }else{
                    ungetc(znak, inputFile);
                    // printZnak(tok_operator, curr, output);
                    char* newStr = malloc(2 * sizeof(char));
                    newStr[0] = znak;
                    newStr[1] = '\0';
                    Token currToken = {.tokenType = tok_operator, .tokenValue = newStr};
                    arrput(tokens.items, currToken);
                    tokens.size += 1;
                }
            break;

            // locila
            case ',' : 
            case ';' : 
            case '(' : 
            case ')' : 
            case '{' : 
            case '}' : 
            case '[' : 
            case ']' : 
                char* newStr = malloc(2 * sizeof(char));
                newStr[0] = znak;
                newStr[1] = '\0';
                Token currToken = {.tokenType = tok_separator, .tokenValue = newStr};
                arrput(tokens.items, currToken);
                tokens.size += 1;
                // printZnak(tok_separator, znak, output);
            break;
            
            // niz
            case '"' : 
                i = 0;
                znak = getc(inputFile);
                currWord[i] = znak;
                while(znak != '"' && znak != EOF){
                    i++;
                    znak = getc(inputFile);
                    currWord[i] = znak;
                }
                currWord[i] = '\0';
                {
                #if PRINT_TO_FILE
                fprintf(output, "%s[%s]\n",tokenStrings[tok_str], currWord);
                #elif PRINT_TO_STDOUT
                printf("%s[%s]\n",tokenStrings[tok_str], currWord);
                #else
                char* newStr = malloc((1 + strlen(&currWord[0])) * sizeof(char));
                strcpy(newStr, currWord);
                Token currToken = {.tokenType = tok_str, .tokenValue = newStr};
                arrput(tokens.items, currToken);
                tokens.size += 1;
                #endif
                }
            break;

            // rezerviranke
            default:
                if(isdigit(znak)){
                    i = 0;
                    while(isdigit(znak)){
                        currWord[i++] = znak;
                        znak = getc(inputFile);
                    }
                    currWord[i] = '\0';
                    ungetc(znak, inputFile);

                    #if PRINT_TO_FILE
                    fprintf(output, "%s[%s]\n", tokenStrings[tok_num], currWord);
                    #elif PRINT_TO_STDOUT
                    printf("%s[%s]\n", tokenStrings[tok_num], currWord);
                    #else
                    char* newStr = malloc((1 + strlen(&currWord[0])) * sizeof(char));
                    strcpy(newStr, currWord);
                    Token currToken = {.tokenType = tok_num, .tokenValue = newStr};
                    arrput(tokens.items, currToken);
                    tokens.size += 1;
                    #endif
                }else if(isalpha(znak) || znak == '_'){
                    i = 0;
                    currWord[i] = znak;
                    znak = getc(inputFile);
                    while(isalpha(znak) || isdigit(znak) || znak == '_'){
                        i++;
                        currWord[i] = znak;
                        znak = getc(inputFile);
                    }
                    i++;
                    currWord[i] = '\0';
                    ungetc(znak, inputFile);
                    // printf("%s\n", currWord);
                    bool printed = false;
                    if(i < 7){
                        for(int idx = 0; idx < 7; idx++){
                            if(strcmp(currWord, reservedKeywords[idx]) == 0){
                                #if PRINT_TO_FILE
                                fprintf(output, "%s[%s]\n", tokenStrings[tok_reserved], currWord);
                                #elif PRINT_TO_STDOUT
                                printf("%s[%s]\n", tokenStrings[tok_reserved], currWord);
                                #else
                                char* newStr = malloc((1 + strlen(&currWord[0])) * sizeof(char));
                                strcpy(newStr, currWord);
                                Token currToken = {.tokenType = tok_reserved, .tokenValue = newStr};
                                arrput(tokens.items, currToken);
                                tokens.size += 1;
                                #endif
                                printed = true;
                                break;
                            }
                        }
                    }
                    if(!printed){
                        #if PRINT_TO_FILE
                        fprintf(output, "%s[%s]\n", tokenStrings[tok_name], currWord);
                        #elif PRINT_TO_STDOUT
                        printf("%s[%s]\n", tokenStrings[tok_name], currWord);
                        #else
                        char* newStr = malloc((1 + strlen(&currWord[0])) * sizeof(char));
                        strcpy(newStr, currWord);
                        Token currToken = {.tokenType = tok_name, .tokenValue = newStr};
                        arrput(tokens.items, currToken);
                        tokens.size += 1;
                        #endif
                    }
                }
            break;
        }
    }
    #if !PRINT_TO_FILE && !PRINT_TO_STDOUT 
    printTokens(tokens);
    freeStringsTokens(tokens);
    arrfree(tokens.items);
    #endif

    #if PRINT_TO_FILE
    fclose(output);
    #endif
    return 0;
}