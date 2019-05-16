#include <iostream>
#include <vector>
#include <string>
#include "events.h"
#include "hepmcreader269.h"
#include "hepmcwriter.h"
#include "lhefreader.h"
#include "lhefwriter.h"
#include "readerbase.h"
#include "sorterhelicity.h"
#include "decayW.h"
#include "decayZ.h"
#include "random.h"
#include "logger.h"


void CalculateCrossSection(WZdecay::ReaderBase* input, std::vector<WZdecay::CHepMCWriter*> vecOutput, WZdecay::CDecayW* decayW, WZdecay::CDecayZ* decayZ) {
  logger::CLogger log("CalculateCrossSection()");
  log.toLog("Input Crosssection: " + std::to_string(input->CrossSection()));
  decayW->ReduceCrossSection(input);
  decayZ->ReduceCrossSection(input);
  double CrossSectionPerWeight = input->CrossSection()/input->SumOfWeights();
  for (auto iter : vecOutput) {
    iter->SetCrossSection(CrossSectionPerWeight*iter->SumOfWeights());
    iter->WriteCrossSection();
  }
}

void CalculateCrossSectionlhef(WZdecay::ReaderBase* input, std::vector<WZdecay::CLHEFWriter*> vecOutput, WZdecay::CDecayW* decayW, WZdecay::CDecayZ* decayZ) {
  logger::CLogger log("CalculateCrossSection()");
  log.toLog("Input Crosssection: " + std::to_string(input->CrossSection()));
  decayW->ReduceCrossSection(input);
  decayZ->ReduceCrossSection(input);
  double CrossSectionPerWeight = input->CrossSection()/input->SumOfWeights();
  for (auto iter : vecOutput) {
    iter->SetCrossSection(CrossSectionPerWeight*iter->SumOfWeights());
    //iter->WriteCrossSection();
  }
}

void PrintProgressbar(int current, int total, int iWidth) {
  logger::CLogger log("PrintProgressbar()");
  if (current % 100 == 0) {
    std::cout << "\r[";
    for (int i = 0; i < current*iWidth/total; i ++) {
      std::cout << "-";
    }
    for (int i = 0; i < iWidth - current*iWidth/total; i ++) {
      std::cout << " ";
    }
    std::cout << "] "<< current*100/total << "%     ";
    switch ((current / 500) % 4) {
      case 0 : std::cout << "|"; break;
      case 1 : std::cout << "/"; break;
      case 2 : std::cout << "-"; break;
      case 3 : std::cout << "\\"; break;
    }
    std::cout.flush();
  }
}

void EndProgramm() {
  logger::CLogger* log = new logger::CLogger("EndProgramm()");
  log->toLog("Wrong number of arguments",4);
  std::cerr << "Usage: ./WZdecay [-l LOGLEVEL -c CROSSSECTION] INPUTFILE1 [INPUTFILE2 [...]]" << std::endl;
  std::cerr << "Where the INPUTFILES are either .lhe(f) or .hepmc files. Currently all have to be of the same type and connection of multiple files is only implemented for hepmc-files (Other files are ignored for lhef-files)." << std::endl;
  // log->DumpToFile("WZdecay.log");
  delete log;
  exit(EXIT_FAILURE);
}

