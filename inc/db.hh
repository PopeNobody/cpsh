#ifndef db_hh
#define db_hh db_hh

#include <coinfwd.hh>
#include <pqxx/connection.hxx>
#include <pqxx/transaction.hxx>
#include <pqxx/result.hxx>
#include <pqxx/field.hxx>
#include <pqxx/tuple.hxx>
#include <json.hh>
#include <value.hh>
#include <pqxx.hh>

using namespace pqxx;
namespace db {
  using std::string;
  using value::date_t;
  string connect_string(string userstr = string());
  string dbquote(const json &json);
  string dbquote(const date_t &date);
  string dbquote(const string &str);
  string dbquote(double val);
  string dbquote(bool val);
  typedef vector<int> int_v;
  string single_row(connection &con, const string &sql, const int_v &wids=int_v());
  string single_row(work &work, const string &sql, const int_v &wids=int_v());
  string single_cell(connection &con, const string &sql);
  string single_cell(work &work, const string &sql);
  string single_query(connection &con, const string &sql);
  string single_query(work &work, const string &sql);
  string json_wrap_query(const string &sql);
  vector<string> json_query(connection &con, const string &sql);
  result exec(work &work, const string &sql);
  struct dbif {
    connection &con;
    dbif(connection &con)
      : con(con)
    {
    };
  };
};
#endif
