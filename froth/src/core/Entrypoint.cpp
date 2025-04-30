#include "Entrypoint.h"

int main(int argc, char *argv[]) {
  Froth::Application *app = Froth::CreateApplication();
  app->Run();

  delete app;
  return 0;
}
