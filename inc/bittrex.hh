#ifndef bittrex_hh
#define bittrex_hh bittrex_hh

#include <json.hh>
#include <fmt.hh>
#include <dbg.hh>

#include <bittrex/mkt.hh>
#include <bittrex/bal.hh>
#include <bittrex/goal.hh>
#include <bittrex/order.hh>

namespace db {
  struct db_if;
};
namespace bittrex {

  void  from_json(  const  json&  j,            bal_m&    p  );
  void  to_json(           json&  j,     const  bal_m&    p  );

  void  from_json(  const  json&  j,            bal_t&    p  );
  void  to_json(           json&  j,     const  bal_t&    p  );

  void  from_json(  const  json&  j,            mkt_m&    p  );
  void  to_json(           json&  j,     const  mkt_m&    p  );

  void  from_json(  const  json&  j,            mkt_t&    p  );
  void  to_json(           json&  j,     const  mkt_t&    p  );

  void  from_json(  const  json&  j,            goal_m&   p  );
  void  to_json(           json&  j,     const  goal_m&   p  );

  void  from_json(  const  json&  j,            goal_t&   p  );
  void  to_json(           json&  j,     const  goal_t&   p  );

  void  from_json(  const  json&  j,            order_m&  p  );
  void  to_json(           json&  j,     const  order_m&  p  );

  void  from_json(  const  json&  j,            order_t&  p  );
  void  to_json(           json&  j,     const  order_t&  p  );


  const json load_json_url( const string& url, const string& save_to );
  const char *to_string( nlohmann::detail::value_t ty);
};
namespace value {
  void to_json(json& j, const date_t& p);
  void from_json(const json& j, date_t& p);
};
#endif
#ifdef item
#error item defined!
#endif
