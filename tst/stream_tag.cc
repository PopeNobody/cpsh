#include <iostream>
#include <boost/core/enable_if.hpp>
#include <type_traits>
#include <vector>
#include <fmt.hh>
#include <dbg.hh>
#include <db.hh>

using namespace std;

struct str_t : public fmt::streamable_tag
{
  ostream &stream(ostream &str) const
  {
    str << "It worked!";
    return str;
  };
};
int xmain(std::vector<std::string> &argv)
{
  str_t str;
  ostringstream ss;
  ss << str << endl;
  xassert(ss.str()=="It worked!\n");
  return 0;
};
