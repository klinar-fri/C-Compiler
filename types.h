#ifndef TYPES_H
#define TYPES_H

typedef enum{
    tok_str,
    tok_num,
    tok_reserved,
    tok_name,
    tok_operator,
    tok_separator
} TokenType;

const char* tokenStrings[6] = {
    "niz",
    "stevilo",
    "rezerviranka",
    "ime",
    "operator",
    "locilo",
};

const char* reservedKeywords[7] = {
    "int",
    "char",
    "if",
    "else",
    "for",
    "return",
    "while",
};

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



#endif