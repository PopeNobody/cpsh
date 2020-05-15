#include <bittrex/bal.hh>
#include <bittrex.hh>

using namespace bittrex;

int xmain(std::vector<std::string> &argv)
{

  bal_t bal1("BTC",1.0);
  bal_t bal2("BCH",8.0);
  bal_t bal3("BSV",1.0);

  json js;
  js["bal1"]=bal1;
  js["bal2"]=bal2;
  js["bal3"]=bal3;

  bal_m bals=js;
  for( auto &item : bals )
  {
    auto &key=item.first;
    auto &val=item.second;
  };
  json js2;
  js2=bals;


  return 0;
};
