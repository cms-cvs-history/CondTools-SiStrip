#ifndef SiStripApvGainBuilder_H
#define SiStripApvGainBuilder_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"


namespace cms{
  class SiStripApvGainBuilder : public edm::EDAnalyzer {

  public:

    explicit SiStripApvGainBuilder( const edm::ParameterSet& iConfig);

    ~SiStripApvGainBuilder(){};

    virtual void beginJob( const edm::EventSetup& );

    virtual void analyze(const edm::Event& , const edm::EventSetup& );

  private:
    bool printdebug_;
    std::vector< std::pair<uint32_t, unsigned short> > detid_apvs;
  };
}
#endif
