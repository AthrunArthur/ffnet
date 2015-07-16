#include "framework/application.h"
#include "framework/routine.h"
#include <sstream>

namespace ffnet{
  application::application(const std::string & app_name)
    : m_app_name(app_name)
    , m_desc("Allowed options"){

    m_desc.add_options()
      ("help", "produce help message");
      ("list", "list all routines")
      ("net-mode", value<std::string>(), "real-net, sim-net")
      ("run-routine", value<std::string>(), "routine's name to run")
      ("routine-args", value<std::vector<std::string> >(), "routine's args");

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
        std::string s = vm["net-mode"].as<std::string>();
        std::cout<<s<<std::endl;
      }
      if(vm.count("routine-args")){
        arg_num ++;
        m_routine_args = vm["routine-args"].as<std::vector<std::string> >();
      }
    if(vm.count("run-routine")){
      m_routine_name = vm["run-routine"].as<std::string>();
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
    std::cout<<"routine name: "<<m_routine_name<<std::endl;
    std::cout<<"net mode: "<<m_nm<<std::endl;
    for(size_t i = 0; i < m_routine_args.size(); ++i){
      std::cout<<"\t args "<<i<<": "<<m_routine_args[i]<<std::endl;
    }
  }
}

