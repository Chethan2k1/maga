#include "parser.hpp"
#include "connection.hpp"
#include "header.hpp"
#include "mgparser.hpp"
#include <iostream>
#include <memory>
#include <string_view>

using namespace std;

extern const std::map<std::string, HeaderKey> r_header_map;

static const size_t Parse2Size_t(string_view str) {
  size_t val = 0;
  while (!str.empty()) {
    val *= 10;
    val += (str[0] - '0');
    str = str.substr(1);
  }

  return val;
}

Parser::Parser(shared_ptr<Connection> connection)
    : conn_(connection), parser_(new mg_parser_t<Parser *>),
      settings_(new mg_settings_t<Parser *>) {}

int Parser::parser_init(std::string&& req) {
  req_ = std::move(req);
  parser_->mg_settings_init(settings_);

  parser_->sett->handle_method = parser::handle_method;
  parser_->sett->handle_version = parser::handle_version;
  parser_->sett->handle_url = parser::handle_url;
  parser_->sett->handle_header = parser::handle_header;
  parser_->sett->handle_body = parser::handle_body;

  // should post here?
  mg_return_t err = parser_->mg_parser_execute(req_, this);
  if (err == mg_return_t::SUCCESS) {
    cout << "Succesfully Parsed!" << endl;
    return 0;
  } else if (err == mg_return_t::PAUSE) {
    cout << "Paused" << endl;
    return 0;
  } else {
    cout << "Parser Error " << endl;
    return err;
  }
}

int Parser::parser_resume() {
  mg_return_t err = parser_->mg_parser_resume(req_, this);
  if (err == mg_return_t::SUCCESS) {
    cout << "Succesfully Parsed!" << endl;
    return 0;
  } else {
    cout << "Parser Error " << endl;
    return err;
  }
}

mg_return_t parser::handle_method(std::string_view method, Parser *conn) {
  cout << "Method : " << method << endl;
  return mg_return_t::SUCCESS;
}

mg_return_t parser::handle_version(const int &major, const int &minor,
                                   Parser *conn) {
  cout << "Major : " << major << "Minor : " << minor << endl;
  return mg_return_t::SUCCESS;
}

mg_return_t parser::handle_url(std::string_view url, Parser *conn) {
  cout << "Url : " << url << endl;
  return mg_return_t::SUCCESS;
}

mg_return_t parser::handle_header(std::string_view field,
                                  std::string_view value, Parser *parser) {
  cout << "Field : " << field << " Value : " << value << endl;
  if (field == "Content-Length") {
    cout << "Read this much : " << Parse2Size_t(value) << endl;
    (parser->conn_)
        .lock()
        ->ioc_.post([parser, conn = (parser->conn_).lock(),
                     bytes = Parse2Size_t(value)]() {
          conn->read_body(parser->req_, bytes, [conn]() mutable {
            cout << "Finished Reading Body " << endl;
            conn->parser_->parser_resume();
          });
        });

    return mg_return_t::PAUSE;
  }

  return mg_return_t::SUCCESS;
}

mg_return_t parser::handle_body(std::string_view body, Parser *conn) {
  cout << "Body : " << body << endl;
  return mg_return_t::SUCCESS;
}