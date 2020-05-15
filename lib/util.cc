#include <util.hh>
#include <fcntl.h>
#include <sys/stat.h>
#include <dbg.hh>
#include <fstream>
#include <sstream>

using util::fd_t;
using std::string;

bool util::exists(const string &fn)
{
  struct stat stat_buf;
  if(!stat(fn.c_str(),&stat_buf))
    return true;
  if(errno != ENOENT)
    return true;
  return false;
};
util::fd_streambuf::~fd_streambuf()
{
  if(cout.rdbuf()==this)
    cout.rdbuf(0);
  if(cerr.rdbuf()==this)
    cerr.rdbuf(0);
};
void util::split_stream(const string &logname) {
  int fd=open_log(logname);
  static util::fd_streambuf obuf(1,fd);
  static util::fd_streambuf ibuf(2,fd);
  cout.rdbuf(&obuf);
  cerr.rdbuf(&ibuf);
};

std::string util::strip(const string &str)
{
	auto b(str.begin());
	auto e(str.end());
	if(b==e)
		return str;
	while(b!=e && isspace(*--e))
		;
	while(b!=e && isspace(*b))
		++b;
	return string(b,++e);
};
ssize_t util::write_file(const string &name, const string &text)
{
	ofstream file(name);
  file.write(text.c_str(),text.length());
	return text.length();
};
string util::read_file(const char *path)
{
  return read_file(string(path));
};
string util::read_file(const string &path)
{
	ifstream file(path);
	std::stringstream buf;
	buf << file.rdbuf();
	string res=buf.str();
	return res;
};
bool util::exists(const char *name)
{
	struct stat buf;
	if(stat(name,&buf)<0){
		return false;
	};
	return true;
};
string util::quote(const string &str)
{
  char buf[2*str.size()];
  for(int i=0, j=0;i<str.size();i++, j++)
  {
    cout << i << j << i-j << endl;
  };
  return str;
};
//   string mk_backup_name(string in_fn, int num)
//   {
//     auto ps = split(in_fn+".log",".");
//     ps[ps.size()-2]=lexical_cast<string>(i);
//     return join(".",ps);
//   };
int util::open_log(const string &in_fn)
{
  return xopen(in_fn.c_str(),O_WRONLY|O_CREAT|O_APPEND,0644);
//     struct stat stat_buf;
//     string fn(in_fn);
//     for(int i=100;i<1000;i++)
//     {
//       int res=stat(fn.c_str(),&stat_buf);
//       if( res && errno==ENOENT ) {
//         if(in_fn != fn )
//           xrename(in_fn.c_str(),fn.c_str());
//         return xopen(in_fn.c_str(),O_WRONLY|O_CREAT|O_APPEND,0644);
//       };
//       fn=mk_backup_name(in_fn,i);
//     };
//     xthrowre("clean your log dir, you have 1000 of them!");
};
int util::xrename(const char *ofn, const char *nfn)
{
  int res=rename(ofn,nfn);
  if(res<0)
    xcroak("rename("<<ofn<<","<<nfn<<"):" << strerror(errno));
  return res;
};
int util::xunlink(const char *fn, bool missing_fail)
{
  int res=unlink(fn);
  if(!res)
    return 0;
  if(missing_fail)
    return -1;
  if(errno==ENOENT)
    return 0;
  return -1;
};
void util::xdup2(int fd, int ofd)
{
  int res=dup2(fd,ofd);
  if(res<0)
    assert(!"dup2 failed");
};
void util::xdup(int fd)
{
  int res=dup(fd);
  if(res<0)
    assert(!"dup failed");
};
fd_t util::xopen(const char *fn, int flags, mode_t mode)
{
  int res=open(fn,flags,mode);
  if(res<0)
    assert(!"open failed");
  return res;
};
void util::xclose(int fd)
{
  auto res=close(fd);
  if(res<0)
    assert(!"close failed");
};
std::ostream &operator<<(std::ostream &lhs, const std::type_info &rhs){
  return lhs << util::demangle(rhs.name());
};
