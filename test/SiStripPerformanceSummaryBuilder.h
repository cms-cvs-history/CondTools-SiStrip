#ifndef SiStripPerformanceSummaryBuilder_H
#define SiStripPerformanceSummaryBuilder_H
// -*- C++ -*-
// Package:    CondTools/SiStrip
// Class:      SiStripPerformanceSummaryBuilder
/**\class SiStripPerformanceSummaryBuilder SiStripPerformanceSummaryBuilder.cc CondTools/SiStrip/test/SiStripPerformanceSummaryBuilder.cc
 Description: <one line class summary>
 Implementation:
     <Notes on implementation>
*/
// Original Author:  Dorian Kcira
//         Created:  Mon Apr 30 17:46:00 CEST 2007
// $Id$
#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace cms{
  class SiStripPerformanceSummaryBuilder : public edm::EDAnalyzer {
    public:
      explicit SiStripPerformanceSummaryBuilder(const edm::ParameterSet&);
      ~SiStripPerformanceSummaryBuilder();
    private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
    private:
      std::vector<uint32_t> detectorModules_;
  };
}
#endif
