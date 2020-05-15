#ifndef fmt_hh
#define fmt_hh fmt_hh

#include <coinfwd.hh>
#include <iostream>

namespace fmt {
  using std::ostream;
  using std::string;
  struct streamable_tag
  {
  };
  template<typename rhs_t>
  inline typename std::enable_if<std::is_base_of<fmt::streamable_tag,rhs_t>::value,ostream &>::type &operator<<( ostream &lhs, const rhs_t &rhs )
  {
    return rhs.stream(lhs);
  };
}
#endif
