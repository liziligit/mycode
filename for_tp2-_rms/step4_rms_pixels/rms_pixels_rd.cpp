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
char *radiationFile;
radiationFile = argv[1];
char *txt_type;
txt_type = argv[2];
// char *store_file_name;
// store_file_name = argv[3];

char rdfn[100];
char rdfn2[100];
sprintf(rdfn,"%s.txt", radiationFile);
//sprintf(beamfn,"../data/runData/%s/", store_file_name);

Double_t noise[100];
Int_t num[2];

TCanvas *c1=new TCanvas("cc","rms",1800,1200);

FILE *n1=fopen(rdfn,"r+");

if (!n1) {
      std::cout << "Unable to open "<< rdfn <<" file "  <<std::endl;
      return -1; 
   }  

sprintf(rdfn2,"./combine_100/%s_new100rms.txt", txt_type);
ofstream n2(rdfn2,ios::out);
//output rd_new100rms.txt or nord_new100rms.txt
assert(n2); 

Double_t x[100];
Double_t y[100]; 

//int i=0;
//int j=0;i0=0,i1=0,i2=0,i3=0,i4=0,i5=0,i6=0,i7=0;
TGraph *nois[41472];
int j=0;

while(fscanf(n1,"%i%i%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf\
	%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf\
	%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf\
	%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf\
	%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf\
	%lf%lf",&num[0],&num[1],&noise[0]\
	,&noise[1],&noise[2],&noise[3],&noise[4],&noise[5],&noise[6],&noise[7],&noise[8],&noise[9]\
	,&noise[10],&noise[11],&noise[12],&noise[13],&noise[14],&noise[15],&noise[16],&noise[17],&noise[18],&noise[19]\
	,&noise[20],&noise[21],&noise[22],&noise[23],&noise[24],&noise[25],&noise[26],&noise[27],&noise[28],&noise[29]\
	,&noise[30],&noise[31],&noise[32],&noise[33],&noise[34],&noise[35],&noise[36],&noise[37],&noise[38],&noise[39]\
	,&noise[40],&noise[41],&noise[42],&noise[43],&noise[44],&noise[45],&noise[46],&noise[47],&noise[48],&noise[49]\
	,&noise[50],&noise[51],&noise[52],&noise[53],&noise[54],&noise[55],&noise[56],&noise[57],&noise[58],&noise[59]\
	,&noise[60],&noise[61],&noise[62],&noise[63],&noise[64],&noise[65],&noise[66],&noise[67],&noise[68],&noise[69]\
	,&noise[70],&noise[71],&noise[72],&noise[73],&noise[74],&noise[75],&noise[76],&noise[77],&noise[78],&noise[79]\
	,&noise[80],&noise[81],&noise[82],&noise[83],&noise[84],&noise[85],&noise[86],&noise[87],&noise[88],&noise[89]\
	,&noise[90],&noise[91],&noise[92],&noise[93],&noise[94],&noise[95],&noise[96],&noise[97],&noise[98],&noise[99])!=EOF)
//for(i = 0; i<=41472;i++){
 //fscanf(n1,"%lf%lf%lf%lf",&noise[0],&noise[1],&noise[2],&noise[3]);
{
	for(int i=0;i<100;i++){
	x[i]=i;
	y[i]=noise[i];
	}

nois[j]=new TGraph(100,x,y);
double xx = nois[j]->GetRMS(2);
n2 << num[0] << " " << num[1] << " " << fixed << setprecision(6) << xx <<"\n";
j++;
}

/*
//cout<<i0<<" "<<i1<<" "<<i2<<" "<<i3<<" "<<i4<<" "<<i5<<" "<<i6<<" "<<i7<<endl;
c1->Divide(4,2);
TGraph *nois[8];
TF1 *fpol[8];

n2<< pedeId << " ";
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
c1->SaveAs(TString::Format("./rms_txt/rms_beam_%d.png",pedeId));
*/
fclose(n1);
n2.close();
return 0;
}
