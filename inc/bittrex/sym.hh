#ifndef sym_hh
#define sym_hh sym_hh

#include <string>
#include <fmt.hh>
#include <array>
#include <bittrex/ops.hh>
#include <util.hh>
namespace bittrex
{
  using std::string;
  using std::ostream;
  using std::array;
  array<char,8> mk_array(const string &);

  class sym_t : public fmt::streamable_tag
  {
    struct data_t {
      array<char,8> txt;
      data_t(const array<char,8> &rhs);
    } data;
    public:
    sym_t();
    explicit sym_t(const string& str);
    explicit sym_t(const char *);
    explicit sym_t(const array<char,8> &rhs);
    explicit operator string() const {
      return string( begin(data.txt) );
    };
    size_t length() const;
    size_t size() const;
    ostream &stream(ostream &lhs) const;
    static int cmp( const sym_t& lhs, const sym_t &rhs );
  };
  inline string operator+(const string &lhs, const sym_t &rhs)
  {
    return lhs+string(rhs);
  };
  inline bool operator>( const sym_t& lhs, const sym_t& rhs )
  {
    return bittrex::sym_t::cmp( lhs, rhs ) > 0;
  };
  inline bool operator<( const sym_t& lhs, const sym_t& rhs )
  {
    return bittrex::sym_t::cmp( lhs, rhs ) < 0;
  };
  inline bool operator>=( const sym_t& lhs, const sym_t& rhs )
  {
    return bittrex::sym_t::cmp( lhs, rhs ) >= 0;
  };
  inline bool operator<=( const sym_t& lhs, const sym_t& rhs )
  {
    return bittrex::sym_t::cmp( lhs, rhs ) <= 0;
  };
  inline bool operator!=( const sym_t& lhs, const sym_t& rhs )
  {
    return bittrex::sym_t::cmp( lhs, rhs ) != 0;
  };
  inline bool operator==( const sym_t& lhs, const sym_t& rhs )
  {
    return bittrex::sym_t::cmp( lhs, rhs ) == 0;
  };


}; // namespace bittrex

#endif
