#ifndef web_api_hh
#define web_api_hh web_api_hh


#include <coinfwd.hh>
#include <string>
namespace web {
  using std::string;
  template<typename str_t, typename list_t>
    const string load_page(const str_t &url, const list_t &hdr=list_t());

  bool set_verbose(bool verbose);
  bool get_verbose();
  const string load_hmac_page( const string &url);
};

#endif

