#include "CondFormats/SiStripObjects/interface/SiStripApvGain.h"
#include "CondFormats/DataRecord/interface/SiStripApvGainRcd.h"

#include "CondTools/SiStrip/test/SiStripApvGainReader.h"

#include <iostream>
#include <stdio.h>
#include <sys/time.h>


using namespace cms;

SiStripApvGainReader::SiStripApvGainReader( const edm::ParameterSet& iConfig ):
  printdebug_(iConfig.getUntrackedParameter<bool>("printDebug",false)){};

SiStripApvGainReader::~SiStripApvGainReader(){}

void SiStripApvGainReader::analyze( const edm::Event& e, const edm::EventSetup& iSetup){

  edm::ESHandle<SiStripApvGain> SiStripApvGain_;
  iSetup.get<SiStripApvGainRcd>().get(SiStripApvGain_);
  edm::LogInfo("SiStripApvGainReader") << "[SiStripApvGainReader::analyze] End Reading SiStripApvGain" << std::endl;
  
  std::vector<uint32_t> detid;
  SiStripApvGain_->getDetIds(detid);
  
  if (printdebug_)
    for (size_t id=0;id<detid.size();id++)
      {
	SiStripApvGain::Range range=SiStripApvGain_->getRange(detid[id]);
	
	int apv=0;
	for(int it=0;it<range.second-range.first;it++){
	  edm::LogInfo("SiStripApvGainReader")  << "detid " << detid[id] << " \t"
					     << " apv " << apv++ << " \t"
					     << SiStripApvGain_->getApvGain(it,range)     << " \t" 
					     << std::endl; 	    
	} 
      }
}
