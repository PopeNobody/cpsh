#ifndef bittrex_mkt_hh
#define bittrex_mkt_hh bittrex_mkt_hh

#include <bittrex/ops.hh>

#include <fmt.hh>
#include <bittrex/val.hh>
#include <bittrex/pri.hh>

namespace db {
  struct dbif;
};
namespace bittrex {
  struct mkt_t;
  class mkt_m : public map<string,mkt_t>
  {
  };
  class mkt_t : public fmt::streamable_tag
  {

#define mkt_attrs() \
    item(  val_t,  bid,   "Bid"         ) \
    item(  val_t,  ask,   "Ask"         ) \
    item(  val_t,  last,  "Last"        ) \
    ;
    struct data_t {
      sym_t cur;
      sym_t sym;
      double bid;
      double ask;
      double last;
      data_t();
    };
    data_t  data;
    public:
    string  name()   const  {  return  string(cur())+"-"+sym(); };
    sym_t   cur()    const  {  return  data.cur;                };
    sym_t   sym()    const  {  return  data.sym;                };
    double   bid()    const  {  return  data.bid;               };
    double   ask()    const  {  return  data.ask;               };
    double   last()   const  {  return  data.last;              };
    double   price()  const  {  return (data.bid+data.ask)/2;   };
    mkt_t(const mkt_t &rhs);
    mkt_t(
        const sym_t &cur,
        const sym_t &sym,
        double bid,
        double ask,
        double last );
    mkt_t();
    bool changed(const mkt_t &rhs) const;
    string to_sql() const;
		ostream &stream(ostream &lhs) const;
    val_t convert(const val_t &rhs, bool neutral) const;

    static string url();
    static mkt_m list;
    static mkt_m &load(bool force=false);
    static void dbstore(const db::dbif &con, const mkt_m &mkts);
    static void dbstore(const db::dbif &con);
    static mkt_m dbload(const db::dbif &con);
    static const mkt_t &get( const sym_t &sym1, const sym_t &sym2 );
    static val_t get_price( string &sym1, string &sym2 );
    static val_t convert(const val_t &qty, const sym_t &t, bool neutral);
  };
};
#endif
