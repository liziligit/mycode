#include "TH1F.h"
#include "TH2F.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TF1.h"

#include "MyEventClass.h"

const int NX = 72;
const int NY = 72;
const int nFrame = 758; 
const int nChannel = 1;

//-----------------------------------------------------------------------------------------------
//
class MyRootClass
{
  public:
    MyRootClass(TString fp, TString pp)
    {
        ip = 0;
        nped = 3;
        hPed = NULL;
        hPedm = NULL;
        hPeds = NULL;
        hAll1 = NULL;
		hqqq= NULL;
		hlength= NULL;
        filePath = fp;
        pedPath = pp;
    };
    ~MyRootClass();

    void ReadPed();
    void IniHistogram();

    //1---
    void Analysis(TString);
    void AnalysisDir(TString);
    void AnalysisFile(TString);
    void DrawRaw(const char *opt = "colz");
    void DrawPre(const char *opt = "colz");
    void DrawNext(const char *opt = "colz");
    void DrawSelected(int, const char *opt = "colz");
    void ButtonFunc11();
    TString *GetInfo();

    //2---
    void ButtonFunc21(int, const char *opt = "colz");
    void ButtonFunc22(int, const char *opt = "colz");
    void ButtonFunc23(int, const char *opt = "colz");

    //3---
    void ButtonFunc31(double);
    void ButtonFunc32();
    void ButtonFunc33();
    void AnalysisPed(TString path);

    int GetIp() { return ip; }
    int GetNEvent() { return fEventList.size(); }

  private:
    int ip;
    int nEvent;
    double nped;
	double devaresult;
	double alldeva=0;
	unsigned short storeAllData[nFrame][NX][NY];  //单通道数组
    TH2F *hPed;
    TH2F *hPedm;
    TH2F *hPeds;
	

    TString filePath;
    TString rootFilePath;
    TString pedPath;
    TString rootPedPath;

	ofstream AllOfqOutput;

    TH2F *hAll1;       // plot all hits in one histogram
    TH2F *hAll2;       // plot all hits in one histogram (remove ped)
    TH2F *hBaryCenter; // plot the all barycenter
    TH2F *hIPoint;     // plot the all IP point;
    TH1F *hPol1;       // plot the barycenter line
    TH1F *hPol2;       // plot the polarization
	TH2F *hlength;
	TH1F *hqqq;
	TH2F *hqMean;
	TH2F *hqRMS;

    vector<TH1F *> fPed;
    vector<MyEventClass *> fEventList;
};

MyRootClass::~MyRootClass()
{
    // Destructor.
}

//______________________________________________________________________________
void MyRootClass::AnalysisPed(TString pp)
{
    pedPath = pp;

    if (hPedm != NULL)
    {
        delete hPed;
        delete hPedm;
        delete hPeds;
    }

    hPed = new TH2F("hPed", "Pedestal", NX, 1, NX + 1, NY, 1, NY + 1);
    hPedm = new TH2F("hPedm", "Pedestal mean", NX, 1, NX + 1, NY, 1, NY + 1);
    hPeds = new TH2F("hPeds", "Pedestal sigma", NX, 1, NX + 1, NY, 1, NY + 1);

    //----------
    cout << "--> Opening: " << pedPath << endl;
    vector<int> dvec[NX][NY];
    ifstream ifSignal(pedPath, ios::binary);

    while (ifSignal.good())
    {
        unsigned short _data[NX][NY];
        ifSignal.read((char *)(&_data), sizeof(_data));
        for (int i = 0; i < NX; i++)
            for (int j = 0; j < NY; j++)
                dvec[i][j].push_back(_data[i][j]);
    }

    //----------
    fPed.clear();
    for (int i = 0; i < NX; i++)
    {
        for (int j = 0; j < NY; j++)
        {
            int xmin = dvec[i][j][0];
            int xmax = dvec[i][j][0];
            for (int k = 0; k < (int)dvec[i][j].size(); k++)
            {
                xmin = (xmin < dvec[i][j][k]) ? xmin : dvec[i][j][k];
                xmax = (xmax > dvec[i][j][k]) ? xmax : dvec[i][j][k];
            }

            if (gDirectory->Get(Form("h%d", i * NY + j)) != NULL)
            {
                TH1F *f = (TH1F *)gDirectory->Get(Form("h%d", i * NY + j));
                delete f;
            }
            TH1F *f = new TH1F(Form("h%d", i * NY + j), Form("histogram for %d,%d", i, j), xmax - xmin + 10, xmin - 5, xmax + 5);
            for (int k = 0; k < (int)dvec[i][j].size(); k++)
                f->Fill(dvec[i][j][k]);

            fPed.push_back(f);
        }
    }

    for (int i = 0; i < NX; i++)
    {
        for (int j = 0; j < NY; j++)
        {
            hPedm->Fill(i + 1, j + 1, fPed[i * NY + j]->GetMean());
            hPeds->Fill(i + 1, j + 1, fPed[i * NY + j]->GetRMS());
            hPed->SetBinContent(i + 1, j + 1, hPedm->GetBinContent(i + 1, j + 1) + nped * hPeds->GetBinContent(i + 1, j + 1));
        }
    }

    ifSignal.close();

    rootPedPath = pedPath;
    if (rootPedPath.Index(".mdat") != -1)
        rootPedPath.Replace(rootPedPath.Index(".mdat"), 5, ".root");
    if (rootPedPath.Index(".dat") != -1)
        rootPedPath.Replace(rootPedPath.Index(".dat"), 4, ".root");

    TFile *rootPedFile = new TFile(rootPedPath, "recreate");
    hPedm->Write();
    hPeds->Write();
    hPed->Write();
    for (int i = 0; i < NX; i++)
    {
        for (int j = 0; j < NY; j++)
        {
            fPed[i * NY + j]->Write();
        }
    }
    rootPedFile->Close();

    cout << "---->Save ped root file to: " << rootPedPath << endl;

    hPedm->Draw("colz");
}

