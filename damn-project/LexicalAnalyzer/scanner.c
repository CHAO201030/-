#include"scanner.h"

int pyNB = 1;

typedef struct scanner_t{
    const char* start;
    const char* current;
    int line;
} Scanner;

// ȫ�ֱ���
Scanner scanner;

void initScanner(const char* source)
{
    // ��ʼ��scanner
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

/***************************************************************************************
 *                                   ��������										   *
 ***************************************************************************************/

// If c is alpha return True, else return False
static bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_';
}

// If c is digit return True, else return False
static bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

// If pointer variable Scanner.current point to '\0\ return True, else return False 
static bool isAtEnd()
{
    return *scanner.current == '\0';
}

// Scanner.current point to next position and return it
static char advance()
{
    return *scanner.current++;
}

// Return the object pointed by Scanner.current
static char peek()
{
    return *scanner.current;
}

// Return the next object pointed by Scanner.current
static char peekNext()
{
    if (isAtEnd()) return '\0';
    return *(scanner.current + 1);
}

// If next == expect return true and cur mv to next pos
static bool match(char expected)
{
    if (isAtEnd()) return false;
    if (peek() != expected) return false;
    scanner.current++;
    return true;
}

// ����TokenType, ������Ӧ���͵�Token�������ء�
static Token makeToken(TokenType type)
{
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;

    return token;
}

// �������ܽ��������ʱ�����Ǵ���һ��ERROR Token.
// ���磺����'$' �ȷ���ʱ�����ַ������ַ�û�ж�Ӧ��������ʱ��
static Token errorToken(const char* message)
{
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;

    return token;
}

static void skipWhitespace()
{
    // �����հ��ַ�: ' ', '\r', '\t', '\n'��ע��
    // ע����'//'��ͷ, һֱ����β
    // ע�����scanner.line��
    while (*scanner.current)
    {
        switch (*scanner.current)
        {
        case ' ':case '\r':case '\t':advance(); break;
        case '\n':advance(); scanner.line++; break;
        case '/':
        {
            if (peekNext() == '/')
            {
                while (peek() != '\n')
                {
                    advance();
                }
                break;
            }
        }
        default:
            return;
        }
    }
}

// ����˵����
// start: ���ĸ�����λ�ÿ�ʼ�Ƚ�
// length: Ҫ�Ƚ��ַ��ĳ���
// rest: Ҫ�Ƚϵ�����
// type: �����ȫƥ�䣬��˵����type���͵Ĺؼ���

