
#include <vector>
#include <cmath>
#include "globals.hh"
#include "G4MagneticField.hh"
#include "G4ios.hh"
using namespace std;

class VITOMagneticField : public G4MagneticField
{
  
  // Storage space for the table
	vector< vector< double > > Bz1;
	vector< vector< double > > Br1;
	vector<double> r1Val;
	vector<double> z1Val;
  	// The physical limits of the defined region
  	double minZ1, maxZ1, minR1, maxR1;
  	
	vector< vector< double > > Bz2;
	vector< vector< double > > Br2;
	vector<double> r2Val;
	vector<double> z2Val;
  	// The physical limits of the defined region
  	double minZ2, maxZ2, minR2, maxR2;  	
  	 
  	
  	
public:
  VITOMagneticField(const char* Bz1File, const char* Br1File, const char* Bz2File, const char* Br2File);
  void  GetFieldValue( const  double point[4], double *Bfield) const;
  void ReadFile(const char* file, vector< vector< double > >& data, vector<double>& rVal, vector<double>& zVal);
  void CheckLimits();
};

