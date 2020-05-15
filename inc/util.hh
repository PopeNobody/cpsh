
#ifndef util_hh
#define util_hh util_hh

#include <boost/core/demangle.hpp>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/signal.h>
#include <sys/types.h>
#include <typeinfo>
#include <unistd.h>
#include <vector>
#include <string.h>



using std::string;
extern string progname;
int xmain(std::vector<std::string> &argv);
namespace util {
  using boost::core::demangle;
  using namespace std;

  typedef int fd_t;
  bool exists(const string &fn);
  class fd_streambuf : public streambuf
  {
    int fd1;
    int fd2;
    public:
    fd_streambuf(int fd1, int fd2)
      : fd1(fd1)
        , fd2(fd2)
    {
    };
    int overflow(int c = EOF )
    {
      char tmp[1];
      tmp[0]=(c&0xff);
      write(fd1,tmp,1);
      write(fd2,tmp,1);
      return traits_type::to_int_type( c );
    };
    virtual ~fd_streambuf();
  };
  void split_stream(const string &logfile);

  template<typename itr_t>
    inline string join(const string &sep, itr_t b, itr_t e)
    {
      string res;
      if(b==e)
        return res;
      res=*b++;
      if(b==e)
        return res;
      while(b!=e) {
        res+=sep;
        res+=*b++;
      };
      return res;
    };
  template<typename cont_t>
    inline string join(const string &sep, cont_t cont)
    {
      return join(sep,cont.begin(),cont.end());
    };
  inline vector<string> split( char sep, const string &str )
  {
    vector<string> res;
    auto s=str.begin();
    for( auto b(s), e(str.end()); b!=e; b++ ) {
      if( *b == sep )
      {
        res.push_back(string(s,b));
        s=b+1;
      };
    };
    res.push_back(string(s,str.end()));

    return res;
  }
  template<typename cont_t>
    inline void sort(cont_t &cont)
    {
      sort(begin(cont),end(cont));
    };
  //   	template<typename cont_t, typename itr_t>
  //   	cont_t split(char sep, itr_t b, itr_t e)
  //   	{
  //   		cont_t res;
  //   		if(b!=e) {
  //   			do {
  //   				auto s=find(b,e,sep);
  //   				res.push_back(string(b,s));
  //   				b=s;
  //   				if(b!=e)
  //   					++b;
  //   			} while(b!=e);
  //   		};
  //   		return res;
  //   	};
  //   	template<typename cont_t>
  //   	cont_t split(char sep, c_str str)
  //   	{
  //   		return split<cont_t>(sep,str.begin(),str.end());
  //   	};
  template<typename itr_t, typename val_t>
    bool contains(itr_t b, itr_t e, const val_t &val){
      while(b!=e) {
        if( *b++ == val )
          return true;
      }
      return false;
    };
  template<typename con_t, typename val_t>
    bool contains(const con_t &con, const val_t &val) {
      return contains(begin(con),end(con),val);
    };
  string strip(const string &str);
  bool exists(const string &fn);
  bool exists(const char *path);
  string read_file(const char *path);
  string read_file(const string &path);
  ssize_t write_file(const string &name, const string &text);
  string quote(const string &str);
  int open_log(const string &in_fn);
  int xrename(const char *ofn, const char *nfn);
  int xunlink(const char *fn, bool missing_fail=false);
  void xdup2(int fd, int ofd);
  void xdup(int fd);
  void xclose(fd_t fd);

  fd_t xcreat(const char *pathname, mode_t mode);

  fd_t xopenat(int dirfd, const char *pathname, int flags);
  fd_t xopenat(int dirfd, const char *pathname, int flags, mode_t mode);
  void *xmmap(void *addr, size_t length, int prot, int flags, fd_t fd, off_t offset);
  int xmunmap(void *addr, size_t length);
  off_t xlseek(fd_t fd, off_t offset, int whence);
  pid_t xfork();
  size_t xread( fd_t fd, char *buf, size_t len );
  size_t xwrite( fd_t fd, const char *buf, size_t len );
  void xpipe( int ( &fds )[ 2 ] );
  sighandler_t xsignal( int sig, sighandler_t hand );
  inline double abs(double d)
  {
    return (d<0?-d:d);
  };
  template<typename obj_t>
    inline int cmp(const obj_t &lhs, const obj_t &rhs)
    {
      return obj_t::cmp(lhs,rhs);
    };
  template<>
    inline int cmp<long>(const long &lhs, const long &rhs)
    {
      return lhs-rhs;
    };
  template<>
    inline int cmp<double>(const double &lhs, const double &rhs)
    {
      if(lhs<rhs)
        return -1;
      else if (rhs<lhs)
        return 1;
      else
        return 0;
    };
  template<>
    inline int cmp<char>( const char &lhs, const char &rhs )
    {
      return lhs-rhs;
    };
  template<>
    inline int cmp<int>( const int &lhs, const int &rhs )
    {
      return lhs-rhs;
    };
  inline void perror(const char *msg1, const char *msg2="")
  {
    size_t len1=strlen(msg1);
    size_t len2=strlen(msg2);
    char msg[len1+len2+2];
    char *dst=msg;
    while(char ch=*msg1++)
      *dst++=ch;
    *dst++=':';
    while(char ch=*msg2++)
      *dst++=ch;
    *dst=0;
    perror(msg);
  };
  inline void pexit(const char *msg1, const char *msg2)
  {
    perror(msg1,msg2);
    exit(1);
  };
  inline void pexit(const char *msg)
  {
    perror(msg);
    exit(1);
  };

  typedef int fd_t;
  fd_t xopen(const char *pathname, int flags, mode_t mode);
  fd_t xopen(const char *fn, int flags, mode_t mode);

  fd_t xcreat(const char *pathname, mode_t mode);

  fd_t xopenat(int dirfd, const char *pathname, int flags);
  fd_t xopenat(int dirfd, const char *pathname, int flags, mode_t mode);
  void xclose( fd_t fd );
  void xdup2( int ofd, int nfd );
  void xexeclp( const char* file, const char* arg, ... );
  int xmunmap(void *addr, size_t length);
  off_t xlseek(fd_t fd, off_t offset, int whence);
  pid_t xfork();
  sighandler_t xsignal( int sig, sighandler_t hand );
  size_t xread( fd_t fd, char *buf, size_t len );
  size_t xwrite( fd_t fd, const char *buf, size_t len );
  void *xmmap(void *addr, size_t length, int prot, int flags, fd_t fd, off_t offset);

#define comp_op(x) \
  template<typename val_t> \
  bool operator x (const val_t &lhs, const val_t &rhs) { return util::cmp(lhs,rhs) x 0; };

  comp_op(==);
  comp_op(!=);
  comp_op(<=);
  comp_op(>=);
  comp_op(< );
  comp_op(> );
#undef comp_op


};

inline std::ostream &operator<< (std::ostream &lhs, const std::exception &rhs)
{
  return lhs<<rhs.what();
};
std::ostream &operator<< (std::ostream &lhs, const std::type_info &rhs);

#endif
