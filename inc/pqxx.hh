#ifndef pqxx_hh
#define pqxx_hh pqxx_hh

#include <pqxx/connection.hxx>
#include <pqxx/transaction.hxx>
#include <pqxx/result.hxx>

namespace pqxx
{
  class field;
  class tuple;
  class result;

  inline ostream &operator<<(ostream &lhs, const pqxx::field &rhs)
  {
    return lhs << (string("{")+rhs.c_str()+"}");
  };
  inline ostream &operator<<(ostream &lhs, const pqxx::tuple &rhs)
  {
    lhs << "{" << endl;
    for( auto field : rhs ) {
      lhs << field << endl;
    };
    return lhs << "}" << endl;
  };
  inline ostream &operator<<(ostream &lhs, const pqxx::result &rhs)
  {
    lhs << "{" << endl;
    for( auto field : rhs ) {
      lhs << field << endl;
    };
    return lhs << "}" << endl;
  };
};
#endif
