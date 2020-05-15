#ifndef attr_hh
#define attr_hh attr_hh

#include <bittrex/ops.hh>

namespace bittrex {
  struct attr_t;
  class attr_m : public map<string,attr_t> {
  };
  struct attr_t {
    string sql;
    string json;
    string mem;
    string type;
    attr_t()
    {
    };
    attr_t(const string &sql, const string &json, const string &mem, const string &type)
      : sql(sql), json(json), mem(mem), type(type)
    {
    };
  };
};

#endif
