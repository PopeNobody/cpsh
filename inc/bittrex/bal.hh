#ifndef bittrex_bal_hh
#define bittrex_bal_hh bittrex_bal_hh

#include <bittrex/ops.hh>

#include <fmt.hh>
#include <pqxx.hh>
#include <dbg.hh>
#include <bittrex/val.hh>

namespace bittrex
{
  using pqxx::connection;
  class bal_t;
  class bal_m : public map<sym_t,bal_t> {
  };

  class bal_t : public fmt::streamable_tag
  {
    public:

    bal_t(
        const char* sym="",
        double bal=0,
        double pend=0
        );
    bal_t(
        const val_t &bal,
        const val_t &pend
        );
    bal_t(
        const sym_t& sym,
        double bal,
        double pend=0
        );

    struct data_t {
      val_t bal;
      val_t pend;
      data_t(const val_t &bal, const val_t &pend)
        : bal(bal), pend(pend)
      {
      };
      data_t(
        const sym_t &sym,
        double bal,
        double pend
        )
        :bal(sym,bal), pend(sym,pend)
      {
      };
    };
    bool changed(const bal_t &rhs) const
    {
      xassert(sym()==rhs.sym());
      if( data.bal != rhs.data.bal )
        return true;
      if( data.pend != rhs.data.pend )
        return true;
      return false;
    };
    data_t data;
    const sym_t &sym() const { return data.bal.sym(); };
    const val_t &bal() const { return data.bal; };
    const val_t &pend() const { return data.pend; };
    val_t avail() const { return bal()-pend(); };

		ostream &stream(ostream &lhs) const;

    static string url();
    static bal_m list;
    static bal_m &load(bool force=false);
    static void dbstore(connection &con, const bal_m &bals);
    static void dbstore(connection &con);
    static bal_m dbload(connection &con);
  };
};
#endif
