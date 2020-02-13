#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <getopt.h>

#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TBranch.h"

void Usage(char* Name);
int main(int argc, char** argv)
{
	const struct option longopts[] = 
	{
		{"list", 	required_argument, 	0, 'l'},
		{"output", 	required_argument, 	0, 'o'},
		{"bins", 	required_argument, 	0, 'b'},
		{"help", 	no_argument,	 	0, 'h'},
		{0,	0, 	0,	0},
	};

	int index; 
	int iarg = 0;
	opterr = 1;
	
	std::string ListFile, BaseName;
	unsigned int Bins = 10;
	
	while((iarg = getopt_long(argc,argv, "l:o:b:h", longopts, &index)) != -1)
	{
		switch(iarg)
		{
			case 'l':
				ListFile.assign(optarg);
				break;
			case 'o':
				BaseName.assign(optarg);
				break;
			case 'b':
				Bins = strtol(optarg, NULL, 10);
				break;
			case 'h':
				Usage(argv[0]);
				return 1;
			default:
				break;
		}
	
	}

	std::ifstream List(ListFile.c_str());

	double Size;
	std::string RootFile;
	std::vector<double> vSize;
	std::vector<std::string> vFile;

	std::string Line;
	std::stringstream ssL;
	while (std::getline(List, Line))
	{
		if (Line[0] == '#') continue;

		ssL.clear();
		ssL.str("");

		ssL << Line;
		ssL >> Size >> RootFile;

		vSize.push_back(Size);
		vFile.push_back(RootFile);
	}

	List.close();

	std::vector<std::vector<double> > vEner(vSize.size());
	std::vector<std::vector<double> > vPerc(vSize.size());
	std::vector<double> vInitial(vSize.size());
	std::vector<double> vKinemat(vSize.size());
	std::vector<unsigned int> vNumber(vSize.size());
	double Range = 8.0/Bins;

	for (unsigned int i = 0; i < vSize.size(); ++i)
	{
		TFile InFile(vFile.at(i).c_str());	//OPEN mode
		TTree *Data = dynamic_cast<TTree*>(InFile.Get("Data"));

		double Initial, Final, Absorb;
		TBranch *b_fInitial, *b_fFinal, *b_fAbsorb;
		Data->SetBranchAddress("Initial", &Initial, &b_fInitial);
		Data->SetBranchAddress("Final",   &Final,   &b_fFinal);
		Data->SetBranchAddress("Absorb",  &Absorb,  &b_fAbsorb);
		Data->SetBranchStatus("*", 1);

		vEner.at(i).clear();
		vEner.at(i).resize(Bins);
		vPerc.at(i).clear();
		vPerc.at(i).resize(Bins);
		for (unsigned int n = 0; n < Data->GetEntries(); ++n)
		{
			Data->GetEntry(n);

			if (Final > 0)
			{
				vKinemat.at(i) += Final;
				++vNumber.at(i);
			}

			for (unsigned int j = 0; j < Bins; ++j)
			{
				if (Initial >= j*Range && Initial < (j+1)*Range)
				{
					vEner.at(i).at(j) += Absorb;
					vInitial.at(i) += Initial;

					vPerc.at(i).at(j) += (Final > 0);
				}
			}
		}

		for (unsigned int j = 0; j < Bins; ++j)
		{
			if (Data->GetEntries())
				vPerc.at(i).at(j) /= double(Data->GetEntries());
		}
	}

	std::ofstream OutE((BaseName+"_energy.dat").c_str());
	std::ofstream OutN((BaseName+"_number.dat").c_str());
	for (unsigned int i = 0; i < vSize.size(); ++i)
	{
		double Avg = 0.0;
		double Tot = 0.0;
		OutE << vSize.at(i);
		OutN << vSize.at(i);
		for (unsigned int j = 0; j < Bins; ++j)
		{
			OutE << "\t" << vEner.at(i).at(j);
			OutN << "\t" << vPerc.at(i).at(j);
			Avg += vEner.at(i).at(j);
			Tot += vPerc.at(i).at(j);
		}
		//OutE << "\t" << Avg/vInitial.at(i);
		std::cout << vSize.at(i) << "\t" << vKinemat.at(i) << "\t" << vNumber.at(i) << std::endl;
		OutE << "\t" << vKinemat.at(i) / vNumber.at(i);
		OutN << "\t" << Tot;

		OutE << std::endl;
		OutN << std::endl;
	}

	return 0;
}
	
void Usage(char *Name)
{
	std::cout << "Description" << std::endl;
	std::cout << "Usage : " << std::endl;
	std::cout << Name << " [OPTIONS]" << std::endl;
	std::cout <<"\n  -1,  --option1" << std::endl;
	std::cout << "\t\tDescription" << std::endl;
	std::cout <<"\n  -2,  --option2" << std::endl;
	std::cout << "\t\tDescription" << std::endl;
	std::cout <<"\n  -h,  --help" << std::endl;
	std::cout << "\t\tPrint this message and exit" << std::endl;
}
