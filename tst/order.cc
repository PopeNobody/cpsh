#include <bittrex.hh>
#include <json.hh>

int xmain(std::vector<std::string> &argv)
{
  using namespace bittrex;
  json copy;
  {
    order_t order;
    order.data.uuid="asdfj";
    order.data.closed=date_t("09/11/2001");
    order.data.commission=0;
    order.data.immediate_or_cancel=true;
    order.data.is_conditional=true;
    order.data.xlimit=0.15;
    order.data.order_type="LIMIT_BUY";
    order.data.price=0.14;
    order.data.price_per_unit=0.11;
    order.data.quantity=0.11;
    order.data.quantity_remaining=0.11;
    order.data.stamptime=date_t("09/12/2001");
    order.data.condition_target=3.14159;
    order.data.condition="fair";
    order.data.exchange="Chicago";
    order.data.commission=43;
    copy=order;
  };
  {
    order_t order=copy;
    cout << order << endl;
  };
  return 0;
};
