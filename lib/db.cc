#include <fcntl.h>
#include <iostream>
#include <json.hh>
#include <db.hh>
#include <time.h>
#include <util.hh>
#include <web_api.hh>
#include <dbg.hh>
#include <fmt.hh>

#include <pqxx/connection.hxx>
#include <pqxx/result.hxx>
#include <pqxx/transaction.hxx>

namespace {
  bool trace_sql=false;
};
result db::exec(work &work, const string &sql)
{
  if(trace_sql)
    cout << sql << endl;
  try {
    return work.exec(sql);
  } catch ( exception &e ) {
    xtrace( e.what() );
    throw;
  };
};
string db::connect_string(string db)
{
  if(!db.size()) {
    auto str=getenv("PGDATABASE");
    if(str)
      db=str;
  } 
  if(db.size())
    db="dbname="+db;
  return db;
};
string db::json_wrap_query(const string &sql)
{
  return
    "SELECT array_to_json(array_agg(row_to_json (r))) FROM ( \n"
    "   "+sql+"\n"
    "  ) r;                                                  \n"
    ;
};
string db::dbquote(bool val)
{
  return (val?"true":"false");
};
string db::dbquote(const date_t &date)
{
  return dbquote((string)date);
};
string db::dbquote(const json &json)
{
  if(json.is_null()) {
    return "null";
  } else if (json.is_string()) {
    string val;
    val=json;
    val=dbquote(val);
    return val;
  } else {
    ostringstream str;
    str << json;
    return str.str();
  };
};
string db::dbquote(double val)
{
  return lexical_cast<string>(val);
};
string db::dbquote(const string &val)
{
  char buf[val.length()*2+1];
  int i=0;
  buf[i++]='\'';
  for ( auto c : val ) {
    if( c == '\'' )
      buf[i++]=c;
    buf[i++]=c;
  };
  buf[i++]='\'';
  buf[i++]=0;
  return buf;
};
string db::single_query(work &work, const string &sql) {
  return single_row(work,sql);
};
string db::single_query(connection &con, const string &sql) {
  return single_row(con,sql);
};
string db::single_cell(connection &con, const string &sql) {
    work work(con);
    return single_cell(work,sql);
}
string db::single_row(connection &con, const string &sql, const vector<int> &wvec) {
  work work(con);
  return single_row(work,sql,wvec);
};
string db::single_row(work &work, const string &sql, const vector<int> &wvec) {
  try {
    ostringstream text;
    auto res=work.exec(sql);
    int i=0;
    for( auto row : res ) {
      for ( auto col : row ) {
        text << " | ";
        if(i<wvec.size()) {
          text << setw(wvec[i]);
          i++;
        };
        text << col.c_str();
      };
      text << " | ";
      return text.str();
    };
    throw runtime_error("no rows returned");
  } catch ( exception &ex ) {
    cout << ex.what() << endl;
    throw;
  };

};
string db::single_cell(work &work, const string &sql) {
  try {
    auto res=work.exec(sql);
    for( auto row : res ) {
      for ( auto col : row ) {
        return col.c_str();
      };
    };
    throw runtime_error("no rows returned");
  } catch ( exception &ex ) {
    cout << ex.what() << endl;
    throw;
  };

};
vector<string> db::json_query(connection &con, const string &sql)
{
  string wrap = " select row_to_json(r) from (\n"+sql+") r";
  vector<string> rows;
  try {
    work work(con);
    auto res=work.exec(wrap);
    for( auto row : res ) {
      for ( auto col : row ) {
        rows.push_back(col.c_str());
      };
    };
    return rows;
  } catch ( exception &ex ) {
    cout << ex.what() << endl;
    throw;
  };
};
