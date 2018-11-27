#include<iostream>
#include<string>
int result(const char *fName,const char *th,const char *no)
{
char id_hic;
char id_fpc;
//Double_t chip1,chip2,chip3,chip4,chip5,chip6,chip7,chip8,chip9,chip10,chip11,chip12,chip13,chip14;
Double_t chip[14];  //the same with double
Double_t thresh[14];
Double_t noise[14];
TH1F *chi[14];
for(int i=0;i<14;i++)
{
chi[i]=new TH1F(Form("chip[%i]",i),"",60,0,6);
chi[i]->GetXaxis()->SetTitle("result");
chi[i]->GetYaxis()->SetTitle("counts");
}
TH1F *h=new TH1F("h","chips total result distribution",60,0,6);
TH1F *thre=new TH1F("thre","threshold distribution",150,80,130);
TH1F *nois=new TH1F("nois","noise distribution",100,2,8);
h->SetXTitle("Total value");
h->SetYTitle("Counts");
thre->SetXTitle("threshold");
thre->SetYTitle("Counts");
nois->SetXTitle("noise");
nois->SetYTitle("Counts");
TCanvas *c=new TCanvas("c","result",800,600);
TCanvas *c1=new TCanvas("c1","threshold",800,600);
TCanvas *c2=new TCanvas("c2","chips results",1500,1200);
ifstream infile;
//const char *th="input.txt";
//const char *no="noise.txt";
FILE *input=fopen(fName,"r+");
FILE *t=fopen(th,"r+");
FILE *n=fopen(no,"r+");
if (!input) {
      std::cout << "Unable to open file "  <<std::endl;
      return -1; 
   }   

while(fscanf(input,"%s%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",&id_hic,&chip[0],&chip[1],&chip[2],&chip[3],&chip[4],&chip[5],&chip[6],&chip[7],&chip[8],&chip[9],&chip[10],&chip[11],&chip[12],&chip[13])!=EOF)
{
for(int i=0;i<14;i++)
{
h->Fill(chip[i]);
chi[i]->Fill(chip[i]);
}

}

while(fscanf(t,"%s%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",&id_fpc,&thresh[0],&thresh[1],&thresh[2],&thresh[3],&thresh[4],&thresh[5],&thresh[6],&thresh[7],&thresh[8],&thresh[9],&thresh[10],&thresh[11],&thresh[12],&thresh[13])!=EOF)
{

for(int j=0;j<14;j++)
{
if(thresh[j]<50||thresh[j]>500)continue;
else
thre->Fill(thresh[j]);
}
}

 while(fscanf(n,"%s%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",&id_fpc,&noise[0],&noise[1],&noise[2],&noise[3],&noise[4],&noise[5],&noise[6],&noise[7],&noise[8],&noise[9],&noise[10],&noise[11],&noise[12],&noise[13])!=EOF)
{
for(int b=0;b<14;b++)
{
if(noise[b]<1||noise[b]>10)continue;
else
nois->Fill(noise[b]);

}
}
c1->Divide(1,2);
c1->cd(1);
thre->Draw();
c1->cd(2);
nois->Draw();
c->cd();
h->Draw();
c2->Divide(7,2);
for(int i=0;i<14;i++)
{
c2->cd(i+1);
chi[i]->Draw();
c2->Modified();
c2->Update();
}
fclose(input);
fclose(t);
return 0;
}
