#include <bittrex/val.hh>


int xmain(vector<string> &args)
{
  using bittrex::sym_t;
  using bittrex::val_t;


  val_t btc("BTC",1);
  val_t bch("BCH",2);
//  xexpose(btc+1);  // Should not and does not compile.
  bool got_except=false;
  try {
    auto x=btc+bch;
  } catch ( exception &ex ) {
    got_except=true;
  };
  if( !got_except )
    xthrowre("expected an exception there");

  xassert(btc+btc==val_t("BTC",2));
  //cout << btc.convert("BCH") << endl;

  return 0;
};
