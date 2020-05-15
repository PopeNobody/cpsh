#include <bittrex/goal.hh>
#include <bittrex.hh>

using bittrex::goal_t;
using bittrex::goal_m;
int xmain(std::vector<std::string> &argv)
{
  json js;
  goal_t goal0("XXX",12);
  js["goal0"]=goal0;
  js["goal1"]=goal_t("BTC",100);
  goal_t goal1;


  goal_m goals;
  from_json(js,goals);
  for( auto pair : goals )
  {
    auto &key=pair.first;
    auto &val=pair.second;
  };

  json js2;
  js2=goals;

  return 0;
};
