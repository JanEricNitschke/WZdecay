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
  WZdecay::CLHEFWriter output("output.lhef", reader);
  std::vector<WZdecay::CLHEFWriter *> vecOutput {&output};

  // specify allowed decays
  WZdecay::CDecayW decWtoENu(1,0,0,0);
  WZdecay::CDecayZ decZtoENu(0,1,0,0);


  WZdecay::CRandom* random = new WZdecay::CRandom(0);



  // loop over event
  WZdecay::CEvent* pEvent = 0;
  const int iMaxNumberEvents = 20;
  int iCounter = 0;
  while (iCounter < iMaxNumberEvents && reader->ReadEvent(pEvent)) {
    iCounter ++;
    log.toLog("In new event.", 1);
    decWtoENu.DecayAllPDGID(pEvent, random);
    decZtoENu.DecayAllPDGID(pEvent, random);
    if (iCounter % 100 == 0) {
        PrintProgressbar(iCounter, fmin(2000000., iMaxNumberEvents), 60);
    }
    log.toLog("Sort events.", 1);
    output.WriteEvent(pEvent);
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

  log.toLog("Crosssection before allowed decays: " + std::to_string(reader->CrossSection()));
  // CalculateCrossSection(reader, vecOutput, &decWtoENu, &decZtoENu);
  
  log.toLog("Crosssection after allowed decays: " + std::to_string(reader->CrossSection()));
  log.toLog("Sum of Input Weights: " + std::to_string(reader->SumOfWeights()));
  // log.toLog("WLZL: Sum of Weights: " + std::to_string(output.SumOfWeights()) + " CrossSection: " + std::to_string(output.CrossSection()) + "fb");
  log.toLog("Done.");
  delete random;

  // log.DumpToFile("WZdecay.log");
  return 0;
}
