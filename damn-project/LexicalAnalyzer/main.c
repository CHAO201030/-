/*
 * Copyright (c) 2024 Wangdao programmer training camp CPP 58th
 *
 * Authors:
 *   - CHAO  
 *     GitHub:https://github.com/CHAO201030/I-love-study
 *     Gitee :https://gitee.com/chao201030/I-love-study
 *
 * This software is licensed under the MIT license.
 * For more information, please refer to the LICENSE file.
 *
 * Description:
 *   This is a simple C lexical analysis machine based on C.
 *
 * Date: 2024-5-11
 */

#include"scanner.h"

static char* strtoken(Token token)
{
    switch (token.type)
    {
        // ���ַ�Token
    case TOKEN_LEFT_PAREN:      return "(";
    case TOKEN_RIGHT_PAREN:     return ")";
    case TOKEN_LEFT_BRACKET:    return "[";
    case TOKEN_RIGHT_BRACKET:   return "]";
    case TOKEN_LEFT_BRACE:      return "{";
    case TOKEN_RIGHT_BRACE:     return "}";
    case TOKEN_COMMA:           return ",";
    case TOKEN_DOT:             return ".";
    case TOKEN_SEMICOLON:       return ";";
    case TOKEN_TILDE:           return "~";
        // һ���ַ��������ַ���Token
    case TOKEN_PLUS:            return "+";
    case TOKEN_PLUS_PLUS:       return "++";
    case TOKEN_PLUS_EQUAL:      return "+=";
    case TOKEN_MINUS:           return "-";
    case TOKEN_MINUS_MINUS:     return "--";
    case TOKEN_MINUS_EQUAL:     return "-=";
    case TOKEN_MINUS_GREATER:   return "->";
    case TOKEN_STAR:            return "*";
    case TOKEN_STAR_EQUAL:      return "*=";
    case TOKEN_SLASH:           return "/";
    case TOKEN_SLASH_EQUAL:     return "/=";
    case TOKEN_PERCENT:         return "%";
    case TOKEN_PERCENT_EQUAL:   return "%=";
    case TOKEN_AMPER:           return "&";
    case TOKEN_AMPER_EQUAL:     return "&=";
    case TOKEN_AMPER_AMPER:     return "&&";
    case TOKEN_PIPE:            return "|";
    case TOKEN_PIPE_EQUAL:      return "|=";
    case TOKEN_PIPE_PIPE:       return "||";
    case TOKEN_HAT:             return "^";
    case TOKEN_HAT_EQUAL:       return "^=";
    case TOKEN_EQUAL:           return "=";
    case TOKEN_EQUAL_EQUAL:     return "==";
    case TOKEN_BANG:            return "!";
    case TOKEN_BANG_EQUAL:      return "!=";
    case TOKEN_LESS:            return "<";
    case TOKEN_LESS_EQUAL:      return "<=";
    case TOKEN_LESS_LESS:       return "<<";
    case TOKEN_GREATER:         return ">";
    case TOKEN_GREATER_EQUAL:   return ">=";
    case TOKEN_GREAER_GREATER: 	return ">>";
        // ����ֵ: ��ʶ��, �ַ�, �ַ���, ����
    case TOKEN_IDENTIFIER:      return "IDENTIFIER";
    case TOKEN_CHARACTER:       return "CHARACTER";
    case TOKEN_STRING:          return "STRING";
    case TOKEN_NUMBER:          return "NUMBER";
        // �ؼ���
    case TOKEN_SIGNED:          return "SIGNED";
    case TOKEN_UNSIGNED:        return "UNSIGNED";
    case TOKEN_CHAR:            return "CHAR";
    case TOKEN_SHORT:           return "SHORT";
    case TOKEN_INT:             return "INT";
    case TOKEN_LONG:            return "LONG";
    case TOKEN_FLOAT:           return "FLOAT";
    case TOKEN_DOUBLE:          return "DOUBLE";
    case TOKEN_STRUCT:          return "STRUCT";
    case TOKEN_UNION:           return "UNION";
    case TOKEN_ENUM:            return "ENUM";
    case TOKEN_VOID:            return "VOID";
    case TOKEN_IF:              return "IF";
    case TOKEN_ELSE:            return "ELSE";
    case TOKEN_SWITCH:          return "SWITCH";
    case TOKEN_CASE:            return "CASE";
    case TOKEN_DEFAULT:         return "DEFAULT";
    case TOKEN_WHILE:           return "WHILE";
    case TOKEN_DO:              return "DO";
    case TOKEN_FOR:             return "FOR";
    case TOKEN_BREAK:           return "BREAK";
    case TOKEN_CONTINUE:        return "CONTINUE";
    case TOKEN_RETURN:          return "RETURN";
    case TOKEN_GOTO:            return "GOTO";
    case TOKEN_CONST:           return "CONST";
    case TOKEN_SIZEOF:          return "SIZEOF";
    case TOKEN_TYPEDEF:         return "TYPEDEF";
        // ����Token
    case TOKEN_ERROR:           return "ERROR";
    case TOKEN_EOF:             return "EOF";
    }
}

static void run(const char* source)
{

    initScanner(source);
    int line = -1;
    // ��ӡToken, ����TOKEN_EOFΪֹ
    // ANSIת������ \033[32m ��ɫ�ı�
    printf("\033[32mOut[%2d]:\n", pyNB);

    for (;;)
    {
        Token token = scanToken();
        if (token.line != line)
        {
            printf("%4d ", token.line);
            line = token.line;
        }
        else 
        {
            printf("   | ");
        }
        printf("%10s -> '%.*s'\n", strtoken(token), token.length, token.start);

        if (token.type == TOKEN_EOF) break;
    }
}

// ���û��������û�ÿ����һ�д��룬����һ�д��룬����������
// repl��"read evaluate print loop"����д
static void repl()
{
    // 1. �� STDIN �������� printf("Hello World\n");
    // 2. ��ʼ���зִ�
    char buf[MAX_BUFFER_SIZE] = { 0 };
    // ANSIת������ \033[31m ��ɫ�ı�
    printf("\033[31mIn [%2d]\033[0m: ",pyNB);
    while (fgets(buf, MAX_BUFFER_SIZE - 1, stdin) != NULL)
    {
        run(buf);
        printf("\033[31mIn [%2d]\033[0m: ", ++pyNB);
    }

}

static char* readFile(const char* path)
{
    // �û������ļ������������ļ������ݶ����ڴ棬����ĩβ���'\0'
    // ע��: ����Ӧ��ʹ�ö�̬�ڴ���䣬���Ӧ������ȷ���ļ��Ĵ�С��
}

static void runFile(const char* path)
{
    // ����'.c'�ļ�:�û������ļ��������������ļ�������������
    char* source = readFile(path);
    run(source);    // ���� initScanner �� scanToken�ʷ�����
    free(source);
}

int main(int argc, const char* argv[])
{
    if (argc == 1)
    {
        // ./scanner û�в���,����뽻��ʽ����
        repl();
    }
    else if (argc == 2)
    {
        // ./scanner file �����һ������,����������ļ�
        runFile(argv[1]);
    }
    else
    {
        fprintf(stderr, "Usage: scanner [path]\n");
        exit(1);
    }

    return 0;
}