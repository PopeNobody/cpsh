#include <bittrex/mkt.hh>
#include <bittrex.hh>
#include <util.hh>
#include <dbg.hh>

bittrex::mkt_t::data_t::data_t()
{
};
bittrex::mkt_m bittrex::mkt_t::list;
bittrex::mkt_m &bittrex::mkt_t::load(bool force)
{
  if( force || list.size()==0 ) {
    mkt_m res;
    from_json(load_json_url( url(), "info/mkt.json" ), res); 
    cout << res.size() << " mkts loaded" << endl;
    list=res;
  };
  return list;
};
bittrex::mkt_t::mkt_t()
{
};
bittrex::mkt_t::mkt_t(const mkt_t &rhs)
  :data(rhs.data)
{
  xassert(rhs.cur()==cur());
  xassert(rhs.sym()==sym());
  xassert(rhs.bid()==bid());
  xassert(rhs.ask()==ask());
  xassert(rhs.last()==last());
}; 
//   bittrex::mkt_t::mkt_t(
//       const sym_t &cur,
//       const sym_t &sym,
//       double ask,
//       double bid,
//       double last )
//   {
//     xthrowre("fixme");
//   };
const bittrex::mkt_t &bittrex::mkt_t::get(const sym_t &lhs, const sym_t &rhs)
{
  load(false);
  string name1=string(rhs)+"-"+lhs;
  string name2=string(lhs)+"-"+rhs;
  for( const auto &item : list ) {
    if( item.first == name1 || item.first == name2 )
      return item.second;
  };
  xthrowre("cannot convert from " << lhs << " to " << rhs);
};
bittrex::val_t bittrex::mkt_t::convert(const val_t &lhs, const sym_t &rhs, bool neutral)
{
  if(rhs==lhs.sym())
    return lhs;
  auto &mkt=get(lhs.sym(),rhs);
  return mkt.convert(lhs,neutral);
};
bittrex::val_t bittrex::mkt_t::convert(const val_t &lhs, bool neutral) const
{
  sym_t ret;
  double fact=0;
  if(lhs.sym()==cur()) {
    ret=sym();
    fact=1/(neutral?price():ask());
  } else if (lhs.sym()==sym()) {
    ret=cur();
    fact=(neutral?price():bid());
  } else {
    xthrowre("I can convert between " << cur() << " and " << sym() 
        << " but you're trying to give me " << lhs.sym());
  };
  return val_t(ret,lhs.val()*fact);
};
bittrex::mkt_m bittrex::mkt_t::dbload( const db::dbif& if_con )
{
  connection &con(if_con.con);
  mkt_m res;
  work work(con);
  auto set=work.exec("select sym,cur,bid,ask,last from mkt");
  for( auto const &row : set )
  {
    sym_t sym(row[0].c_str());
    sym_t cur(row[1].c_str());
    double bid=lexical_cast<double>(row[2].c_str());
    double ask=lexical_cast<double>(row[2].c_str());
    double last=lexical_cast<double>(row[4].c_str());
    mkt_t mkt(sym,cur,bid,ask,last);
    res[mkt.name()]=mkt;
  };
  return res;
};
bool bittrex::mkt_t::changed(const mkt_t &rhs) const
{
  xassert(name()==rhs.name());
  xassert(sym()==rhs.sym());
  xassert(cur()==rhs.cur());
  if( data.bid != rhs.data.bid )
    return true;
  if( data.ask != rhs.data.ask )
    return true;
  if( data.last != rhs.data.last )
    return true;
  return false;
};
void bittrex::mkt_t::dbstore( const db::dbif& con_if )
{
  dbstore( con_if.con, list );
};
void bittrex::mkt_t::dbstore( const db::dbif& con_if, const mkt_m& mkts )
{
  try {
    cout << db::single_row(con_if.con,
        "select utime(now())-mkt_round() as delta, mkt_round() as time, count(*) as cnt from mkt_h where stime=mkt_round();\n"
          , {20,20,20}
          ) << endl;
  } catch ( exception &ex ) {
    xexpose(ex.what());
    throw;
  };
  try {
    ostringstream sql;
    for ( auto& item : mkts )
    {
      auto& mkt = item.second;
      sql <<
        "insert into mkt_h (\n"
        "   stime, etime, sym, cur, bid, ask, last\n"
        ") values (\n"
        "   mkt_round(),\n"
        "   null,\n"
        "   " << db::dbquote( mkt.sym() ) << ",\n"
        "   " << db::dbquote( mkt.cur() ) << ",\n"
        "   " << db::dbquote( mkt.bid() ) << ",\n"
        "   " << db::dbquote( mkt.ask() ) << ",\n"
        "   " << db::dbquote( mkt.last() ) << "\n"
        ")\n"
        " on conflict do nothing;\n\n";
    };
    work work( con_if.con );
    db::exec( work, sql.str() );
    work.commit();
  } catch ( exception &ex ) {
    xexpose(ex.what());
    throw;
  };
}
ostream& bittrex::mkt_t::stream( ostream& lhs ) const
{
  ostringstream txt;
  txt
    << left << fixed << setprecision( 8 ) 
    << setw( 10 ) << "market" << "{" 
    << setw( 6 ) << sym() 
    << setw( 6 ) << cur() << right << " pri:" 
    << setw( 18 ) << price() << " bid:" 
    << setw( 18 ) << data.bid << " ask:" 
    << setw( 18 ) << data.ask << "}";
  return lhs << txt.str();
};
string bittrex::mkt_t::url()
{
  const static string url =
           "https://bittrex.com/api/v1.1/public/getmarketsummaries?";
  return url;
};
bittrex::mkt_t::mkt_t( const sym_t& cur,
              const sym_t& sym,
              double bid,
              double ask,
              double last )
{
  data_t tmp;
  tmp.cur=cur;
  tmp.sym=sym;
  tmp.bid=bid;
  tmp.ask=ask;
  tmp.last=last;
  data=tmp;
};
 
