#include "../froth/core/Application.h"
#include <iostream>

class Playground : public Froth::Application {
public:
  Playground() { std::cout << "Hello World" << std::endl; }
};

Froth::Application *Froth::CreateApplication() { return new Playground(); }
