#include "framework/application.h"
#include "framework/routine.h"
#include <sstream>

namespace ffnet{
  application::application(const std::string & app_name)
    : m_app_name(app_name)
      , m_desc("Allowed options"){

        using namespace boost::program_options;
        m_desc.add_options()
          ("help", "produce help message")
          ("list", "list all routines")
          ("net-mode", value<int>(), "1: real-net, 2: simu-net, 3: single_net")
          ("run-routine", value<std::vector<std::string> >(), "routine's name to run")
          ("routine-arg", value<std::vector<std::string> >(), "routine's args");

      }

  void application::initialize(int argc, char *argv[]){

    using namespace boost::program_options;
    variables_map vm;
    store(parse_command_line(argc, argv, m_desc), vm);
    notify(vm);

    if(vm.count("help")){
      m_to_run_func = boost::bind(&application::print_help, this);
      return ;
    }
    if(vm.count("list")){
      m_to_run_func = boost::bind(&application::list_routines, this);
      return ;
    }
    int arg_num = 0;
    if(vm.count("net-mode")){
      arg_num ++;
      m_nm = static_cast<net_mode>(vm["net-mode"].as<int>());
    }
    if(vm.count("routine-args")){
      arg_num ++;
      m_routine_args = vm["routine-args"].as<std::vector<std::string> >();
    }
    if(vm.count("run-routine")){
      m_routine_name = vm["run-routine"].as<std::vector<std::string> >();
      if(m_nm == real_net  && m_routine_name.size() != 1){
        std::cout<<"Can only specify one routines in real_net mode! "<<std::endl;
        m_to_run_func = boost::bind(&application::list_routines, this);
        return ;
      }else{
        m_to_run_func = boost::bind(&application::run_rountine, this);
      }
    }
    else{
      if(arg_num == 0){
        m_to_run_func = boost::bind(&application::print_help, this);
        return ;
      }else{
        std::cout<<"Have to set run-routine to an available rountine"<<std::endl;
        m_to_run_func = boost::bind(&application::list_routines, this);
        return ;
      }
    }
  }

  void application::register_routine(routine * rp){
    assert(rp != NULL);
    m_routines.push_back(rp);
  }

  void application::run(){
    m_to_run_func();
  }

  void application::print_help(){
    std::cout<<m_desc<<std::endl;
  }

  void application::list_routines(){
    std::cout<<"\tall:  represent all routines"<<std::endl;
    for(size_t i = 0; i < m_routines.size(); ++i){
      std::cout<<"\t"<<m_routines[i]->get_name()<<std::endl;
    }
  }

  void application::run_rountine(){
    std::map<std::string, routine *> rnames;
    for(size_t i = 0; i < m_routines.size(); ++i){
      std::string n = m_routines[i]->get_name();
      if(rnames.find(n) != rnames.end()){
        std::cout<<"More than one routine with same name, "<<n<<std::endl;
        return ;
      }
      rnames.insert(std::make_pair(m_routines[i]->get_name(), m_routines[i]));
    }

    for(size_t i = 0; i < m_routine_name.size(); ++i){
      std::string s = m_routine_name[i];
      if(rnames.find(s) == rnames.end()){
        std::cout<<"Cannot find routine "<<s<<std::endl;
        return ;
      }
    }

    for(size_t i = 0; i < m_routine_name.size(); ++i){
      std::string s = m_routine_name[i];
      routine * pr = rnames[s];

      start_routine(pr);
    }
  }

  void application::start_routine(routine * r){
    if(m_nm == real_net){
      r->initialize(m_nm, m_routine_args);
      r->run();
    }else if(m_nm == simu_net){

      std::cout<<"TODO start_routine:do something here!"<<std::endl;

    }
  }

}