int main (int argc, char **argv)
{

  logger::CLogger log("main function");

  // parse command line arguments
  double dXsection = 0;
  if (argc == 1) {
    EndProgramm();
  }

  int indCrosssection = 1;
  int indFirstFile = 1;

  // check if log level is provided
  if (static_cast<std::string>(argv[1]) == "-l") {
    if (argc < 3) {
      EndProgramm();
    }

    std::string sLevel = argv[2];
    unsigned int iLevel = std::stoi(sLevel);
    if (iLevel <= 4u) {
      logger::CLogger::SetLevel(iLevel);
    }
    else {
      log.toLog("unrecognized argument: " + sLevel + ": Skipping.", 3);
    }

    indFirstFile    += 2;
    indCrosssection += 2;
  }

  // check if crosssection is provided
  if (static_cast<std::string>(argv[indCrosssection]) == "-c") {
    if (argc < indCrosssection + 2) {
      EndProgramm();
    }
    std::string xsect = argv[indCrosssection + 1];
    dXsection = std::stod(xsect);
    indFirstFile = indCrosssection + 2;
  }

  // fill vector of filenames
  std::vector<const char*> vFilenames;
  for (int index = indFirstFile; index < argc; index ++) {
    vFilenames.push_back(argv[index]);
  }
  
  // test which filetype is given
  std::string FirstFile = vFilenames[0];
  WZdecay::ReaderBase* reader = 0;
  if (FirstFile.find(".hepmc") != std::string::npos /* ".hepmc" is found in filename */) {
    reader = new WZdecay::CHepMCReader269(vFilenames, dXsection);
  }
  else if (FirstFile.find(".lhe") != std::string::npos /* ".lhe" is found in filename */) {
    //if (dXsection == 0) {
    //  reader = new WZdecay::LHEFReader(vFilenames);
    //}
    //else {
      reader = new WZdecay::LHEFReader(vFilenames, dXsection);
    //}
    
  }
  else {
    EndProgramm();
  }

  // contruction of objects for sorting and output
  std::vector<int> vecHelLongitudinal {0};
  std::vector<int> vecHelTransversal {-1,1};
  //WZdecay::CHepMCWriter output("output.hepmc");//, reader);
  //std::vector<WZdecay::CHepMCWriter *> vecOutput {&output};
  //-------------------------
  //WZdecay::CLHEFWriter output("output.lhef", reader);
  //std::vector<WZdecay::CLHEFWriter *> vecOutput {&output};
  //-------------------------
  //WZdecay::CHepMCWriter outputWLZL("outputWLZL.hepmc");
  WZdecay::CLHEFWriter outputWLZL("outputWLZL.lhef", reader);
  WZdecay::CSorterHelicity sorterWLZL(vecHelLongitudinal, vecHelLongitudinal);
  //WZdecay::CHepMCWriter outputWLZT("outputWLZT.hepmc");
  WZdecay::CLHEFWriter outputWLZT("outputWLZT.lhef", reader);
  WZdecay::CSorterHelicity sorterWLZT(vecHelLongitudinal, vecHelTransversal);
  //WZdecay::CHepMCWriter outputWTZL("outputWTZL.hepmc");
  WZdecay::CLHEFWriter outputWTZL("outputWTZL.lhef", reader);
  WZdecay::CSorterHelicity sorterWTZL(vecHelTransversal, vecHelLongitudinal);
  //WZdecay::CHepMCWriter outputWTZT("outputWTZT.hepmc");
  WZdecay::CLHEFWriter outputWTZT("outputWTZT.lhef", reader);
  WZdecay::CSorterHelicity sorterWTZT(vecHelTransversal, vecHelTransversal);
  std::vector<WZdecay::CLHEFWriter *> vecOutput {&outputWLZL, &outputWLZT, &outputWTZL, &outputWTZT};




  // specify allowed decays
  WZdecay::CDecayW decWtoENu(1,1,0,0);
  WZdecay::CDecayZ decZtoENu(1,1,0,0);


  WZdecay::CRandom* random = new WZdecay::CRandom(0);



  // loop over event
  WZdecay::CEvent* pEvent = 0;
  const int iMaxNumberEvents = 1E8;
  int iCounter = 0;
  int LL = 1;
  int LT = 1;
  int TL = 1;
  int TT = 1;
  while (iCounter < iMaxNumberEvents && reader->ReadEvent(pEvent)) {
    iCounter ++;
    log.toLog("In new event.", 1);
    decWtoENu.DecayAllPDGID(pEvent, random);
    decZtoENu.DecayAllPDGID(pEvent, random);
    if (iCounter % 100 == 0) {
        PrintProgressbar(iCounter, fmin(2000000., iMaxNumberEvents), 60);
    }
    //log.toLog("Sort events.", 1);
    //output.WriteEvent(pEvent);
    log.toLog("Sort events.", 1);
    if (sorterWLZL.IsAccepted(pEvent)) {
      log.toLog("Found WLZL event.", 1);
      outputWLZL.WriteEvent(pEvent, LL);
      LL = 0;
    }
    if (sorterWLZT.IsAccepted(pEvent)) {
      log.toLog("Found WLZT event.", 1);
      outputWLZT.WriteEvent(pEvent, LT);
      LT = 0;
    }
    if (sorterWTZL.IsAccepted(pEvent)) {
      log.toLog("Found WTZL event.", 1);
      outputWTZL.WriteEvent(pEvent, TL);
      TL = 0;
    }
    if (sorterWTZT.IsAccepted(pEvent)) {
      log.toLog("Found WTZT event.", 1);
      outputWTZT.WriteEvent(pEvent, TT);
      TT = 0;
    }
    delete pEvent;
  }

  PrintProgressbar(1, 1, 60);
  std::cout << std::endl;
  log.toLog("Done analyzing events.");
  if (iCounter >= iMaxNumberEvents) {
    log.toLog("Reached maximum number of events. Did " + std::to_string(iCounter) + "Events.", 2);
  }
  if (reader->ReadEvent(pEvent) == 0) {
    log.toLog("No next Event.", 2);
  }

  //log.toLog("Crosssection before allowed decays: " + std::to_string(reader->CrossSection()));
  // CalculateCrossSection(reader, vecOutput, &decWtoENu, &decZtoENu);
  
  //log.toLog("Crosssection after allowed decays: " + std::to_string(reader->CrossSection()));
  //log.toLog("Sum of Input Weights: " + std::to_string(reader->SumOfWeights()));
  // log.toLog("WLZL: Sum of Weights: " + std::to_string(output.SumOfWeights()) + " CrossSection: " + std::to_string(output.CrossSection()) + "fb");
  
  log.toLog("Crosssection before allowed decays: " + std::to_string(reader->CrossSection()));
  CalculateCrossSectionlhef(reader, vecOutput, &decWtoENu, &decZtoENu);
  
  log.toLog("Crosssection after allowed decays: " + std::to_string(reader->CrossSection()));
  log.toLog("Sum of Input Weights: " + std::to_string(reader->SumOfWeights()));
  log.toLog("WLZL: Sum of Weights: " + std::to_string(outputWLZL.SumOfWeights()) + " CrossSection: " + std::to_string(outputWLZL.CrossSection()) + "fb");
  log.toLog("WLZT: Sum of Weights: " + std::to_string(outputWLZT.SumOfWeights()) + " CrossSection: " + std::to_string(outputWLZT.CrossSection()) + "fb");
  log.toLog("WTZL: Sum of Weights: " + std::to_string(outputWTZL.SumOfWeights()) + " CrossSection: " + std::to_string(outputWTZL.CrossSection()) + "fb");
  log.toLog("WTZT: Sum of Weights: " + std::to_string(outputWTZT.SumOfWeights()) + " CrossSection: " + std::to_string(outputWTZT.CrossSection()) + "fb");


  log.toLog("Done.");
  delete random;

  // log.DumpToFile("WZdecay.log");
  return 0;
}
