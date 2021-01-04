using namespace RooStats;

void roostat()
{
  RooWorkspace w("w");

  // make Poisson model * Gaussian constraint 
  w.factory("sum:nexp(s[3,0,15],b[1,0,10])");

  // Poisson of (n | s+b) 
  w.factory("Poisson:pdf(nobs[0,50],nexp)");
  w.factory("Gaussian:constraint(b0[0,10],b,sigmab[1])");
  w.factory("PROD:model(pdf,constraint)");

  w.var("b0")->setVal(1);
  w.var("b0")->setConstant(true);
  w.var("sigmab")->setVal(0.2*1);

  ModelConfig mc("ModelConfig",&w);
  mc.SetPdf(*w.pdf("model"));
  mc.SetParametersOfInterest(*w.var("s"));
  w.var("nobs")->setVal(3);
  mc.SetObservables(*w.var("nobs"));
  mc.SetNuisanceParameters(*w.var("b"));

  //these are needed for the hypothesis tests 
  mc.SetSnapshot(*w.var("s")); 
  mc.SetGlobalObservables(*w.var("b0"));

  mc.Print();

  // import model in the workspace 
  w.import(mc);

  // make data set with the namber of observed events 
  RooDataSet data("data","", *w.var("nobs"));
  w.var("nobs")->setVal(3);
  data.add(*w.var("nobs"));

  // import data set in workspace and save it in a file 
  w.import(data);
  w.Print();

  TString fileName = "CountingModel.root";
  w.writeToFile(fileName, true);
}
