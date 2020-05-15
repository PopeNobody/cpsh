#include "pqxx/except.hxx"
#include <coinfwd.hh>
#include <dbg.hh>

inline ostream &operator<<(ostream &lhs, const exception &ex);
using namespace std;
string progname;
int main( int argc, char** argv )
{
  try {
    if(!argc) {
      cerr << "no args?" << endl;
      return 1;
    };
    progname=*argv;
    vector< string > args( argv+1, argv + argc ) ;
    return xmain(args );
  } catch ( const exception& e ) {
    xcarp( "\n" << e );
  } catch ( ... ) {
    xcarp( "unexpected exception!" );
  };
  return -1;
}
