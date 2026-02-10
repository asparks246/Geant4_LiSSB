#ifndef OutputRoot_step_H
#define OutputRoot_step_H 1
#include "G4RootAnalysisManager.hh" 

class OutputRoot_step
{
	public:
    static OutputRoot_step* GetInstance()
    {
       if(instance)
           return instance;
       else
       {
          instance = new OutputRoot_step();
          return instance;
       }
    }
	void SaveOutput_step();
	void CreateFile_step(G4String filename);
	void AddHit_step(int ID, int eventId, int stepId, int volId, int particleID, int processID, double deltaEn, double x_mom, double y_mom, double z_mom, double x_prePos, double y_prePos, double z_prePos, double x_postPos, double y_postPos, double z_postPos, double deltaTime, double globalTime_pre, double globalTime_post, double properTime_pre, double properTime_post, int trackID, int parentID, double kineticEnergy);
	private:
	OutputRoot_step();
	static OutputRoot_step* instance;
	G4RootAnalysisManager* rootManager;
	void CreateSiliTuple_step();
	int siliTupleId_step;

};

#endif
