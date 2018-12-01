//lizili
//719 radiation test for 8 Topmetal-II-, this code is for calculating rms of each chip

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
char *rms_dir;
rms_dir = argv[1];
char *rmsfile;
rmsfile = argv[2];


char rmsfn[100];
//sprintf(rmsfn,"%s.txt", rmsfile);
sprintf(rmsfn,"%s%s.txt", rms_dir, rmsfile);
//sprintf(beamfn,"../data/runData/%s/", store_file_name);

Double_t rms_num[9];

TCanvas *c1=new TCanvas("cc","rms",1800,1200);

FILE *n1=fopen(rmsfn,"r+");

if (!n1) {
      std::cout << "Unable to open rms file "  <<std::endl;
      return -1; 
   }  

ofstream n2("./rms_rms_txt/noradiation_rms_rms.txt",ios::app);
//radiation_rms_rms.txt or noradiation_rms_rms.txt
assert(n2); 

Double_t x[8][100];
Double_t y[8][100]; 

int i=0;

while(fscanf(n1,"%lf%lf%lf%lf%lf%lf%lf%lf%lf",&rms_num[0],&rms_num[1],&rms_num[2],&rms_num[3],&rms_num[4],&rms_num[5],\
	&rms_num[6],&rms_num[7],&rms_num[8])!=EOF)
//for(i = 0; i<=41472;i++){
 //fscanf(n1,"%lf%lf%lf%lf",&noise[0],&noise[1],&noise[2],&noise[3]);
{
	
	x[0][i]=i;
	y[0][i]=rms_num[1];
	y[1][i]=rms_num[2];
	y[2][i]=rms_num[3];
	y[3][i]=rms_num[4];
	y[4][i]=rms_num[5];
	y[5][i]=rms_num[6];
	y[6][i]=rms_num[7];
	y[7][i]=rms_num[8];
	i++;	
}
//cout<<i0<<" "<<i1<<" "<<i2<<" "<<i3<<" "<<i4<<" "<<i5<<" "<<i6<<" "<<i7<<endl;
c1->Divide(4,2);
TGraph *nois[8];
TF1 *fpol[8];

//n2<< pedeId << " ";
for(int j=0;j<8;j++)
{
nois[j]=new TGraph(100,x[0],y[j]);
nois[j]->SetTitle(Form("chip[%i]",j)); 
nois[j]->GetXaxis()->SetTitle("Pixel Number");
nois[j]->GetYaxis()->SetTitle("rms' rms of chip");
nois[j]->GetXaxis()->SetLimits(0,105);
//nois[j]->GetYaxis()->SetLimits(0,40);
nois[j]->GetYaxis()->SetRangeUser(0,1);

//Create the fitting function
// fpol[j] = new TF1("fpol", "pol", 0, 100);
// fpol[j]->SetLineWidth(2);
// nois[j]->Fit(fpol[j], "Q");

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
n2 << fixed << setprecision(6) << x <<" ";
TLatex* lt = new TLatex();
lt->SetTextSize(0.06);
lt->SetTextAlign(13);
lt->DrawLatexNDC(0.15,0.85,TString::Format("%.6f",x));
c1->Modified();
c1->Update();
}
n2<<"\n";
//c1->Draw();
c1->SaveAs(TString::Format("./rms_rms_txt/rms_rms_%s.png",rmsfile));

fclose(n1);
n2.close();
return 0;
}