static TokenType checkKeyword(int start, int length, const char* rest, TokenType type)
{
    int len = (int)(scanner.current - scanner.start); // TOKEN�ĳ���
    // start + length: �ؼ��ֵĳ���
    if (start + length == len && memcmp(scanner.start + start, rest, length) == 0)
    {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

// �жϵ�ǰToken�����Ǳ�ʶ�����ǹؼ���
static TokenType identifierType()
{
    // ȷ��identifier������Ҫ�����ַ�ʽ��
    // 1. �����еĹؼ��ַ����ϣ���У�Ȼ����ȷ��
    // 2. �����еĹؼ��ַ���Trie���У�Ȼ����ȷ��
    // Trie���ķ�ʽ�����ǿռ��ϻ���ʱ���϶����ڹ�ϣ��ķ�ʽ
    char c = scanner.start[0];
    TokenType helper;
    // ��switch���ʵ��Trie��
    switch (c)
    {
    case 'b':return checkKeyword(1, 4, "reak", TOKEN_BREAK);
    case 'c':
    {
        // case
        helper = checkKeyword(1, 3, "ase", TOKEN_CASE);
        if (helper == TOKEN_CASE)return helper;
        
        // char
        helper = checkKeyword(1, 3, "har", TOKEN_CHAR);
        if (helper == TOKEN_CHAR)return helper;
        
        // continue
        helper = checkKeyword(1, 7, "ontinue", TOKEN_CONTINUE);
        if (helper == TOKEN_CONTINUE)return helper;

        // const or identifier
        helper = checkKeyword(1, 4, "onst", TOKEN_CONST);
        
        return helper;    
    }
    case 'd':
    {
        // default
        helper = checkKeyword(1, 6, "efault", TOKEN_DEFAULT);
        if (helper == TOKEN_DEFAULT)return helper;

        // double
        helper = checkKeyword(1, 5, "ouble", TOKEN_DOUBLE);

        return helper;
    }
    case 'e':
    {
        // enum
        helper = checkKeyword(1, 3, "num", TOKEN_ENUM);
        if (helper == TOKEN_ENUM)return helper;

        // else
        helper = checkKeyword(1, 3, "lse", TOKEN_ELSE);

        return helper;
    }
    case 'f':
    {
        // float
        helper = checkKeyword(1, 4, "loat", TOKEN_FLOAT);
        if (helper == TOKEN_FLOAT)return helper;

        // for
        helper = checkKeyword(1, 2, "or", TOKEN_FOR);

        return helper;
    }
    case 'g':return checkKeyword(1, 3, "oto", TOKEN_GOTO);
    case 'i':
    {
        // if
        helper = checkKeyword(1, 1, "f", TOKEN_IF);
        if (helper == TOKEN_IF)return helper;

        // int
        helper = checkKeyword(1, 2, "nt", TOKEN_INT);

        return helper;
    }
    case 'l':return checkKeyword(1, 3, "ong", TOKEN_LONG);
    case 'r':return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
    case 's':
    {
        // short
        helper = checkKeyword(1, 4, "hort", TOKEN_SHORT);
        if (helper == TOKEN_SHORT)return helper;

        // signed
        helper = checkKeyword(1, 5, "igned", TOKEN_SIGNED);
        if (helper == TOKEN_SIGNED)return helper;

        // sizeof
        helper = checkKeyword(1, 5, "izeof", TOKEN_SIZEOF);
        if (helper == TOKEN_SIZEOF)return helper;

        // struct
        helper = checkKeyword(1, 5, "truct", TOKEN_STRUCT);
        if (helper == TOKEN_STRUCT)return helper;

        // switch
        helper = checkKeyword(1, 5, "witch", TOKEN_SWITCH);
        
        return helper;
    }
    case 't':return checkKeyword(1, 6, "ypedef", TOKEN_TYPEDEF);
    case 'u':
    {
        // unsigned
        helper = checkKeyword(1, 7, "nsigned", TOKEN_UNSIGNED);
        if (helper == TOKEN_UNSIGNED)return helper;

        // union
        helper = checkKeyword(1, 4, "nion", TOKEN_UNION);

        return helper;
    }
    case 'v':return checkKeyword(1, 3, "oid", TOKEN_VOID);
    case 'w':return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }

    // identifier
    return TOKEN_IDENTIFIER;
}

static Token identifier()
{
    // IDENTIFIER����: ��ĸ�����ֺ��»���
    while (isAlpha(peek()) || isDigit(peek()))
    {
        advance();
    }
    // ������Token�����Ǳ�ʶ��, Ҳ�����ǹؼ���, identifierType()������ȷ��Token���͵�
    return makeToken(identifierType());
}

static Token number()
{
    // ����������ǽ�NUMBER�Ĺ���������:
    // 1. NUMBER���԰������ֺ����һ��'.'��
    // 2. '.'��ǰ��Ҫ������
    // 3. '.'�ź���ҲҪ������
    // ��Щ���ǺϷ���NUMBER: 123, 3.14
    // ��Щ���ǲ��Ϸ���NUMBER: 123., .14
    while (isDigit(peek()))
    {
        advance();
    }// peek Ϊ . ���߽�β

    if (peek() == '.')
    {
        advance();
        
        while (peek() && peek() != '\n')
        {
            // '.'�ĺ��治������ ����errorToken
            if (!isDigit(peek()))
            {
                /*
                    BUG
                    ������ 3..ʱ��һ��'\n'�ַ� ���´�ӡ����໻��һ��
                */
                return errorToken(scanner.start);
            }
            advance();
        }
    }
 
    return makeToken(TOKEN_NUMBER);
}

static Token string()
{
    // �ַ�����"��ͷ����"��β�����Ҳ��ܿ���
}

static Token character()
{
    // �ַ�'��ͷ����'��β�����Ҳ��ܿ���   
}

/***************************************************************************************
 *                                   	�ִ�										   *
 ***************************************************************************************/

Token scanToken()
{
    // ����ǰ�ÿհ��ַ���ע��
    skipWhitespace();
    // ��¼��һ��Token����ʼλ��
    scanner.start = scanner.current;

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    char c = advance();
    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();

    switch (c)
    {
        // single-character tokens
    case '(':return makeToken(TOKEN_LEFT_PAREN);
    case ')':return makeToken(TOKEN_RIGHT_PAREN);
    case '[':return makeToken(TOKEN_LEFT_BRACKET);
    case ']':return makeToken(TOKEN_RIGHT_BRACKET);
    case '{':return makeToken(TOKEN_LEFT_BRACE);
    case '}':return makeToken(TOKEN_RIGHT_BRACE);
    case ',':return makeToken(TOKEN_COMMA);
    case '.':return makeToken(TOKEN_DOT);
    case ';':return makeToken(TOKEN_SEMICOLON);
    case '~':return makeToken(TOKEN_TILDE);

        // one or two characters tokens
    case '+':
    {
        if (match('+')) return makeToken(TOKEN_PLUS_PLUS);
        else if (match('=')) return makeToken(TOKEN_PLUS_EQUAL);
        else return makeToken(TOKEN_PLUS);
    }
    case '-':
    {
        if (match('-'))return makeToken(TOKEN_MINUS_MINUS);
        else if (match('='))return makeToken(TOKEN_MINUS_EQUAL);
        else if (match('>'))return makeToken(TOKEN_MINUS_GREATER);
        else return makeToken(TOKEN_MINUS);
    }
    case '*':
    {
        if (match('='))return makeToken(TOKEN_STAR_EQUAL);
        else return makeToken(TOKEN_STAR);
    }
    case '/':
    {
        if (match('='))return makeToken(TOKEN_SLASH_EQUAL);
        else return makeToken(TOKEN_SLASH);
    }
    case '%':
    {
        if (match('='))return makeToken(TOKEN_PERCENT);
        else return makeToken(TOKEN_PERCENT_EQUAL);
    }
    case '&':
    {
        if (match('='))return makeToken(TOKEN_AMPER_EQUAL);
        else if (match('&'))return makeToken(TOKEN_AMPER_EQUAL);
        else return makeToken(TOKEN_AMPER_AMPER);
    }
    case '|':
    {
        if (match('='))return makeToken(TOKEN_PIPE_EQUAL);
        else if (match('|'))return makeToken(TOKEN_PIPE_PIPE);
        else return makeToken(TOKEN_PIPE);
    }
    case '^':
    {
        if (match('='))return makeToken(TOKEN_HAT_EQUAL);
        else return makeToken(TOKEN_HAT);
    }
    case '=':
    {
        if (match('='))return makeToken(TOKEN_EQUAL_EQUAL);
        else return makeToken(TOKEN_EQUAL);
    }
    case '!':
    {
        if (match('='))return makeToken(TOKEN_BANG_EQUAL);
        else return makeToken(TOKEN_BANG);
    }
    case '<':
    {
        if (match('='))return makeToken(TOKEN_LESS_EQUAL);
        else if (match('<'))return makeToken(TOKEN_LESS_LESS);
        else return makeToken(TOKEN_LESS);
    }
    case '>':
    {
        if (match('='))return makeToken(TOKEN_GREATER_EQUAL);
        else if (match('>'))return makeToken(TOKEN_GREAER_GREATER);
        else return makeToken(TOKEN_GREATER);
    }
        // various-character tokens
    case '"': return string();
    case '\'': return character();
    }

    return errorToken("Unexpected character.");
}