#pragma once

namespace Froth {

class Application {
private:
public:
  Application();
  ~Application();
  void Run();
};
Application *CreateApplication();
} // namespace Froth
