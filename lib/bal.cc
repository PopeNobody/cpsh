#include <bittrex/bal.hh>
#include <bittrex.hh>
#include <bittrex/mkt.hh>

string bittrex::bal_t::url(){
  static const string url =
    "https://bittrex.com/api/v1.1/account/getbalances?";
  return url;
};

bittrex::bal_t::bal_t(
    const char* sym,
    double val,
    double pend
    )
  : data(sym_t(sym),val,pend)
{
};

bittrex::bal_t::bal_t(
    const val_t& bal,
    const val_t& pend
    )
  : data(bal,pend)
{
};
bittrex::bal_t::bal_t(
    const sym_t& sym,
    double val,
    double pend
    )
  : data(val_t(sym,val),val_t(sym,pend))
{
};
bittrex::bal_m bittrex::bal_t::list;
bittrex::bal_m& bittrex::bal_t::load( bool force )
{
  if ( force || list.size() == 0 )
  {
    bal_m res;
    json data;
    data=load_json_url(bittrex::bal_t::url(),"info/bal.json");
    for ( auto obj : data )
    {
      bittrex::bal_t bal;
      from_json( obj, bal );
      if ( bal.sym() == sym_t("BTXCRD") )
        continue;
      if ( !bal.bal() )
        continue;
      res[ bal.sym() ] = bal;
    };
    list = res;
  };
  return list;
};
bittrex::bal_m bittrex::bal_t::dbload( connection& con )
{
  bal_m res;
  work work( con );
  auto set = work.exec( "select sym,bal,pend,avail from bal" );
  for ( auto const& row : set )
  {
    sym_t sym(row[ 0 ].c_str());
    double bal = lexical_cast< double >( row[ 1 ].c_str() );
    double pend =  lexical_cast< double >( row[ 2 ].c_str() );
    double ava =  lexical_cast< double >( row[ 3 ].c_str() );
    xassert( bal - pend == ava ); 
    res[ sym ] = bal_t( val_t(sym,bal), val_t(sym,pend) );
  };
  return res;
};
ostream& bittrex::bal_t::stream( ostream& lhs ) const
{
  ostringstream txt;
  txt << fixed << setprecision( 8 ) << left << setw( 10 ) << "balance"
      << "{" << setw( 10 ) << sym() << right << " bal:" << setw( 18 )
      << data.bal << " pen:" << setw( 18 ) << data.pend << "}";
  return lhs << txt.str();
};
