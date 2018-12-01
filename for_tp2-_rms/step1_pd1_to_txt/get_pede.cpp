#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TROOT.h>
#include <TMath.h>
#include <TParameter.h>
#include <TGraph.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include "../../topmetal1X8/rootlib.h"

//generate pede file
//dataID is a serial numnber of file name
int get_pede(const char *path,const char *InName,const int dataID){
//input file and output buffer
	char Input[200];
	char Output[200];
//generate file names 
	sprintf(Input,"%s/%s%d.pd1",path,InName,dataID);
	sprintf(Output,"./beam_txt/%s%d.txt",InName,dataID);


//read data to pd1 class from disk

	placData_1 pd;

	pd.read(Input);

//check data validity
	cout<<"nFrame=="<<pd.nFrame()<<endl;
	cout<<"nPix=="<<pd.nPix()<<endl;
	cout<<"adcCha=="<<pd.adcCha()<<endl;

//calculate mean and rms

	ofstream of;
	of.open(Output);
	double tsum2[8];

	for(int k=0;k<pd.adcCha();k++){
	tsum2[k]=0;
	for(int i=0;i<pd.nPix();i++){
	double sum=0;
	double sum2=0;
	pd.getPixDat(k,i);
	for(int j=0;j<pd.nFrame();j++){

	sum+=pd.pixDat[j];
	sum2+=pd.pixDat[j]*pd.pixDat[j];

	}
	double np=pd.nFrame();
	sum/=np;  //mean
	sum2/=np;
	sum2=sqrt(sum2-sum*sum); //rms
//save data to file
	of<<k<<" "<<i<<" "<<sum<<" "<<sum2<<endl;
	tsum2[k]+=sum2;
	}
	tsum2[k]/=pd.nPix();
	cout<<"chip"<<k<<"average RMS = "<<tsum2[k]<<endl;
	}


	of.close();
	return 0;

}














