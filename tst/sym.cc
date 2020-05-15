#include <util.hh>
#include <bittrex/sym.hh>
#include <dbg.hh>


int xmain(vector<string> &args)
{
  using bittrex::sym_t;
  using util::operator==;
 
  vector<sym_t> syms;


  sym_t btc("BTC");
  sym_t bch("BCH");
  sym_t dsh("DASH");

  ostringstream str;
  str << "|" << btc << "|";
  string res=str.str();
  xassert(bch<btc);
  xassert(btc<dsh);
  xassert(bch==bch);
  xassert(bch!=btc);
  xassert(bch!=dsh);

  return 0;
};
