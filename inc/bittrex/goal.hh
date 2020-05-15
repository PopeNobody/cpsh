#ifndef bittrex_goal_hh
#define bittrex_goal_hh bittrex_goal_hh

#include <bittrex/ops.hh>

#include <fmt.hh>
#include <db.hh>
#include <bittrex/sym.hh>

namespace bittrex {
  class goal_t;
  class goal_m : public std::map<sym_t,goal_t> 
  {
  };
  class goal_t : public fmt::streamable_tag
  {
    public:
    struct data_t {
      sym_t sym;
      double weight;
      double pct;
    };
    data_t data;
    goal_t();
    goal_t(const goal_t &goal);
    goal_t(const char* sym, double weight, double pct=0);
    goal_t(const sym_t& sym, double weight, double pct=0)
    {
      data.sym=sym;
      data.weight=weight;
      data.pct=pct;
    };
		~goal_t();
		ostream &stream(ostream &lhs) const;
    void sym(sym_t &sym)
    {
      data.sym=sym;
    };;
    const sym_t &sym() const
    {
      return data.sym;
    };;
    double pct() const
    {
      return data.pct;
    };
    void pct(double pct)
    {
      data.pct=pct;
    };
    double weight() const
    {
      return data.weight;
    };
    // unlike other classes, goal_t leaves its
    // data in the temporary table.

    static void dbstore(connection &con, const goal_m &goals);
    static void dbstore(connection &con);
    static goal_m list;
    static goal_m &load( connection& con );
  };
};
#endif
