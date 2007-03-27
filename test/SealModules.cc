#include "PluginManager/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "CondTools/SiStrip/test/SiStripApvGainBuilder.h"
#include "CondTools/SiStrip/test/SiStripApvGainReader.h"

using cms::SiStripApvGainBuilder;
using cms::SiStripApvGainReader;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(SiStripApvGainBuilder);
DEFINE_ANOTHER_FWK_MODULE(SiStripApvGainReader);
