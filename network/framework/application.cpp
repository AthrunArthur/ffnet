#include "framework/application.h"
#include "framework/routine.h"
#include "simu_net/simu_server.h"
#include <common/defines.h>
#include "simu_net/net_simu_base.h"


namespace ffnet{
  application::application(const std::string & app_name)
    : m_app_name(app_name){}

  void application::initialize(int argc, char *argv[]){
    ::google::InitGoogleLogging(argv[0]);
    ::google::ParseCommandLineFlags(&argc, &argv, true);
  }

  void application::register_routine(routine * rp){
    assert(rp != NULL);
    m_routines.push_back(rp);
  }

  void application::run(){
    if(FLAGS_list_routines){
      list_routines();
      return ;
    }
    if(FLAGS_simu_server){
      start_simu_server();
      return ;
    }
    if(FLAGS_run_routine != std::string("")){
      m_routine_name.push_back(FLAGS_run_routine);
    }
    m_nm = static_cast<net_mode>(FLAGS_net_mode);
    run_routine();
  }

  void application::list_routines(){
    std::cout<<"\tall:  represent all routines"<<std::endl;
    for(size_t i = 0; i < m_routines.size(); ++i){
      std::cout<<"\t"<<m_routines[i]->get_name()<<std::endl;
    }
  }

  void application::run_routine(){
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

    if(m_routine_name.size() == 0){
      std::cout<<"No available routines to run!"<<std::endl;
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

  void application::start_simu_server(){
    simu_server ss;
    simu_net_default snd(&ss);
    ss.init(&snd);
    ss.run();
  }
}

