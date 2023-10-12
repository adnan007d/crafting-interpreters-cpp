#include "scanner.h"
#include "lox.h"
#include "token_type.h"

const std::vector<Token> &Scanner::scanTokens() {
  while (!isAtEnd()) {
    start = current;
    scanToken();
  }

  tokens.emplace_back(TokenType::_EOF, "", "", line);
  return tokens;
}

void Scanner::scanToken() {
  char c = advance();
  switch (c) {
  case '(':
    addToken(TokenType::LEFT_PAREN);
    break;
  case ')':
    addToken(TokenType::RIGHT_PAREN);
    break;
  case '{':
    addToken(TokenType::LEFT_BRACE);
    break;
  case '}':
    addToken(TokenType::RIGHT_BRACE);
    break;
  case ',':
    addToken(TokenType::COMMA);
    break;
  case '.':
    addToken(TokenType::DOT);
    break;
  case '-':
    addToken(TokenType::MINUS);
    break;
  case '+':
    addToken(TokenType::PLUS);
    break;
  case ';':
    addToken(TokenType::SEMICOLON);
    break;
  case '*':
    addToken(TokenType::STAR);
    break;
  case '!':
    addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
    break;
  case '=':
    addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
    break;
  case '<':
    addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
    break;
  case '>':
    addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
    break;
  case '/':
    if (match('/')) {
      while (peek() != '\n' && !isAtEnd()) {
        advance();
      }
    } else {
      addToken(TokenType::SLASH);
    }
    break;

  // Intentional fall through and skipping whitespace
  case ' ':
  case '\r':
  case '\t':
    break;

  case '\n':
    ++line;
    break;

  case '"':
    parseString();
    break;
  default:
    Lox::error(line, "Unexpected character.");
    break;
  }
}

bool Scanner::isAtEnd() { return current >= source.size(); }

char Scanner::advance() { return source.at(current++); }

void Scanner::addToken(TokenType type) { addToken(type, nullptr); }

void Scanner::addToken(TokenType type, std::string literal) {
  std::string text = source.substr(start, current);
  tokens.emplace_back(type, std::move(text), std::move(literal), line);
}

/**
 * Returns true if its an comparison operator (<=, ==, >=) else false
 * Also used for / to check if it is a comment or not
 */
bool Scanner::match(char expected) {
  if (isAtEnd()) {
    return false;
  }

  if (source.at(current) != expected) {
    return false;
  }

  ++current;

  return true;
}

char Scanner ::peek() {
  if (isAtEnd()) {
    return '\0';
  }
  return source.at(current);
}

void Scanner::parseString() {
  while (peek() != '"' && isAtEnd()) {
    if (peek() == '\n') {
      ++line;
    }
    advance();
  }

  if (isAtEnd()) {
    Lox::error(line, "Unterminated string");
    return;
  }

  advance();

  std::string value = source.substr(start + 1, current - 1);
  addToken(TokenType::STRING, value);
}
