#include "LayerStack.h"
#include <memory>

namespace Froth {

LayerStack::~LayerStack() {
  for (auto layer : m_Layers) {
    layer->onDetatch();
  }
}

void LayerStack::pushLayer(std::shared_ptr<Layer> layer) {
  m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
  m_LayerInsertIndex++;
}

void LayerStack::pushOverlay(std::shared_ptr<Layer> overlay) {
  m_Layers.emplace_back(std::move(overlay));
}

} // namespace Froth
