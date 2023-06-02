#include <Core/engine.h>

constexpr int ERROR_CMDLINE = 128;

#if defined(WAVE_PLATFORM_WINDOWS) && !defined(DEBUG) && !defined(TESTING)

int WinMain(int argc, char **argv)
{
  //Read args from command line.
  if (argc > 1 && argv)
  {
    if (strcmp(argv[1], "--logs") == 0 || strcmp(argv[1], "-l") == 0)
    {
      Wave::print_logs();
      return 0;
    }
    else if (strcmp(argv[1], "--reset") == 0 || strcmp(argv[1], "-r") == 0)
    {
      Wave::reset_logs();
      return 0;
    }
    else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
      puts("-[h|l|r]");
      return 0;
    }
    else
    {
      fprintf(stderr, "[CMDLINE ERROR] : ERROR WHILE PARSING CMDLINE ARGS!\tRE-RUN"
                      " USING -h OR --help TO VIEW ALL ARGS RECOGNIZED...\n");
      return ERROR_CMDLINE;
    }
  }
  // Setup and launch engine.
  Wave::Engine *app = Wave::create_app();
  int exit_status;
  // Start and run app in engine.
  log_task("App", RED, 1, "Initialising app ...", app->init(), "App initialised")
  app->run();
  exit_status = (int) app->get_exit_status();
  delete app;
  return exit_status;
}

#elif !defined(TESTING)

int main(int argc, char **argv)
{
  //Read args from command line.
  if (argc > 1 && argv)
  {
    if (strcmp(argv[1], "--logs") == 0 || strcmp(argv[1], "-l") == 0)
    {
      Wave::print_logs();
      return 0;
    } else if (strcmp(argv[1], "--reset") == 0 || strcmp(argv[1], "-r") == 0)
    {
      Wave::reset_logs();
      return 0;
    } else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
      puts("-[h|l|r]");
      return 0;
    } else
    {
      fprintf(stderr, "[CMDLINE ERROR] : ERROR WHILE PARSING CMDLINE ARGS!\tRE-RUN"
                      " USING -h OR --help TO VIEW ALL ARGS RECOGNIZED...\n");
      return ERROR_CMDLINE;
    }
  }
  // Setup and launch engine.
  Wave::Engine *app = Wave::create_app();
  int exit_status;
  // Start and run app in engine.
  WAVE_LOG_TASK("App", RED, 1, "Building app ...", app->build(), "App built")
  app->run();
  exit_status = Wave::Engine::get_exit_status();
  WAVE_LOG_TASK("App", RED, 1, "Destroying app ...", app->unbuild(), "Cleaning up")
  delete app;
  return exit_status;
}

#endif