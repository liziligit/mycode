#include <iostream>
#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TROOT.h>
#include <TMath.h>
#include <TParameter.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TString.h>
#include <TVirtualFFT.h>
#include <TRandom.h>
#include <TStyle.h>
#include <TPad.h>
#include <TPaveStats.h>
#include <TSystem.h>
#include <TF1.h>
#include <TApplication.h>
#include <TLatex.h>
#include <TText.h>
#include <fstream>
#include <iomanip>//scientific format

using namespace std;
int main(int argc, char **argv)
{
char *pedefile;
pedefile = argv[1];
int pedeId;
pedeId = atol(argv[2]);
// char *store_file_name;
// store_file_name = argv[3];

char pedefn[100];
sprintf(pedefn,"%s_%d.txt", pedefile, pedeId);
//sprintf(beamfn,"../data/runData/%s/", store_file_name);

Double_t noise[4];

TCanvas *c1=new TCanvas("cc","rms",1800,1200);

//FILE *n=fopen(no,"r+");
FILE *n1=fopen(pedefn,"r+");

if (!n1) {
      std::cout << "Unable to open pd1 file "  <<std::endl;
      return -1; 
   }  

//FILE *n2=fopen("./rms_txt/rms.txt","r+");
ofstream n2("./rms_txt/rms.txt");

// if (!n2) {
//       std::cout << "Unable to open rms.txt file "  <<std::endl;
//       return -1; 
//    }  

Double_t x[8][5184];
Double_t y[8][5184]; 

//int i=0;
int i0=0,i1=0,i2=0,i3=0,i4=0,i5=0,i6=0,i7=0;

while(fscanf(n1,"%lf%lf%lf%lf",&noise[0],&noise[1],&noise[2],&noise[3])!=EOF)
//for(i = 0; i<=41472;i++){
 //fscanf(n1,"%lf%lf%lf%lf",&noise[0],&noise[1],&noise[2],&noise[3]);
{
	if(noise[0] == 0){
	x[0][i0]=noise[1];
	y[0][i0]=noise[3];
	i0++;
	//cout<<y[0][i]<<endl;	
	}
	else if(noise[0] == 1){
	x[1][i1]=noise[1];
	y[1][i1]=noise[3];
	i1++;
	//cout<<x[1][i]<<" "<<y[1][i]<<endl;	
	}
	else if(noise[0] == 2){
	x[2][i2]=noise[1];
	y[2][i2]=noise[3];
	i2++;
	//cout<<x[2][i]<<" "<<y[2][i]<<endl;	
	}
	else if(noise[0] == 3){
	x[3][i3]=noise[1];
	y[3][i3]=noise[3];
	i3++;
	//cout<<x[3][i]<<" "<<y[3][i]<<endl;	
	}
	else if(noise[0] == 4){
	x[4][i4]=noise[1];
	y[4][i4]=noise[3];
	i4++;
	//cout<<x[4][i]<<" "<<y[4][i]<<endl;	
	}
	else if(noise[0] == 5){
	x[5][i5]=noise[1];
	y[5][i5]=noise[3];
	i5++;
	//cout<<x[5][i]<<" "<<y[5][i]<<endl;	
	}
	else if(noise[0] == 6){
	x[6][i6]=noise[1];
	y[6][i6]=noise[3];
	i6++;
	//cout<<x[6][i]<<" "<<y[6][i]<<endl;	
	}
	else if(noise[0] == 7){
	x[7][i7]=noise[1];
	y[7][i7]=noise[3];
	i7++;
	//cout<<x[7][i]<<" "<<y[7][i]<<endl;	
	}
	else{
	continue;
	}
//i++;
//cout<<i<<endl;
//}
}
//cout<<i0<<" "<<i1<<" "<<i2<<" "<<i3<<" "<<i4<<" "<<i5<<" "<<i6<<" "<<i7<<endl;
c1->Divide(4,2);
TGraph *nois[8];
TF1 *fpol[8];


for(int j=0;j<8;j++)
{
nois[j]=new TGraph(5184,x[j],y[j]);
nois[j]->SetTitle(Form("chip[%i]",j)); 
nois[j]->GetXaxis()->SetTitle("Pixel Number");
nois[j]->GetYaxis()->SetTitle("rms of pixel");
nois[j]->GetXaxis()->SetLimits(0,5200);
//nois[j]->GetYaxis()->SetLimits(0,40);
nois[j]->GetYaxis()->SetRangeUser(1,5);
//Create the fitting function
fpol[j] = new TF1("fpol", "pol", 0, 5184);
fpol[j]->SetLineWidth(2);
nois[j]->Fit(fpol[j], "Q");

c1->cd(j+1);
nois[j]->Draw("ap");
nois[j]->SetMarkerStyle(7);
nois[j]->SetMarkerSize(0.7);
double x = nois[j]->GetRMS(2);
/*
//Return RMS of X (axis=1) or Y (axis=2)
 Double_t TGraph::GetRMS(Int_t axis) const
 {
    if (axis < 1 || axis > 2) return 0;
    if (fNpoints <= 0) return 0;
    Double_t sumx = 0, sumx2 = 0;
    for (Int_t i = 0; i < fNpoints; i++) {
       if (axis == 1) {
          sumx += fX[i];
          sumx2 += fX[i] * fX[i];
       } else           {
          sumx += fY[i];
          sumx2 += fY[i] * fY[i];
       }
    }
    Double_t x = sumx / fNpoints;
    Double_t rms2 = TMath::Abs(sumx2 / fNpoints - x * x);
    return TMath::Sqrt(rms2);
 } 
*/
//cout<<x<<endl;
n2<< fixed << setprecision(6) << x <<" ";
TLatex* lt = new TLatex();
lt->SetTextSize(0.06);
lt->SetTextAlign(13);
lt->DrawLatexNDC(0.15,0.85,TString::Format("%.6f",x));
c1->Modified();
c1->Update();
}
//c1->Draw();
c1->SaveAs(TString::Format("rms_beam_%d.png",pedeId));

fclose(n1);
return 0;
}
