using namespace RooStats;

void getInterval( 
  const char* infile = "CountingModel.root",
  const char* workspaceName = "w",
  const char* modelConfigName = "ModelConfig",
  const char* dataName = "data" )
{ 
  // open input file, and get workspace
  TFile *file = TFile::Open(infile);
  RooWorkspace* w = dynamic_cast<RooWorkspace*>(file->Get(workspaceName));

  // get the modelConfig out of the file 
  RooStats::ModelConfig* mc = dynamic_cast<ModelConfig*>(w->obj(modelConfigName));
  
  RooAbsData* data = w->data(dataName);
  
  ProfileLikelihoodCalculator pl(*data,*mc);
  pl.SetConfidenceLevel(0.683); // 68% interval
  LikelihoodInterval* interval = pl.GetInterval();
  
  // find the iterval on the first Parameter of Interest 
  RooRealVar* firstPOI = dynamic_cast<RooRealVar*>(mc->GetParametersOfInterest()->first());
  
  double lowerLimit = interval->LowerLimit(*firstPOI);
  double upperLimit = interval->UpperLimit(*firstPOI);
  
  cout << "\n68% interval on " <<firstPOI->GetName()<<" is : ["<< lowerLimit << ", "<< upperLimit <<"] "<<endl;
  
  LikelihoodIntervalPlot * plot = new LikelihoodIntervalPlot(interval);

  TCanvas c1;
  plot->Draw("tf1");
  c1.Print("c1.pdf");
}
