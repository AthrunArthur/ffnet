#include "framework/flags.h"

DEFINE_string(simu_server_addr, "127.0.0.1",
    "the simulation server's ip address");


DEFINE_int32(simu_server_port, 8150,
    "the simulation server's port");

static bool ValidatePort(const char* flagname, int32_t value) {
  if (value > 0 && value < 32768)   // value is ok
         return true;
     printf("Invalid value for --%s: %d\n", flagname, (int)value);
        return false;
}
static const bool port_dummy = google::RegisterFlagValidator(&FLAGS_simu_server_port, &ValidatePort);

DEFINE_int32(net_mode, 1,
    "the mode to run routines, 1 - real net, 2 - simu-net");

DEFINE_string(run_routine, "",
    "the routine's name to run");

DEFINE_bool(list_routines, false,
    "list all available routines");
