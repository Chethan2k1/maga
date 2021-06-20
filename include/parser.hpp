#pragma once

#include "header.hpp"
#include "mgparser.hpp"
#include <istream>
#include <memory>
#include <string_view>

class Connection;

class Parser {
public:
  std::string req_;
  std::weak_ptr<Connection> conn_;
  mg_parser_t<Parser *> *parser_;
  explicit Parser(std::shared_ptr<Connection> connection);
  int parser_init(std::string&& req);
  int parser_resume();

private:
  mg_settings_t<Parser *> *settings_;
  void header_value_handler(const std::string &header_val);
};

namespace parser {
mg_return_t handle_method(std::string_view, Parser *);
mg_return_t handle_version(const int &, const int &, Parser *);
mg_return_t handle_url(std::string_view, Parser *);
mg_return_t handle_header(std::string_view, std::string_view, Parser *);
mg_return_t handle_body(std::string_view, Parser *);
} // namespace parser