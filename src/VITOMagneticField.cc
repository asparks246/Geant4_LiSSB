#include <fstream>
#include <functional>
#include <algorithm>
#include <iterator>
#include "Exception.hh"
#include "G4SystemOfUnits.hh"
#include "VITOMagneticField.hh"

VITOMagneticField::VITOMagneticField(const char* Bz1File, const char* Br1File, const char* Bz2File, const char* Br2File) 
{    

	ReadFile(Bz1File, Bz1, r1Val, z1Val);
	ReadFile(Br1File, Br1, r1Val, z1Val);
	ReadFile(Bz2File, Bz2, r2Val, z2Val);
	ReadFile(Br2File, Br2, r2Val, z2Val);
	CheckLimits();
}


void VITOMagneticField::ReadFile(const char* filename, vector< vector< double > >& data, vector<double>& rVal, vector<double>& zVal)
{
  double lenUnit= meter;
  double fieldUnit= tesla; 
  G4cout << "\n-----------------------------------------------------------"
	 << "\n      Magnetic field"
	 << "\n-----------------------------------------------------------";
    
  G4cout << "\n ---> " "Reading the field grid from " << filename << " ... " << endl; 


  fstream file;
  file.open( filename ); // Open the file for reading.
  
	if (!file.is_open())
		throw IOException("Warning message: The file " + (string) filename + " is not open!");
	

	std::string radiusVal;
	getline(file, radiusVal);
	std::istringstream rBuffer(radiusVal);
    std::vector<double> rLine((std::istream_iterator<double>(rBuffer)),
                             std::istream_iterator<double>());
    std::transform(rLine.begin(), rLine.end(), rLine.begin(),
               std::bind(std::multiplies<double>(), std::placeholders::_1, lenUnit));
    if(rVal.size() == 0)
    {
    	rVal=rLine; 
    	std::cout << rVal.size() << std::endl;
    }                       

	std::string temp;
	while (!file.eof())
	{
		std::getline(file, temp);
		if(temp == "")
			break;
    	std::istringstream buffer(temp);
    	std::vector<double> line((std::istream_iterator<double>(buffer)),
                             	std::istream_iterator<double>());
        if (!std::binary_search(zVal.begin(), zVal.end(), line.at(0)*lenUnit))  
        {                   	
			zVal.push_back(line.at(0)*lenUnit);
		}
		line.erase (line.begin());
		std::transform(line.begin(), line.end(), line.begin(),
               std::bind(std::multiplies<double>(), std::placeholders::_1, fieldUnit));
    	data.push_back(line);
	}
}

void VITOMagneticField::CheckLimits()
{
	minZ1=z1Val.at(0);
	maxZ1=z1Val.back();
	minR1=r1Val.at(0);
	maxR1=r1Val.back();
	minZ2=z2Val.at(0);
	maxZ2=z2Val.back();
	minR2=r2Val.at(0);
	maxR2=r2Val.back();
	
	std::cout << "First magnetic field: " << std::endl;
	std::cout << "Z min: " << minZ1 << " Z max: " << maxZ1 << " size: " << z1Val.size() << std::endl;
	std::cout << "R min: " << minR1 << " R max: " << maxR1 << " size: " << r1Val.size() << std::endl;
	std::cout << "Axiar field size: " << Bz1.size() << " x " << Bz1.at(0).size() << std::endl;
	std::cout << "Radial field size: " << Br1.size() << " x " << Br1.at(0).size() << std::endl;

	std::cout << "Second magnetic field: " << std::endl;
	std::cout << "Z min: " << minZ2 << " Z max: " << maxZ2 << " size: " << z2Val.size() << std::endl;
	std::cout << "R min: " << minR2 << " R max: " << maxR2 << " size: " << r2Val.size() << std::endl;
	std::cout << "Axiar field size: " << Bz2.size() << " x " << Bz2.at(0).size() << std::endl;
	std::cout << "Radial field size: " << Br2.size() << " x " << Br2.at(0).size() << std::endl;

}