void MyRootClass::ButtonFunc31(double _ped)
{
    if (hPedm == NULL)
        return;
    if (hPeds == NULL)
        return;
    if (hPed == NULL)
        return;

    nped = _ped;

    for (int i = 0; i < NX; i++)
        for (int j = 0; j < NY; j++)
            hPed->SetBinContent(i + 1, j + 1, hPedm->GetBinContent(i + 1, j + 1) + nped * hPeds->GetBinContent(i + 1, j + 1));

    hPed->Draw("colz");
}

void MyRootClass::ButtonFunc32()
{
    if (hPedm != NULL)
        hPedm->Draw("colz");
}

void MyRootClass::ButtonFunc33()
{
    if (hPeds != NULL)
        hPeds->Draw("colz");
}

//______________________________________________________________________________
//
void MyRootClass::ReadPed()
{
    if (hPedm == NULL)
    {
        rootPedPath = pedPath;
        if (rootPedPath.Index(".mdat") != -1)
            rootPedPath.Replace(rootPedPath.Index(".mdat"), 5, ".root");
        if (rootPedPath.Index(".dat") != -1)
            rootPedPath.Replace(rootPedPath.Index(".dat"), 4, ".root");
        cout << "----> Read ped from " << rootPedPath << endl;

        TFile *rootPedFile = new TFile(rootPedPath);
        if (rootPedFile->IsOpen())
        {
            hPedm = (TH2F *)rootPedFile->Get("hPedm");
            hPeds = (TH2F *)rootPedFile->Get("hPeds");
            hPed = (TH2F *)rootPedFile->Get("hPeds");

            for (int i = 0; i < NX; i++)
                for (int j = 0; j < NY; j++)
                    hPed->SetBinContent(i + 1, j + 1, hPedm->GetBinContent(i + 1, j + 1) + nped * hPeds->GetBinContent(i + 1, j + 1));
        }
        else 
        {
            hPed = NULL;
            hPedm = NULL;
            hPeds = NULL;
        }
    }
}

void MyRootClass::IniHistogram()
{
    ip = 1;
    nEvent = 0;

    if (hAll1 != NULL)
    {
        delete hAll1;
        delete hAll2;
        delete hBaryCenter;
        delete hIPoint;
        delete hPol1;
        delete hPol2;
		delete hlength;
		delete hqqq;
    }
    hAll1 = new TH2F("hAll1", "Histogram of all hits", NX, 1, NX + 1, NY, 1, NY + 1);
    hAll2 = new TH2F("hAll2", "Histogram of all hits", NX, 1, NX + 1, NY, 1, NY + 1);
    hBaryCenter = new TH2F("hBaryCenter", "Histogram of all barycenters", NX, 1, NX + 1, NY, 1, NY + 1);
    hIPoint = new TH2F("hIPoint", "Histogram of all IP points", NX, 1, NX + 1, NY, 1, NY + 1);
    hPol1 = new TH1F("hPol1", "#theta of the bary-center line", 100, -TMath::Pi() , TMath::Pi() );
    hPol2 = new TH1F("hPol2", "#theta of the fitted Polarization \n abs(par[0]*sin(par[1]*x[0]))+par[2]", 50, 0, TMath::Pi());
	hPol2->GetXaxis()->SetTitle("#theta");
	hPol2->GetYaxis()->SetTitle("counts");
	//hlength = new TH1F("hlength","length of the longest",50,0,20);
	hqMean = new TH2F("hqMean", "the mean of all event", NX, 1, NX + 1, NY, 1, NY + 1);
	hqRMS = new TH2F("hqRMS", "the std Dev of all event", NX, 1, NX + 1, NY, 1, NY + 1);
	hqqq= new TH1F("hqqq","the all of q",200,0,50000);
    fEventList.clear();
}

