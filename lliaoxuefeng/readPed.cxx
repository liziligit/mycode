//____________________________________________
// 1个数据文件包含8个channel
// 1个channel包含72x72个pixel的数据
// 1个pixel的数据包含从第0frame到第808frame的数据顺序排列
// so:
// 1个数据文件大小为: 72x72x809
//
//
// 将其他不用的数据去掉，并按frame重新排列
// 
//  .pd1   ---->   .mdat
//
//                               - lq
//____________________________________________


const int nFrame = 758;  //帧数
//const int nFrame = 24;
const int nChannel = 8;
const int nXpixel = 72;
const int nYpixel = 72;
const int nLength = (nXpixel*nYpixel)*nFrame;
const int channel = 0;
unsigned short data[nLength*nChannel];
//unsigned short data[nLength];

const char *filetypes[] = {
                            "raw data file", "*.pd1",
                            0,               0 };


//-----
// 
TString SelectFile()
{
  static TString dir(".");
  TGFileInfo fi;
  fi.fFileTypes = filetypes;
  fi.fIniDir    = StrDup(dir);
  printf("fIniDir = %s\n", fi.fIniDir);
  new TGFileDialog(gClient->GetRoot(), NULL, kFDOpen, &fi);
  if(fi.fFilename==NULL) { printf("No file selected!\n"); return TString("NONE"); }
  printf("Open directory: %s\n", fi.fIniDir);
  return fi.fIniDir;
}

//___________________________
// main
void readPed()
{
    TString path = SelectFile();
    FILE *fp = gSystem->OpenPipe("ls "+path+"/*.pd1", "r");
    if(!fp) { cout<<"----> NO pd1 data exists in "<<path<<"!"<<endl; return;}
    
    vector <TString> pdList;    
    char line[1000];
    while(fgets(line, sizeof(line), fp))
    {
        TString s(line);
        if(s.Index(".pd1")==-1) continue;
        pdList.push_back(s.ReplaceAll("\n",""));
    } 
    cout<<"----> "<<pdList.size()<<" pd1 data files exist:"<<endl;


    for(int i=0; i<(int)pdList.size(); i++)
    {
        TString mdatName = pdList[i];
        mdatName.Replace(mdatName.Index(".pd1"), 4, ".mdat");

 
        cout<<"----> converting "<<pdList[i]<<" to "<<mdatName<<endl;;    
        
        ifstream ifSignal(pdList[i], ios::binary);

        ofstream oftest(mdatName, ios::binary);

        if(ifSignal.good())
        {
            // read data
            //
            ifSignal.seekg( (channel - nChannel) * nLength * sizeof(unsigned short), ios::end);
			//ifSignal.seekg(1024,ios::beg); //另一种方式
            ifSignal.read((char *)data, sizeof(data));
			cout<<"======= this "<<sizeof(data)<<endl;
			for (int kk = 0; kk < nChannel; kk++)
			{
		        for(int ii = 0; ii < nFrame; ii++)
		        {
		            for(int jj = 0; jj<nXpixel*nYpixel; jj++)
		            {

						oftest.write((char *)(&data[ jj * nFrame + ii + kk *nLength]), sizeof(unsigned short));
		            }
		        }
			} 
			/*ifSignal.seekg(1024,ios::beg);
            ifSignal.read((char *)data, sizeof(data));
			//cout<<"======= this "<<sizeof(data)<<endl;
		        for(int ii = 0; ii < nFrame; ii++)
		        {
		            for(int jj = 0; jj<nXpixel*nYpixel; jj++)
		            {

						oftest.write((char *)(&data[ jj * nFrame + ii]), sizeof(unsigned short));
		            }
		        }*/
		
        }

        ifSignal.close();
        oftest.close();
    }
}