void VITOMagneticField::GetFieldValue(const double point[4], double *Bfield ) const
{

	double x = point[0];
	double y = point[1];
	double z = point[2];
	
	vector< vector< double > > Bz;
	vector< vector< double > > Br;
	vector<double> rVal;
	vector<double> zVal;
	
	double z_mem;
	z_mem = z;
	if(z<0)
	{
		z=-z;
	}		
	double r=pow(x*x+y*y, 0.5);
	//std::cout << r << std::endl;
	//double fieldNr;
	
	if(z<450 && r<78)
	{
		//fieldNr=1;
		Bz = Bz1;
		Br = Br1;
		rVal = r1Val;
		zVal = z1Val;
		double dr = (rVal.back() - rVal.at(0))/(double(rVal.size()-1));
		double dz= (zVal.back() - zVal.at(0))/(double(zVal.size()-1));
	
		int rpos = int ((r-rVal.at(0))/dr);
		if(!(r>=rVal.at(rpos) && r<=rVal.at(rpos+1)))
		{
			std::cout << "VITOMagneticField::GetFieldValue( const  double Point[4], double *Bfield, int fieldNr) something is wrong" << endl;
			std::cout << "r: " << r << " " << rpos << " " << rVal.at(rpos) << " " << rVal.at(rpos+1) << " " << dr << endl;	
		}
		int zpos = int ((z-zVal.at(0))/dz);
		if(!(z>=zVal.at(zpos) && z<=zVal.at(zpos+1)))
		{
			std::cout << "VITOMagneticField::GetFieldValue( const  double Point[4], double *Bfield, int fieldNr) something is wrong" << endl;
			std::cout << "z: " << z << " " << zpos << " " << zVal.at(zpos) << " " << zVal.at(zpos+1) << " " << dz <<endl;	
		}
		
		double rDist = (r-rVal.at(rpos))/dr;
		double BrVal = (Br.at(zpos)).at(rpos) + ((Br.at(zpos)).at(rpos+1) - (Br.at(zpos)).at(rpos))*rDist;
		double zDist = (z-zVal.at(zpos))/dz;
		double BzVal = (Bz.at(zpos)).at(rpos) + ((Bz.at(zpos+1)).at(rpos) - (Bz.at(zpos)).at(rpos))*zDist;

		if(r>0)
		{
			if(z_mem>=0)
			{
				Bfield[0] = (BrVal*x/r)*1e03 *tesla;
    				Bfield[1] = (BrVal*y/r)*1e03 *tesla;
    				Bfield[2] = (BzVal)*1e03 *tesla;
    				//std::cout << Bfield[2] << std::endl;
    			}
    			else if(z_mem<0)
    			{
    				Bfield[0] = -(BrVal*x/r)*1e03 *tesla;
    				Bfield[1] = -(BrVal*y/r)*1e03 *tesla;
    				Bfield[2] = (BzVal)*1e03 *tesla;
    			}
    		}
    		else
    		{
    			Bfield[0] = 0 *tesla;
    			Bfield[1] = 0 *tesla;
    			Bfield[2] = (BzVal)*1e03 *tesla;
    		}
    		//std::cout << Bfield[0] << " " << Bfield[1] << " " << Bfield[2] << std::endl;
	}
	else if(z>=450 && z<1000 && r<550)
	{
		//fieldNr=2;
		Bz = Bz2;
		Br = Br2;
		rVal = r2Val;
		zVal = z2Val;
		double dr = (rVal.back() - rVal.at(0))/(double(rVal.size()-1));
		double dz= (zVal.back() - zVal.at(0))/(double(zVal.size()-1));
	
		int rpos = int ((r-rVal.at(0))/dr);	
		if(!(r>=rVal.at(rpos) && r<=rVal.at(rpos+1)))
		{
			std::cout << "VITOMagneticField::GetFieldValue( const  double Point[4], double *Bfield, int fieldNr) something is wrong" << endl;
			std::cout << "r: " << r << " " << rpos << " " << rVal.at(rpos) << " " << rVal.at(rpos+1) << " " << dr << endl;	
		}
		
		int zpos = int ((z-zVal.at(0))/dz);
		if(!(z>=zVal.at(zpos) && z<=zVal.at(zpos+1)))
		{
			std::cout << "VITOMagneticField::GetFieldValue( const  double Point[4], double *Bfield, int fieldNr) something is wrong" << endl;
			//std::cout << "z: " << z << " " << zpos << " " << zVal.at(zpos) << " " << zVal.at(zpos+1) << " " << dz <<endl;	
		}
		double rDist = (r-rVal.at(rpos))/dr;
		double BrVal = (Br.at(zpos)).at(rpos) + ((Br.at(zpos)).at(rpos+1) - (Br.at(zpos)).at(rpos))*rDist;
		double zDist = (z-zVal.at(zpos))/dz;
		double BzVal = (Bz.at(zpos)).at(rpos) + ((Bz.at(zpos+1)).at(rpos) - (Bz.at(zpos)).at(rpos))*zDist;
			
		if(r>0)
		{
			if(z_mem>=0)
			{
				Bfield[0] = (BrVal*x/r)*1e03 *tesla;
    				Bfield[1] = (BrVal*y/r)*1e03 *tesla;
    				Bfield[2] = BzVal*1e03 *tesla;
   			}
   			else if(z_mem<0)
   			{
   				Bfield[0] = -(BrVal*x/r)*1e03 *tesla;
    				Bfield[1] = -(BrVal*y/r)*1e03 *tesla;
    				Bfield[2] = BzVal*1e03 *tesla;
   			}
    		}
    		else
    		{
    			Bfield[0] = 0 *tesla;
    			Bfield[1] = 0 *tesla;
    			Bfield[2] = (BzVal)*1e03 *tesla;
    		}	
	}
	else 
	{
    		Bfield[0] = 0.0 *tesla;
    		Bfield[1] = 0.0 *tesla;
    		Bfield[2] = 0.0 *tesla;
    		//std::cout << x << " " << y << " " << z << std::endl;
	}
}
