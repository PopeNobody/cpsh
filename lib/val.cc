#include <bittrex/val.hh>
#include <bittrex/mkt.hh>

bittrex::val_t::val_t()
{
};
bittrex::val_t::val_t( const char* sym, double val )
  : data( sym_t(sym), val )
{
};
bittrex::val_t::val_t( const sym_t& sym, double val )
  : data( sym, val )
{
};
bittrex::val_t::val_t( const val_t& rhs )
  : data( rhs.data )
{
};
ostream& bittrex::val_t::stream( ostream& lhs ) const
{
  lhs << fixed << setprecision( 8 ) << val() << sym();
  return lhs;
};
int bittrex::val_t::cmp( const val_t& lhs, const val_t& rhs )
{
  int res = util::cmp( lhs.sym(), rhs.sym() );
  if ( !res )
    res = util::cmp( lhs.val(), rhs.val() );
  return res;
};
bittrex::val_t bittrex::val_t::convert(const sym_t &sym, bool neutral)
{
  return mkt_t::convert(*this,sym, neutral);
}