void MyRootClass::AnalysisDir(TString fileDir)
{
    ReadPed();
    IniHistogram();

    FILE *fp = gSystem->OpenPipe("ls " + fileDir + "/*.data", "r");
    if (!fp)
    {
        cout << "----> NO data files exists in " << fileDir << "!" << endl;
        return;
    }

    vector<TString> dataList;
    char line[1000];
    while (fgets(line, sizeof(line), fp))
    {
        TString s(line);
        if (s.Index(".data") == -1)
            continue;
        dataList.push_back(s.ReplaceAll("\n", ""));
    }
    cout << "----> " << dataList.size() << " data files exist in " << fileDir << "." << endl;

    for (int i = 0; i < (int)dataList.size(); i++)
        AnalysisFile(dataList[i]);

	TCanvas *can11 = new TCanvas("can11","multigraph",10,10,700,500);
	can11->cd();
	hlength->Draw();

    cout << "----> " << nEvent << " events recorded." << endl;
}

//
void MyRootClass::Analysis(TString filePath)
{
	AllOfqOutput.open("/home/stuf/amexe/topmetal/abc.txt",ios::app); // 所有q保存到这里
    ReadPed();
    IniHistogram();
    AnalysisFile(filePath);
	//devaresult=alldeva/(nEvent-1);
	//outf<<filePath<<" "<<devaresult<<endl;   //putout D(x)

	/*
	TCanvas *can11 = new TCanvas("can11","multigraph",10,10,700,500);
	TCanvas *can12 = new TCanvas("can12","the all of q",20,20,700,500);
	can11->cd();
	hlength->Draw();
	can12->cd();
	hqqq->Draw();*/
	for (int i=0;i<NX;i++)
	{
		for (int j=0; j<NY; j++)
		{
			double sumOfq = 0;
			double avgOfq = 0;
			double standardResult =0;
			double standardq = 0;
			for (int k=0; k<nFrame; k++)	
			{
				sumOfq += storeAllData[k][i][j];
			}
			avgOfq = sumOfq/nFrame;
			hqMean->SetBinContent(i,j,avgOfq);
			hqMean->GetXaxis()->SetTitle("channel");
			hqMean->GetXaxis()->SetTitleOffset(1.5);
			hqMean->GetYaxis()->SetTitle("channel");
			hqMean->GetYaxis()->SetTitleOffset(1.5);
			hqMean->GetZaxis()->SetTitle("q");
			hqMean->GetZaxis()->SetTitleOffset(1.5);
			for (int a=0; a<nFrame; a++)
			{
				standardq +=pow(storeAllData[a][i][j]-avgOfq,2);
			}
			standardResult = sqrt(standardq/nFrame);
			hqRMS->SetBinContent(i,j,standardResult);
			hqRMS->GetXaxis()->SetTitle("channel");
			hqRMS->GetXaxis()->SetTitleOffset(1.5);
			hqRMS->GetYaxis()->SetTitle("channel");
			hqRMS->GetXaxis()->SetTitleOffset(1.5);
		}
	}
    cout << "----> " << nEvent << " events recorded111." << endl;
	TCanvas *can11 = new TCanvas("can11","multigraph",10,10,700,500);
	can11->Divide(2,1);
	can11->cd(1);
	hqMean->Draw("SURF3");
	can11->cd(2);
	hqRMS->Draw("SURF3");
}

