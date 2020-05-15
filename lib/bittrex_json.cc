#include <bittrex.hh>
#include <dbg.hh>
#include <json.hh>


using nlohmann::detail::value_t;
namespace bittrex
{
  void to_json( json& j, const double& v );
  void from_json( const json& j, double& v );

  void to_json( json& j, const sym_t& v );
  void from_json( const json& j, sym_t& v );

  void to_json( json& j, const string& v );
  void from_json( const json& j, string& v );

  void to_json( json& j, const bool& v );
  void from_json( const json& j, bool& v );

  const char *to_string(value_t ty);
}; // namespace bittrex

const char *bittrex::to_string(value_t ty) {
  switch(ty) {
    case value_t::null: return "null";
    case value_t::object: return "object";
    case value_t::array: return "array";
    case value_t::string: return "string";
    case value_t::boolean: return "boolean";
    case value_t::number_integer: return "number_integer";
    case value_t::number_unsigned: return "number_unsigned";
    case value_t::number_float: return "number_float";
    case value_t::discarded: return "discarded";
  };
  return "idk";
};

void bittrex::from_json( const json& j, double& v )
{
  if( j.is_null() )
    v=0;
  else
    j.get_to(v);
};
void bittrex::from_json( const json& j, string& v )
{
  if( j.is_null() )
    v.clear();
  else
    j.get_to(v);
};
void bittrex::to_json( json& j, const bool& v )
{
  j=v;
};
void bittrex::from_json( const json& j, bool& v )
{
  if(j.is_null())
    v=false;
  else
    j.get_to(v);
};
void bittrex::from_json( const json& js, bittrex::mkt_m& ms )
{
  mkt_m mkts;
  for( auto j : js ) {
    mkt_t mkt;
    try {
      from_json(j,mkt);
      mkts[mkt.name()]=mkt;
    } catch ( const exception &e ) {
      cout << e << endl;
      cout << "while parsing: " << endl;
      cout << setw(4) << j << endl;
      cout << "pressing on" << endl;
    };
  };
  ms=mkts;

};
vector<string> mkt_bl={
  "BTC-URAC",
  "BTC-MOC",
  "ETH-OGN",
  "USDT-VDX",
  "BTC-BKX",
  "BTC-UPP",
  "BTC-FNB",
  "BTC-TSHP",
  "BTC-FRSP",
  "BTC-DCT",
  "ETH-BNT",
  "USDT-BRZ",
  "BTC-BRZ",
  "BTC-DNA",
  "USDT-DNA"
};
void bittrex::from_json( const json& j, bittrex::mkt_t& p )
{
  string name;
  double bid, ask, last;

  from_json(j[ "MarketName" ], name );
  if(find(mkt_bl.begin(),mkt_bl.end(),name)!=mkt_bl.end())
    return;
  vector<string> pts=util::split('-',name);
  sym_t cur(pts[0]);
  sym_t sym(pts[1]);
  from_json(j[ "Bid" ], bid );
  from_json(j[ "Ask" ], ask );
  if( !bid || !ask || !last ) {
    cout << "skipping market: " << name << endl;
    return;
  };
  from_json(j[ "Last" ], last );

  xassert(bid<ask);
  bittrex::mkt_t mkt(cur,sym,bid,ask,last);
  xassert(mkt.sym()==sym);
  xassert(mkt.cur()==cur);
  xassert(mkt.name()==name);
  p=mkt;

};
void bittrex::to_json( json& j, const bittrex::mkt_t& p )
{
  to_json( j, p.name() );
  to_json( j, p.ask() );
  to_json( j, p.bid() );
  to_json( j, p.last() );
};
void bittrex::from_json( const json& j, goal_t& p )
{
  from_json( j.at( "sym" ), p.data.sym );
  from_json( j.at( "weight" ), p.data.weight );
};
void value::from_json( const json& j, date_t& p )
{
  string str = j;
  p = date_t( str );
};
void bittrex::from_json( const json& j, sym_t& p )
{
  string str;
  j.get_to(str);
  p=sym_t(str);
};
void bittrex::from_json( const json& j, bal_t& p )
{
  sym_t sym;
  double bal,pend,ava;
  from_json(j.at("Currency"),sym);
  j.at(  "Balance"    ).get_to(  bal    );
  j.at(  "Available"  ).get_to(  ava  );
  j.at(  "Pending"    ).get_to(  pend   );
  xassert(bal-pend==ava);
  p=bal_t(val_t(sym,bal),val_t(sym,pend));
};

