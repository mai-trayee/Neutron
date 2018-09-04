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

	std::vector<std::vector<double> > vData(vSize.size());
	double Range = 8.0/Bins;

	for (unsigned int i = 0; i < vSize.size(); ++i)
	{
		TFile InFile(vFile.at(i).c_str());	//OPEN mode
		TTree *Data = dynamic_cast<TTree*>(InFile.Get("Data"));

		double Initial, Final, Absorb;
		TBranch *b_fInitial, *b_fFinal, *b_fAbsorb;
		Data->SetBranchStatus("*", 1);
		Data->SetBranchAddress("Initial", &Initial, &b_fInitial);
		Data->SetBranchAddress("Final",   &Final,   &b_fFinal);
		Data->SetBranchAddress("Absorb",  &Absorb,  &b_fAbsorb);

		vData.at(i).clear();
		vData.at(i).resize(Bins);
		std::vector<unsigned int> vCount(Bins);
		for (unsigned int n = 0; n < Data->GetEntries(); ++n)
		{
			Data->GetEntry(n);
			for (unsigned int j = 0; j < Bins; ++j)
			{
				if (Initial >= j*Range && Initial < (j+1)*Range)
				{
					vData.at(i).at(j) += Absorb;
					++vCount.at(j);
				}
			}
		}

		for (unsigned int j = 0; j < vData.at(i).size(); ++j)
			vData.at(i).at(j) /= vCount.at(j);
	}

	for (unsigned int j = 0; j < Bins; ++j)
	{
		ssL.clear();
		ssL.str("");
		ssL << BaseName << std::setfill('0') << std::setw(4) << j*Range*1000 << ".dat";
		std::cout << "Energy bin " << j*Range << "\t in " << ssL.str() << std::endl;

		std::ofstream Out(ssL.str().c_str());
		for (unsigned int i = 0; i < vSize.size(); ++i)
			Out << vSize.at(i) << "\t" << vData.at(i).at(j) << std::endl;
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
