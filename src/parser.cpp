#include "parser.hpp"
#include "connection.hpp"
#include "header.hpp"
#include "mgparser.hpp"
#include <iostream>
#include <string_view>

using namespace std;

extern const std::map<std::string, HeaderKey> r_header_map;

Parser::Parser(shared_ptr<Connection> connection)
    : settings_(new mg_settings_t), parser_(new mg_parser_t),
      conn_(connection) {}

int Parser::parser_init(std::string req) {
  req = req_;
  parser_->mg_settings_init(settings_.get());
  mg_return_t err = parser_->mg_parser_execute(req);
  if (err == mg_return_t::SUCCESS) {
    cout << "Succesfully Parsed!";
    return 0;
  } else {
    cout << "Parser Error " << endl;
    return 1;
  }
}

mg_return_t parser::handle_method(std::string_view method) {
  cout << "Method : " << method << endl;
  return mg_return_t::SUCCESS;
}

mg_return_t parser::handle_version(const int &major, const int &minor) {
  cout << "Major : " << major << "Minor : " << minor << endl;
  return mg_return_t::SUCCESS;
}

mg_return_t parser::handle_url(std::string_view url) {
  cout << "Url : " << url << endl;
  return mg_return_t::SUCCESS;
}

mg_return_t parser::handle_header(std::string_view field,
                                  std::string_view value) {
  cout << "Field : " << field << " Value : " << value << endl;
  return mg_return_t::SUCCESS;
}

mg_return_t parser::handle_body(std::string_view body) {
  cout << "Body : " << body << endl;
  return mg_return_t::SUCCESS;
}