#include <algorithm>
#include <bitcoin/explorer.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range.hpp>
#include <dbg.hh>
#include <experimental/source_location>
#include <iostream>
#include <readline/history.hh>
#include <readline/readline.hh>

using namespace std;
using namespace bc::system;
using namespace bc::explorer;
using namespace bc;
using boost::lexical_cast;

int signal_hook(void)
{
  xexpose(__PRETTY_FUNCTION__);
  return 0;
}
using std::experimental::source_location;
static vector<string> xcmds;
void collect_names(std::shared_ptr<command> cmd)
{
  xcmds.push_back(cmd->name());
}
void pexit(const char* func)
{
  perror(func);
  exit(1);
};
using bc::explorer::starts_with;
char* command_generator(const char* ctext, int state)
{
  if (!xcmds.size())
  {
    broadcast(&collect_names);
    sort(xcmds.begin(),xcmds.end());
  };
  static string text;
  static vector<string> cmds;
  static int pos;

  if(!state) {
    text=ctext;
    int i=0;
    cmds.clear();
    // first, skip everthing that is strictly lower.
    // any match will be greater to or equal to text.
    while(i<xcmds.size() && xcmds[i]<text)
      ++i;
    while(i<xcmds.size() && starts_with(xcmds[i],text))
      cmds.push_back(xcmds[i++]);
    pos=0;
  };
  if(pos>=cmds.size())
    return 0;
  return strdup(cmds[pos++].c_str());
}
void show_loc(const string&          message,
              const source_location& loc= source_location::current())
{
  std::cout << loc.file_name() << ":" << loc.line() << ":" << message
            << endl;
};
template <typename cnt_t>
string join(const cnt_t& list, const string& del)
{
  return join(vector<string>(list.begin(), list.end()), del);
};
vector<string> space_split(string str)
{
  vector<string> res;
  istringstream stream(str);
  while((stream>>str))
    res.push_back(str);
  return res;
};
int main(int argc, char** argv)
{
  rl_completion_entry_function=&command_generator;
  cout << "pid: " << getpid() << endl;
  string prompt="prompt> ";
  string line;
  string buf;
  while(readline(line,prompt)) {
    auto words=space_split(line);
    if(words.size() && words[0]=="bx")
      words=vector<string>(words.begin()+1,words.end());
    line=join(words," ");
    if(line.size())
      add_history(line.c_str());
    vector<const char *> cwords;
    for( auto &word: words )
      cwords.push_back(word.c_str());

    istringstream is(buf);
    ostringstream os;
    if(words[0]=="make-words") {
      words.clear();
      while(words.size()<23)
        words.push_back("abandon");
      words.push_back("art");
      for( auto &word : words )
        os << word << "\n";
    } else if (words[0]=="show-buf") {
      string str=is.str();
      words=space_split(str);
      
      for( auto &word : words )
      {
        cout << word << " ";
        os << word << " ";
      };
      cout << endl;
    } else {
      dispatch_command(
          cwords.size(),
          &cwords[0],
          is,
          os,
          cerr);
      if(words[0]=="help")
        cout << os.str() << endl;
    };
    buf=os.str();
  };
  cout << endl;
}

