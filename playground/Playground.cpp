#include "core/Entrypoint.h"
#include <memory>

class VulkanTriangle : public Froth::Layer {
public:
  virtual void onUpdate(double ts) override {}
};

class Playground : public Froth::Application {
public:
  Playground() { pushLayer(std::make_unique<VulkanTriangle>()); }
};

Froth::Application *Froth::CreateApplication() { return new Playground(); }
