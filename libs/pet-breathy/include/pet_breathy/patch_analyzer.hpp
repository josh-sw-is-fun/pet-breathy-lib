#pragma once

#include "pet_breathy/signal_analyzer.hpp"

namespace pb {

class PatchAnalyzer {
  public:
    PatchAnalyzer(SignalAnalyzer& analyzer);

  private:
    SignalAnalyzer& m_analyzer;
};

} // namespace pb
