// system include files
#include <memory>

// user include files

#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondFormats/SiStripObjects/interface/SiStripApvGain.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h" 
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetType.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"


#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"



#include "CondTools/SiStrip/test/SiStripApvGainBuilder.h"

using namespace std;
using namespace cms;

SiStripApvGainBuilder::SiStripApvGainBuilder( const edm::ParameterSet& iConfig ):
  printdebug_(iConfig.getUntrackedParameter<bool>("printDebug",false)){}

void SiStripApvGainBuilder::beginJob( const edm::EventSetup& iSetup ) {

  edm::ESHandle<TrackerGeometry> pDD;
  iSetup.get<TrackerDigiGeometryRecord>().get( pDD );     
  edm::LogInfo("SiStripApvGainBuilder") <<" There are "<<pDD->detUnits().size() <<" detectors"<<std::endl;
  
  for(TrackerGeometry::DetUnitContainer::const_iterator it = pDD->detUnits().begin(); it != pDD->detUnits().end(); it++){
  
    if( dynamic_cast<StripGeomDetUnit*>((*it))!=0){
      uint32_t detid=((*it)->geographicalId()).rawId();            
      const StripTopology& p = dynamic_cast<StripGeomDetUnit*>((*it))->specificTopology();
      unsigned short NAPVPairs = p.nstrips()/128;
      if(NAPVPairs<1 || NAPVPairs>6 ) {
	edm::LogError("SiStripApvGainBuilder")<<" Problem with Number of strips in detector.. "<< p.nstrips() <<" Exiting program"<<endl;
	exit(1);
      }
      detid_apvs.push_back( pair<uint32_t,unsigned short>(detid,NAPVPairs) );
      if (printdebug_)
	edm::LogInfo("SiStripApvGainBuilder")<< "detid " << detid << " apvpairs " << NAPVPairs;
    }
  }
}

void SiStripApvGainBuilder::analyze(const edm::Event& evt, const edm::EventSetup& iSetup){

  unsigned int run=evt.id().run();

  edm::LogInfo("SiStripApvGainBuilder") << "... creating dummy SiStripApvGain Data for Run " << run << "\n " << std::endl;

  SiStripApvGain* SiStripApvGain_ = new SiStripApvGain();

  for(std::vector< pair<uint32_t,unsigned short> >::const_iterator it = detid_apvs.begin(); it != detid_apvs.end(); it++){
    //Generate Noise for det detid
    std::vector<float> theSiStripVector;
    for(unsigned short j=0; j<it->second; j++){
      float gain= (j+1)*1000+ (RandFlat::shoot(1.)*100);
	if (printdebug_)
	  edm::LogInfo("SiStripApvGainBuilder") << "detid " << it->first << " \t"
					     << " apv " << j << " \t"
					       << gain    << " \t" 
					       << std::endl; 	    
	theSiStripVector.push_back(gain);
    }
  	    
      
    SiStripApvGain::Range range(theSiStripVector.begin(),theSiStripVector.end());
    if ( ! SiStripApvGain_->put(it->first,range) )
      edm::LogError("SiStripApvGainBuilder")<<"[SiStripApvGainBuilder::analyze] detid already exists"<<std::endl;
  }

  
  //End now write sistripnoises data in DB
  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  
  if( mydbservice.isAvailable() ){
    try{
      if( mydbservice->isNewTagRequest("SiStripApvGainRcd") ){
	mydbservice->createNewIOV<SiStripApvGain>(SiStripApvGain_,mydbservice->endOfTime(),"SiStripApvGainRcd");      
      } else {
	mydbservice->appendSinceTime<SiStripApvGain>(SiStripApvGain_,mydbservice->currentTime(),"SiStripApvGainRcd");      
      }
    }catch(const cond::Exception& er){
      edm::LogError("SiStripApvGainBuilder")<<er.what()<<std::endl;
    }catch(const std::exception& er){
      edm::LogError("SiStripApvGainBuilder")<<"caught std::exception "<<er.what()<<std::endl;
    }catch(...){
      edm::LogError("SiStripApvGainBuilder")<<"Funny error"<<std::endl;
    }
  }else{
    edm::LogError("SiStripApvGainBuilder")<<"Service is unavailable"<<std::endl;
  }
}
     