void MyRootClass::AnalysisFile(TString filePath)
{
    cout << "--> Opening: " << filePath << endl;
	AllOfqOutput<<filePath<<endl;
    ifstream ifSignal(filePath, ios::binary);
    while (ifSignal.good())
    {
		//int numFrame = 0;
        unsigned short _data[NX][NY];
        ifSignal.read((char *)(&_data), sizeof(_data));

        if (_data[0][0] == 0xFFFF && _data[1][1] == 0xFFFF)
        {
            ifSignal.read((char *)(&_data), sizeof(_data));
            continue;
        }

        MyEventClass *fEvent = new MyEventClass(nEvent, 0, NX - 1, 0, NY - 1);
		double output =0;
        for (int i = 0; i < NX; i++)
            for (int j = 0; j < NY; j++)
            {
                fEvent->Fill(i, j, _data[i][j]);
                double q = _data[i][j]; // - hPed->GetBinContent(i + 1, j + 1);
                hAll1->Fill(i + 1, j + 1, (q < 0) ? 0 : q);
				storeAllData[nEvent][i][j] = q;
				output +=q;
				//numFrame++;
            }
		
		AllOfqOutput<<nEvent<<" "<<output<<endl;
        fEvent->GenerateHist(hPed);
        fEvent->Fill2DPlot(hAll2);
		fEvent->Fillhqqq(hqqq);
        fEvent->FillBaryCenter(hBaryCenter);
        fEvent->FillIPpoint(hIPoint);
		alldeva+=fEvent->Getdeva();
        fEvent->FillPolarization1(hPol1);
        fEvent->FillPolarization2(hPol2);
		//fEvent->Filllengththelongest(hlength);
        fEventList.push_back(fEvent);

        nEvent++;
		//delete fEvent;
        if (nEvent % 100 == 0)
            cout << "Dealed: " << nEvent << endl;
		//gObjectTable->Print();
		if (nEvent == nFrame*nChannel)  //运行一部分就退出
			break;
    }

    ifSignal.close();
	
}

void MyRootClass::DrawRaw(const char *opt)
{
    fEventList.at(ip)->Get2DRawPlot()->Draw(opt);
}

void MyRootClass::DrawPre(const char *opt)
{
    if (ip <= 0)
        return;

    int _ip = ip;
    while (_ip >= 1)
    {
        _ip--;
        if (fEventList.at(_ip)->GetDataQuality() == 1)
            break;
    }
    ip = _ip;
    fEventList.at(ip)->Draw2DResult(opt);
}

void MyRootClass::DrawNext(const char *opt)
{
    if ((unsigned long)ip == fEventList.size() - 1)
        return;

    int _ip = ip;
    while (_ip < (int)fEventList.size() - 1)
    {
        _ip++;
        if (fEventList.at(_ip)->GetDataQuality() == 1)
            break;
    }
    ip = _ip;
    fEventList.at(ip)->Draw2DResult(opt);
}

void MyRootClass::DrawSelected(int _ip, const char *opt)
{
    if (_ip < 0 || _ip >(int) fEventList.size() - 1)
        return;

    ip = _ip;
    fEventList.at(ip)->Draw2DResult(opt);
}

TString *MyRootClass::GetInfo()
{
    return fEventList.at(ip)->GetInfo();
}

void MyRootClass::ButtonFunc11()
{
    fEventList.at(ip)->DrawEllipse();
}

//______________________________________________________________________________

void MyRootClass::ButtonFunc21(int flag, const char *opt)
{
    if (flag == 1)
        hAll1->Draw(opt);
    else
        hAll2->Draw(opt);
}

void MyRootClass::ButtonFunc22(int flag, const char *opt)
{
    if (flag == 1)
        hBaryCenter->Draw(opt);
    else
        hIPoint->Draw(opt);
}

Double_t fitFunction(Double_t *x,Double_t *par){
	return abs(par[0]*sin(par[1]*x[0]))+par[2];
}
//Double_t fitLineFunction(Double_t *x,Double_t *par){
//	return par[0]*x[0]+par[1];}

void MyRootClass::ButtonFunc23(int flag, const char *opt)
{
	TF1 *hisFunc = new TF1("hisFunc",fitFunction,0,TMath::Pi(),3);
	//TF1 *hisFunc = new TF1("hisFunc",fitLineFunction,0,TMath::Pi(),2);
	//ofstream outf;
	//outf.open("/home/stuf/CXPD/track/abc.txt",ios::app);
    if (flag == 1)
        hPol1->Draw(opt);
    else{
		/*hisFunc->SetParameters(100,1,100);
		//hisFunc->SetParameters(0,200);
		hPol2->Fit(hisFunc,"r");
		hPol2->SetMinimum(200);*/
        hPol2->Draw(opt);
		/*hPol2->SetStats(kTRUE);
		gStyle->SetOptFit(1);
		double param1=hisFunc->GetParameter(0);       //输出到txt文件
		outf<<param1<<" ";
		double param2=hisFunc->GetParameter(2);
		outf<<param2<<" ";
		double parError1=hisFunc->GetParError(0);
		outf<<parError1<<" ";
		double parError2=hisFunc->GetParError(2);
		outf<<parError2<<" ";
		double thechi2=hisFunc->GetChisquare();
		outf<<thechi2<<" "<<endl;*/
		}
		
}