void bittrex::to_json( json& j, const sym_t& p )
{
  j = (string)p;
};
void bittrex::to_json( json& j, const bal_t& p )
{
  j[ "Currency" ] = p.sym();
  j[ "Balance" ] =  p.bal().val();
  j[ "Available" ] = p.avail().val();
  j[ "Pending" ] = p.pend().val();
};
void bittrex::to_json( json& j, const goal_t& p )
{
  j[ "sym" ] = ( p.data.sym );
  j[ "weight" ] = ( p.data.weight );
};
void value::to_json( json& j, const date_t& p )
{
  j = (string)p;
};
void bittrex::to_json( json& j, const string& v )
{
  j = v;
};
void bittrex::to_json( json& j, const double& v )
{
  j = v;
};
void bittrex::to_json( json& j, const order_t& o )
{
  to_json( j[ "OrderUuid" ], o.data.uuid );
  to_json( j[ "Closed" ], o.data.closed );
  to_json( j[ "Commission" ], o.data.commission );
  to_json( j[ "Condition" ], o.data.condition );
  to_json( j[ "ConditionTarget" ], o.data.condition_target );
  to_json( j[ "Exchange" ], o.data.exchange );
  to_json( j[ "ImmediateOrCancel" ], o.data.immediate_or_cancel );
  to_json( j[ "IsConditional" ], o.data.is_conditional );
  to_json( j[ "Limit" ], o.data.xlimit );
  to_json( j[ "OrderType" ], o.data.order_type );
  to_json( j[ "Price" ], o.data.price );
  to_json( j[ "PricePerUnit" ], o.data.price_per_unit );
  to_json( j[ "Quantity" ], o.data.quantity );
  to_json( j[ "QuantityRemaining" ], o.data.quantity_remaining );
  to_json( j[ "TimeStamp" ], o.data.stamptime );
};
void bittrex::from_json( const json& j, goal_m& m )
{
  for ( auto& jj : j )
  {
    goal_t o;
    from_json( jj, o );
    m[ o.sym() ] = o;
  }
};
void bittrex::to_json( json& j, const goal_m& l )
{
  for ( auto& pr : l )
    j[ (string)pr.first ] = pr.second;
};
void bittrex::to_json( json& j, const bal_m& l )
{
  for ( auto& pr : l )
    j[ (string)pr.first ] = pr.second;
};
void bittrex::from_json( const json& j, bal_m& l )
{
  for ( auto& jj : j )
  {
    bal_t o;
    from_json( jj, o );
    l[ o.sym() ] = o;
  }
};
void bittrex::from_json( const json& j, order_m& l )
{
  for ( auto& jj : j )
  {
    order_t o;
    from_json( jj, o );
    l[ o.uuid() ] = o;
  }
};
void bittrex::to_json( json& j, const order_m& l )
{
  for ( auto& pr : l )
    j[ pr.first ] = pr.second;
};
void bittrex::from_json( const json& j, order_t& o )
{
  from_json(j[ "OrderUuid" ], o.data.uuid);
  from_json(j[ "Closed" ], o.data.closed);
  from_json(j[ "Commission" ], o.data.commission);
  from_json(j[ "Condition" ], o.data.condition);
  from_json(j[ "ConditionTarget" ], o.data.condition_target);
  from_json(j[ "Exchange" ], o.data.exchange);
  from_json(j[ "ImmediateOrCancel" ], o.data.immediate_or_cancel);
  from_json(j[ "IsConditional" ], o.data.is_conditional);
  from_json(j[ "Limit" ], o.data.xlimit);
  from_json(j[ "OrderType" ], o.data.order_type);
  from_json(j[ "Price" ], o.data.price);
  from_json(j[ "PricePerUnit" ], o.data.price_per_unit);
  from_json(j[ "Quantity" ], o.data.quantity);
  from_json(j[ "QuantityRemaining" ], o.data.quantity_remaining);
  from_json(j[ "TimeStamp" ], o.data.stamptime);
};
