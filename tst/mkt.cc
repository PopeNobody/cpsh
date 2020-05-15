#include <bittrex/mkt.hh>
#include <json.hh>
#include <bittrex.hh>

using bittrex::mkt_t;
using bittrex::val_t;
using bittrex::mkt_m;
using bittrex::sym_t;
using bittrex::to_string;

bool do_expect(unsigned line, const char *expr, const val_t &res, const val_t &exp)
{
  xexpose(line);
  xexpose(expr);
  xexpose(res);
  xexpose(exp);
  xexpose(res-exp);
  xexpose(abs(res.val()-exp.val()));
  return abs(res.val()-exp.val()) < 0.00000001;
};
#define xexpect(x,y) if(!do_expect(__LINE__,#x,x,y)) { failures++; cout << "failed" << endl;  };

int xmain(std::vector<std::string> &argv)
{
  mkt_t mkt(sym_t("USD"),sym_t("BTC"),90,110,100);
  xexpose(mkt);
  int failures=0;
  try {
    {
      val_t val("USD",100);
      mkt.convert(val,false);
      xexpect(mkt.convert(val,true),val_t("BTC",1));
      xexpect(mkt.convert(val,false),val_t("BTC",100.0/110));
    }
    {
      val_t val("BTC",1);
      xexpect(mkt.convert(val,true),val_t("USD",100));
      xexpect(mkt.convert(val,false),val_t("USD",90.0));
    };
    {
      val_t val("BTC",0.5);
      xexpect(mkt.convert(val,true),val_t("USD",50));
      xexpect(mkt.convert(val,false),val_t("USD",45));
    };
    {
      val_t val("USD",200);
      xexpect(mkt.convert(val,true),val_t("BTC",2));
      xexpect(mkt.convert(val,false),val_t("BTC",2.0/1.1));
    };
    if( failures ) {
      cout << failures << " tests failed" << endl;
    };
  } catch ( const exception &ex ) {
    cout << ex << endl;
    return 1;
  };
  return 0;
};
