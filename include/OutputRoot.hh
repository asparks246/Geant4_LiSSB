#ifndef OutputRoot_H
#define OutputRoot_H 1
#include "G4RootAnalysisManager.hh" 

class OutputRoot 
{
	public:
    static OutputRoot* GetInstance()
    {
       if(instance)
           return instance;
       else
       {
          instance = new OutputRoot();
          return instance;
       }
    }
	void SaveOutput();
	void CreateFile(G4String filename);
	//void AddHit(int eventId, double enFront, double enRear2h, double enRear4h, 
				//double enRear6h, double enRear8h, double enRear10h, double enRear12h);
	void AddHit(int eventId, double enSmallDetector, double enBigDetector);
				
	private:
	OutputRoot();
	static OutputRoot* instance;
	G4RootAnalysisManager* rootManager;
	void CreateSiliTuple();
	int siliTupleId;

};

#endif
