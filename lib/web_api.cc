#include <web_api.hh>
#include <coinfwd.hh>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <bitcoin/explorer.hpp>
#include <fmt.hh>
#include <json.hh>
#include <dbg.hh>

using namespace curlpp::options;
using namespace std;

using bc::system::data_chunk;

namespace {
  bool verbose =false;
}
namespace web {
  bool get_verbose()
  {
    return verbose;
  };
  bool set_verbose(bool v)
  {
    bool res=verbose;
    verbose=v;
    return res;
  };
  template<typename str_t, typename list_t>
    const string load_page(const str_t &url, const list_t &hdr)
  {
    if(verbose)
      cerr << "load_page('" << url << "', ...)" << endl;
    // That's all that is needed to do cleanup of used resources (RAII style).
    curlpp::Cleanup myCleanup;

    // Our request to be sent.
    curlpp::Easy myRequest;

    // Set the URL.
    myRequest.setOpt(new curlpp::options::Url(url));
    //    myRequest.setOpt(new curlpp::options::Proxy("gateway:38383"));
    if(verbose)
      myRequest.setOpt(new curlpp::options::Verbose(true));
    myRequest.setOpt(new curlpp::options::HttpHeader(hdr));


    std::ostringstream os;
    curlpp::options::WriteStream ws(&os);
    myRequest.setOpt(ws);
    myRequest.setOpt(new curlpp::options::FailOnError(true));
    myRequest.perform();

    return os.str();
  }
  data_chunk str2chunk(const string &str) {
    return data_chunk(str.begin(),str.end());
  };
  using bc::system::encode_base16;
  using bc::system::hmac_sha512_hash;

  std::list<string> signurl(const string &url, const string &key, const string &sec)
  {
    list<string> res;
    data_chunk secdat=str2chunk(sec);
    if(url.back()!='&' && url.back()!='?')
      throw runtime_error("url must end with & or ?");
    res.push_back(
        url +"apikey="+key +"&nonce="+lexical_cast<string>(time(0))
        );
    data_chunk urldat=str2chunk(res.back());
    string sign=encode_base16(hmac_sha512_hash(urldat,secdat));
    res.push_back(url);
    res.push_back("apisign:"+sign);
    return res;
  };
  const string load_hmac_page( const string &url)
  {
    char *key=getenv("KEY");
    char *sec=getenv("SEC");
    if(!(key&&sec))
      xcroak("please set your credentials!");
    // Set the URL.
    string fixurl=url;
    list<string> headers=signurl(fixurl,key,sec);
    fixurl=headers.front();
    if(verbose)
      cout << "fixurl: " << fixurl << endl;
    headers.pop_front();
    return load_page(fixurl,headers);
  }
}
