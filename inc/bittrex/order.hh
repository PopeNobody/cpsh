#include <value/date.hh>
#include <bittrex/attr.hh>
#include <db.hh>

#ifndef bittrex_order_hh
#define bittrex_order_hh bittrex_order_hh

namespace bittrex {
  using value::date_t;

  class order_m;
  const static string api_url = "https://bittrex.com/api/v1.1/";
#define order_attrs()                                                  \
  item(  string,  uuid,                 "OrderUuid"          )  \
  item(  date_t,  closed,               "Closed"             )  \
  item(  double,  commission,           "Commission"         )  \
  item(  string,  condition,            "Condition"          )  \
  item(  double,  condition_target,     "ConditionTarget"    )  \
  item(  string,  exchange,             "Exchange"           )  \
  item(  bool,    immediate_or_cancel,  "ImmediateOrCancel"  )  \
  item(  bool,    is_conditional,       "IsConditional"      )  \
  item(  double,  xlimit,               "Limit"              )  \
  item(  string,  order_type,           "OrderType"          )  \
  item(  double,  price,                "Price"              )  \
  item(  double,  price_per_unit,       "PricePerUnit"       )  \
  item(  double,  quantity,             "Quantity"           )  \
  item(  double,  quantity_remaining,   "QuantityRemaining"  )  \
  item(  date_t,  stamptime,            "TimeStamp"          )

  struct order_t : public fmt::streamable_tag {
    struct data_t {
#define item( t, x, y )  t x;
      order_attrs();
#undef item
    } data;
#define item( t, x, y ) const t &x() const{ return data.x; };
      order_attrs();
#undef item
    ~order_t();
    ostream &stream(ostream &lhs) const;
    order_t();
    static order_m dbload( connection& con );
    static void dbstore(connection &con, const order_m &ords);
    static void dbstore(connection &con);
    static order_m &load();
    static order_m list;
    static order_m read_csv(const string &csvfile);
  };
  class order_m : public map<string,order_t>
  {
  };
}
#endif
