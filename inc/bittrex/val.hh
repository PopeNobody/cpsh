#ifndef val_hh
#define val_hh val_hh

#include <bittrex/sym.hh>
#include <dbg.hh>
#include <fmt.hh>
#include <util.hh>

namespace bittrex
{
  class val_t : public fmt::streamable_tag
  {
  public:
    struct data_t
    {
      sym_t sym;
      double val;
      data_t( const sym_t& sym = sym_t(), double val = 0 )
          : sym( sym )
          , val( val ){};
      ~data_t(){};
    };
    data_t data;
    explicit val_t( const sym_t& sym, double val = 0 );
    explicit val_t( const char* sym, double val = 0 );
    val_t( const val_t& rhs );
    val_t();
    const sym_t& sym() const { return data.sym; };
    const double& val() const { return data.val; };
    val_t convert( const sym_t& cur, bool neutral = true );
    ostream& stream( ostream& lhs ) const;
    static int cmp( const val_t& lhs, const val_t& rhs );
    val_t operator-() const { return val_t( sym(), -val() ); };
    bool operator!() const { return !val(); };
    val_t& operator+=( const val_t& rhs )
    {
      xassert( rhs.sym() == sym() );
      data.val += rhs.data.val;
      return *this;
    };
    val_t& operator-=(const val_t& rhs )
    {
      return operator+=(-rhs);
    };
  };

  inline val_t operator*( double lhs, val_t rhs )
  {
    return val_t( rhs.sym(), lhs * rhs.val() );
  };
  inline val_t operator*( const val_t& lhs, double rhs )
  {
    return val_t( lhs.sym(), lhs.val() * rhs );
  };
  inline val_t operator/( const val_t& lhs, double rhs )
  {
    return val_t( lhs.sym(), lhs.val() / rhs );
  };

  inline val_t operator+( val_t lhs, const val_t& rhs )
  {
    lhs += rhs;
    return lhs;
  };
  inline val_t operator-( val_t lhs, const val_t& rhs )
  {
    lhs -= rhs;
    return lhs;
  };

  inline bool operator==( const val_t& lhs, const val_t& rhs )
  {
    return val_t::cmp( lhs, rhs ) == 0;
  };
  inline bool operator!=( const val_t& lhs, const val_t& rhs )
  {
    return val_t::cmp( lhs, rhs ) != 0;
  };
  inline bool operator<=( const val_t& lhs, const val_t& rhs )
  {
    return val_t::cmp( lhs, rhs ) <= 0;
  };
  inline bool operator>=( const val_t& lhs, const val_t& rhs )
  {
    return val_t::cmp( lhs, rhs ) >= 0;
  };
  inline bool operator<( const val_t& lhs, const val_t& rhs )
  {
    return val_t::cmp( lhs, rhs ) < 0;
  };
  inline bool operator>( const val_t& lhs, const val_t& rhs )
  {
    return val_t::cmp( lhs, rhs ) > 0;
  };
} // namespace bittrex
#endif
