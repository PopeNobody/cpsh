#include <bittrex/goal.hh>
#include <bittrex.hh>
#include <dbg.hh>
#include <db.hh>

static const string goal_query="select * from goal order by sym\n\n";
using db::exec;
namespace bittrex
{
  goal_m goal_t::list;
  goal_m &goal_t::load( connection& con )
  {
    using namespace pqxx;
    goal_m res;
//       {
//         work work(con);
//         work.exec("delete from goal");
//         work.commit();
//       };
    auto rows=db::json_query(con,goal_query);
    if(rows.size()){
      for( auto row : rows ) {
        cout << row << endl;
        json j=json::parse(row);
        goal_t goal;
        from_json(j,goal);
        res[goal.sym()]=goal;
      };
    } else {
      ifstream file("etc/goals.json");
      if(file) {
        string line;
        string text;
        while(getline(file,line))
          text+=line;
        json jlist=json::parse(text);
        jlist=jlist.at("goals");
        double total;
        for( auto b(jlist.begin()), e(jlist.end()); b!=e; b++ )
        {
          double val=b.value();
          total+=val;
        };



        for( auto b(jlist.begin()), e(jlist.end()); b!=e; b++ )
        {
          sym_t key(b.key());
          res[key]=goal_t(key,b.value(),(double)b.value()/total);
        };
      };
    };
    list=res;
    return list;
  };
  void goal_t::dbstore(connection &con)
  {
    dbstore(con,list);
  }
  void goal_t::dbstore(connection &con, const goal_m &goals )
  {
    work work(con);
    ostringstream sql;
    sql << " delete from goal_n;\n\n";
    for( auto &item : goals )
    {
      auto &goal=item.second;
      sql
        << " insert into goal_n values (\n"
        << "   " << db::dbquote(goal.sym())  <<  ",\n"
        << "   " << db::dbquote(goal.weight())  <<  "\n"
        << ");\n\n";
    };
    sql <<
      " delete from goal where (sym) not in (select sym from goal_n);\n\n"
      " delete from goal_n where (sym,weight) in (select * from goal);\n\n"
      " insert into goal\n"
      "  (select * from goal_n)\n"
      "   on conflict(sym)\n"
      "  do\n"
      "   update set weight = excluded.weight;\n\n"
      " delete from goal_n where (sym,weight) in (select * from goal);\n\n"
      "\n";
    exec(work,sql.str());
    work.commit();
  };
  ostream& goal_t::stream( ostream& lhs ) const {
    ostringstream txt;
    txt 
      << fixed << setprecision(8) << left
      << setw(10) << "goal_t" << "{"
      << setw(10) << data.sym
      << right
      << " weight:" << setw(18) << data.weight
      << " pct:" << setw(18) << data.pct
      << "}";
    return lhs << txt.str();
  };
  goal_t::~goal_t()
  {
  };
  goal_t::goal_t(
      const char* sym,
      double weight,
      double pct
      )
  {
    data.sym=sym_t(sym);
    data.weight=weight;
    data.pct=pct;
  };

  goal_t::goal_t(const goal_t &goal)
    : data(goal.data)
  {
  };
  goal_t::goal_t()
  {
  };
};
