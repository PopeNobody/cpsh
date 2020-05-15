#include <dbg.hh>
namespace dbg 
{
  using namespace std;
  ostream &show_dbg_msg(
      ostream &str, const char *file, unsigned line, const char *msg
      )
  {
    str << file << ":" << line << ":";
    if(msg)
      str << msg;
    return str;
  };
}
