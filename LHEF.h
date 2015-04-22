// -*- C++ -*-
#ifndef THEPEG_LHEF_H
#define THEPEG_LHEF_H 1
//
// Copyright (C) 2014 Carsten Bittrich
//

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>

namespace LHEF {

  class LHEFParser {
  private:
    const char* m_cFile;
    xmlNode* m_lheInit;
    xmlNode* m_lheFile;
    xmlNode* m_lheHeader;
    xmlNode* m_lheCurrentEvent;
  public:
    int m_iIDBeam1;
    int m_iIDBeam2;
    double m_dEnergyBeam1;
    double m_dEnergyBeam2;
    int m_iPDFGroupBeam1;
    int m_iPDFGroupBeam2;
    int m_iPDFSetBeam1;
    int m_iPDFSetBeam2;
    int m_iWeightingStrategy;
    int m_iNProcesses;
    double m_dCrosssection;
    double m_dCrosssectionError;
    double m_dCrosssectionMax;
    double m_dWeight;
    double m_dEventscale;      // in GeV
    double m_dQCDCoupling;
    double m_dQEDCoupling;
    int m_iNParticles;
    std::vector<double> m_vdHelicity;
    std::vector<double> m_vdStatus;
    std::vector<double> m_vdPDGID;
    std::vector<double> m_viIDColor1;
    std::vector<double> m_viIDColor2;
    std::vector<double> m_viIDMother1;
    std::vector<double> m_viIDMother2;
    std::vector<double> m_vdPx;
    std::vector<double> m_vdPy;
    std::vector<double> m_vdPz;
    std::vector<double> m_vdE;
    std::vector<double> m_vdGeneratedMass;

  public:
    LHEFParser(const char* filename, double xsection = 0):
      m_cFile(filename),
      m_dCrosssection(xsection)
      {
        init();
      };

    const double Crosssection() const {
      return m_dCrosssection;
    }

    const double CrosssectionError() const {
      return m_dCrosssectionError;
    }

    const double CrosssectionMax() const {
      return m_dCrosssectionMax;
    }

    const double Weight() const {
      return m_dWeight;
    }

    const double Eventscale() const {
      return m_dEventscale;
    }

    const double QCDCoupling() const {
      return m_dQCDCoupling;
    }

    const double QEDCoupling() const {
      return m_dQEDCoupling;
    }

    const int NParticles() const {
      return m_iNParticles;
    }

    const std::vector<double> Helicity() const {
      return m_vdHelicity;
    }

    const std::vector<double> ColorID1() const {
      return m_viIDColor1;
    }

    const std::vector<double> ColorID2() const {
      return m_viIDColor2;
    }


    const std::vector<double> MotherID1() const {
      return m_viIDMother1;
    }

    const std::vector<double> MotherID2() const {
      return m_viIDMother2;
    }

    const std::vector<double> Status() const {
      return m_vdStatus;
    }

    const std::vector<double> PDGID() const {
      return m_vdPDGID;
    }

    const std::vector<double> Px() const {
      return m_vdPx;
    }

    const std::vector<double> Py() const {
      return m_vdPy;
    }

    const std::vector<double> Pz() const {
      return m_vdPz;
    }

    const std::vector<double> E() const {
      return m_vdE;
    }

    const std::vector<double> GeneratedMass() const {
      return m_vdGeneratedMass;
    }


    void init();

    bool ReadEvent();

    void ParseInitBlock(std::string InitBlock);

    void ParseEventBlock(std::string EventBlock);

  }; // end of class LHEFParser

  class LHEFWriter {
  private: 
    std::ofstream m_strOutput;
  public:
    // init block
    int m_iIDBeam1;
    double m_dEnergyBeam1;
    int m_iPDFGroupBeam1;
    int m_iPDFSetBeam1;

    int m_iIDBeam2;
    double m_dEnergyBeam2;
    int m_iPDFGroupBeam2;
    int m_iPDFSetBeam2;
    
    int m_iWeightingStrategy;
    int m_iNProcesses;
    std::vector<double> m_vdCrosssection;       // value in pb
    std::vector<double> m_vdCrosssectionError;  // error in pb
    std::vector<double> m_vdCrosssectionMax;    // ? XMAXUP
    std::vector<int> m_viProcessID;

    // event block
    int m_iIDProcess;
    double m_dWeight;
    double m_dEventscale;      // in GeV
    double m_dQEDCoupling;
    double m_dQCDCoupling;
    int m_iNParticles;
    std::vector<int>m_viPDGID;
    std::vector<int>m_viStatus;
    std::vector<int>m_viIDMother1;
    std::vector<int>m_viIDMother2;
    std::vector<int>m_viIDColor1;
    std::vector<int>m_viIDColor2;
    std::vector<double>m_vdPx;
    std::vector<double>m_vdPy;
    std::vector<double>m_vdPz;
    std::vector<double>m_vdE;
    std::vector<double>m_vdGeneratedMass;
    std::vector<double>m_vdLifetime;
    std::vector<double>m_vdHelicity;

    LHEFWriter(const char* filename, const char* generator = 0)
      {
        m_strOutput.open(filename);
        WriteHeader(generator);
        SetDefaultInit();
        SetDefaultEvent();
      }

    ~LHEFWriter() {
      m_strOutput << "</LesHouchesEvents>" << std::endl;
      m_strOutput.close();
    }

    void WriteHeader(const char* generator = 0);
    void SetDefaultInit();
    void SetDefaultEvent();
    void WriteInitBlock();
    void WriteEventBlock();

  }; // end of class LHEFWriter
} // end of namespace LHEF

#endif