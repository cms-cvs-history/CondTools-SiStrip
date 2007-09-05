#include "FWCore/PluginManager/interface/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "CondTools/SiStrip/test/SiStripApvGainBuilder.h"
#include "CondTools/SiStrip/test/SiStripApvGainReader.h"
#include "CondTools/SiStrip/test/SiStripPerformanceSummaryBuilder.h"
#include "CondTools/SiStrip/test/SiStripPerformanceSummaryReader.h"

using cms::SiStripApvGainBuilder;
using cms::SiStripApvGainReader;
using cms::SiStripPerformanceSummaryBuilder;
using cms::SiStripPerformanceSummaryReader;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(SiStripApvGainBuilder);
DEFINE_ANOTHER_FWK_MODULE(SiStripApvGainReader);
DEFINE_ANOTHER_FWK_MODULE(SiStripPerformanceSummaryBuilder);
DEFINE_ANOTHER_FWK_MODULE(SiStripPerformanceSummaryReader);
