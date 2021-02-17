#pragma once

#include "header.hpp"
#include "mgparser.hpp"
#include <istream>
#include <memory>
#include <string_view>

class Connection;

class Parser {
public:
  HeaderKey curr_header_field_;
  std::weak_ptr<Connection> conn_;
  explicit Parser(std::shared_ptr<Connection> connection);
  int parser_init(std::string req);

private:
  std::string req_;
  std::unique_ptr<mg_parser_t> parser_;
  std::unique_ptr<mg_settings_t> settings_;

  void header_value_handler(const std::string &header_val);
};

namespace parser {
mg_return_t handle_method(std::string_view);
mg_return_t handle_version(const int &, const int &);
mg_return_t handle_url(std::string_view);
mg_return_t handle_header(std::string_view, std::string_view);
mg_return_t handle_body(std::string_view);
} // namespace parser