#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondTools/SiStrip/test/SiStripPerformanceSummaryBuilder.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondFormats/SiStripObjects/interface/SiStripPerformanceSummary.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h" 
#include "CLHEP/Random/RandGauss.h"
#include <sys/time.h>

using namespace cms;

SiStripPerformanceSummaryBuilder::SiStripPerformanceSummaryBuilder(const edm::ParameterSet& iConfig) {}
SiStripPerformanceSummaryBuilder::~SiStripPerformanceSummaryBuilder() {}

void SiStripPerformanceSummaryBuilder::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  SiStripPerformanceSummary* psummary = new SiStripPerformanceSummary();
  // fill object
/*
  std::vector<float> input_values;
  input_values.push_back(1.); input_values.push_back(2.); input_values.push_back(3.); input_values.push_back(4.); input_values.push_back(5.); input_values.push_back(6.); input_values.push_back(7.); input_values.push_back(8.); input_values.push_back(9.);
  psummary->set(1234567, input_values);
*/
  for(std::vector<uint32_t>::const_iterator idet = detectorModules_.begin(); idet != detectorModules_.end(); ++idet){
    // generate random values for each detId
    float clusterSizeMean = (float) RandGauss::shoot(4.,2.);
    float clusterSizeRMS  = (float) RandGauss::shoot(2.,1.);
    float clusterChargeMean = (float) RandGauss::shoot(70.,10.);
    float clusterChargeRMS  = (float) RandGauss::shoot(10.,1.);
    float occupancyMean = (float) RandGauss::shoot(50.,20.);
    float occupancyRMS  = (float) RandGauss::shoot(20.,4.);
    float noisyStrips = (float) RandGauss::shoot(7.,1.);
    // set values
    psummary->setClusterSize(*idet, clusterSizeMean, clusterSizeRMS);
    psummary->setClusterCharge(*idet, clusterChargeMean, clusterChargeRMS);
    psummary->setOccupancy(*idet, occupancyMean, occupancyRMS);
    psummary->setPercentNoisyStrips(*idet, noisyStrips);
  }
  clock_t presentTime = clock();
  psummary->setTimeValue((unsigned long long)presentTime);
  psummary->print();
  // Write to DB
  edm::Service<cond::service::PoolDBOutputService> poolDbService;
  if( poolDbService.isAvailable() ){
    try{
      if ( poolDbService->isNewTagRequest( "SiStripPerformanceSummaryRcd" ) ){
        edm::LogInfo("Tag")<<" is new tag request.";
        poolDbService->createNewIOV<SiStripPerformanceSummary>( psummary, poolDbService->endOfTime(),"SiStripPerformanceSummaryRcd"  );
      }else{
        edm::LogInfo("Tag")<<" tag exists already.";
        poolDbService->appendSinceTime<SiStripPerformanceSummary>( psummary, poolDbService->currentTime(),"SiStripPerformanceSummaryRcd" );
      }
    }catch(const cond::Exception& er){
      edm::LogError("DBWriting")<<er.what()<<std::endl;
    }catch(const std::exception& er){
      edm::LogError("DBWriting")<<"caught std::exception "<<er.what()<<std::endl;
    }catch(...){
      edm::LogError("DBWriting")<<"Funny error"<<std::endl;
    }
  }else{
    edm::LogError("PoolDBOutputService")<<" Service is unavailable"<<std::endl;
  }
}

void SiStripPerformanceSummaryBuilder::beginJob(const edm::EventSetup& iSetup) {
  edm::ESHandle<TrackerGeometry> pDD;
  iSetup.get<TrackerDigiGeometryRecord>().get( pDD );
  edm::LogInfo("SiStripApvGainBuilder") <<" There are "<<pDD->detUnits().size() <<" detectors"<<std::endl;
  for(TrackerGeometry::DetUnitContainer::const_iterator it = pDD->detUnits().begin(); it != pDD->detUnits().end(); it++){
    if( dynamic_cast<StripGeomDetUnit*>((*it))!=0){
      uint32_t detid=((*it)->geographicalId()).rawId();
      detectorModules_.push_back(detid);
    }
  }
  edm::LogInfo("Modules")<<" detectorModules_.size()="<<detectorModules_.size();
}

void SiStripPerformanceSummaryBuilder::endJob() {
}
