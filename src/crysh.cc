#include <bitcoin/explorer.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <readline/history.hh>
#include <readline/readline.hh>
#include <experimental/source_location>

using namespace std;
using namespace bc::system;
using namespace bc::explorer;
using namespace bc;
int signal_hook( void )
{
  cerr << "signaled" << endl;
  return 0;
}
using std::experimental::source_location;
static std::vector< string > cmds;
void collect_names( std::shared_ptr< command > cmd )
{
  cmds.push_back( cmd->name() );
}
char* command_generator( const char* text, int state )
{
  static unsigned index = 0;
  static size_t len = 0;
  if ( !cmds.size() )
  {
    broadcast( &collect_names );
  };
  if ( !state )
  {
    index = 0;
    len = strlen( text );
  };
  while ( index < cmds.size() )
  {
    const string& cmd = cmds[ index ];
    ++index;
    if ( !strncmp( text, cmd.c_str(), len ) )
      return strdup( cmd.c_str() );
  };
  return 0;
}
void show_loc( const string& message,
               const source_location& loc = source_location::current() )
{
  std::cout << loc.file_name() << ":" << loc.line() << ":" << message
            << endl;
};
int main( int argc, char** argv )
{
  show_loc("here we are!");
  // rl_bind_key ('\t', rl_insert);
  string last_out;
  ostringstream os;
  istringstream is( last_out );
  
  rl_signal_event_hook = &signal_hook;
  rl_completion_entry_function = &command_generator;
  const char* cline;
  while ( ( cline = readline( "prompt> " ) ) )
  {
    vector< string > words = bc::system::split( cline, " " );
    if ( !words.size() )
      continue;
    if ( words[0]=="bx" ) {
      if(words.size()==1)
        words[0]="help";
      else
        words.erase(words.begin());
      cout << join(words,"-") << endl;;
    };
    string line=join(words," ");
    rl_free( (void*)cline );
    cline=line.c_str();

    if(cline[0])
      add_history(cline);
    
    if ( words[ 0 ] == "mnemonic-to-seed" )
    {
      if ( last_out.size() )
        cout << "discarding buffer: " << last_out << endl;
      for ( int i = 0; i < 23; i++ )
        os << "abandon ";
      os << "art";
      last_out = os.str();
      os.str( "" );
      cout << last_out << endl;
    };
    is.str( last_out );
    vector< const char* > cwords;
    for ( auto& word : words )
      cwords.push_back( word.c_str() );
    dispatch_command( cwords.size() - 1, &cwords[ 0 ], is, os, cerr );
    auto temp = os.str();
    if ( temp.size() )
    {
      last_out = temp;
      cout << last_out << endl;
    };
    os.str( "" );
  };
  cout << endl;
  return 0;
}
