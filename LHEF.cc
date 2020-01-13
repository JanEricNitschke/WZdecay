//
// Copyright (C) 2014 Carsten Bittrich
//

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "LHEF.h"

namespace LHEF {

    void LHEFParser::CreateNodeAttrNamePairs_r(){ // @patmasid

      std::vector<const char*> vec_weightgroup{"name","combine"};//, vec_weight, vec_pdfrwt, vec_wgt;
      std::vector<const char*> vec_weight{"id","MUR","MUF","DYN_SCALE","PDF"};
      std::vector<const char*> vec_pdfrwt{"beam"};
      std::vector<const char*> vec_wgt{"id"};
      
      std::pair <const char*, std::vector<const char*> > pair_weightgroup = std::make_pair("weightgroup",vec_weightgroup);
      std::pair <const char*, std::vector<const char*> > pair_weight = std::make_pair("weight",vec_weight);
      std::pair <const char*, std::vector<const char*> > pair_pdfrwt = std::make_pair("pdfrwt",vec_pdfrwt);
      std::pair <const char*, std::vector<const char*> > pair_wgt = std::make_pair("wgt",vec_wgt);
      
      m_vNodeAttrNamePairs.push_back(pair_weightgroup);
      m_vNodeAttrNamePairs.push_back(pair_weight);
      m_vNodeAttrNamePairs.push_back(pair_pdfrwt);
      m_vNodeAttrNamePairs.push_back(pair_wgt);
    }
  
    void LHEFParser::print_xmlNodes_r(xmlNode * a_node) // @patmasid 
    {

      xmlNode *cur_node = NULL;

      for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {

          bool nodeFoundInPairs = false;
          //std::cout<<m_vNodeAttrNamePairs.size()<<std::endl;                                                                                                                                            
          for(uint iPair=0; iPair<m_vNodeAttrNamePairs.size(); iPair++){
            //std::cout<<(const xmlChar *)m_vNodeAttrNamePairs.at(iPair).first<<std::endl;                                                                                                                
            if ((!xmlStrcmp(cur_node->name, (const xmlChar *)m_vNodeAttrNamePairs.at(iPair).first))) {
              nodeFoundInPairs= true;
	      std::cout<<"<"<<cur_node->name<<" ";
              for(uint iAttr=0; iAttr<m_vNodeAttrNamePairs.at(iPair).second.size(); iAttr++){
                if( (xmlHasProp(cur_node,(const xmlChar *)m_vNodeAttrNamePairs.at(iPair).second.at(iAttr))) == NULL) continue;
                xmlChar *Attr = xmlGetProp(cur_node, (const xmlChar *)m_vNodeAttrNamePairs.at(iPair).second.at(iAttr));
		std::cout<<m_vNodeAttrNamePairs.at(iPair).second.at(iAttr)<<'='<<'"'<<Attr<<'"'<<' ';
              }
	      std::cout<<"> ";
              break;
            }
            else continue;
          }

          if(nodeFoundInPairs==false) std::cout<<"<"<<cur_node->name<<"> ";

	  std::cout<<cur_node->children->content;

          if(xmlStrcmp(cur_node->name,(const xmlChar *)"MG5ProcCard")==0 || xmlStrcmp(cur_node->name,(const xmlChar *)"MGRunCard")==0){
            if(cur_node->children->next != NULL){
	      std::cout<<cur_node->children->next->content;
            }
          }

          print_xmlNodes_r(cur_node->children);

	  std::cout<<"</"<<cur_node->name<<">"<<std::endl;
        }
      }
    }
  
    void LHEFParser::init() {
      xmlDoc* doc = 0;
      doc = xmlParseFile(m_cFile);

      m_lheFile         = (xmlDocGetRootElement(doc));
      m_lheHeader       = (m_lheFile->children->next);
      m_lheInit         = (m_lheHeader->next->next);
      m_lheCurrentEvent = (m_lheInit->next->next);

      // way to cast from unsigned char* to std::string ist unsafe, but only known possibility
      std::string lheInitBlock(reinterpret_cast<char*>(m_lheInit->children->content));

      // erase first linebreak, content starts afterwards
      ParseInitBlock(lheInitBlock.erase(0,1));

      //Test for printing header // @patmasid 
      //print_xmlNodes(m_lheHeader->children); 
      
    }

    bool LHEFParser::ReadEvent() {
      if (m_lheCurrentEvent == NULL || m_lheCurrentEvent->next == NULL) {
        return 0;
      }

      m_lheCurrentEventMgwt = (m_lheCurrentEvent->children->next); // @patmasid 

      // way to cast from unsigned char* to std::string ist unsafe, but only known possibility
      std::string lheEventBlock(reinterpret_cast<char*>(m_lheCurrentEvent->children->content));

      ParseEventBlock(lheEventBlock.erase(0,1));
      m_lheCurrentEvent = m_lheCurrentEvent->next->next;
      return 1;
    }

    void LHEFParser::ParseInitBlock(std::string InitBlock) {
      std::stringstream ssInitBlock(InitBlock);
      std::string Line0; 
      getline(ssInitBlock, Line0);
      std::vector<double> vdLine0;
      std::istringstream issLine0(Line0);
      while (issLine0)
      {
          std::string sub;
          issLine0 >> sub;
          if (issLine0) {
            vdLine0.push_back(std::stod(sub));
          }
      }
  
      if (vdLine0[vdLine0.size() - 1] != 1) {
        throw std::runtime_error("NPRUP != 1 not implemented yet"); // Different processes should each get their own lhef/hepmc file.
      }

      std::string Line1; 
      getline(ssInitBlock, Line1);
      std::vector<double> vdLine1;
      std::istringstream issLine1(Line1);
      while (issLine1)
      {
          std::string sub;
          issLine1 >> sub;
          if (issLine1) {
            vdLine1.push_back(std::stod(sub));
          }
      }

      // fill only if filled with default value
      if (m_dCrosssection == 0) { 
        m_dCrosssection = vdLine1[0];
      }
      m_dCrosssectionError = vdLine1[1];
      m_dCrosssectionMax   = vdLine1[2];
    }

    void LHEFParser::ParseEventBlock(std::string EventBlock) {
      // clear vectors
      m_vdHelicity.clear();
      m_vdStatus.clear();
      m_vdPDGID.clear();
      m_vdPx.clear();
      m_vdPy.clear();
      m_vdPz.clear();
      m_vdE.clear();
      m_vdGeneratedMass.clear();
      m_viIDColor1.clear();
      m_viIDColor2.clear();
      m_viIDMother1.clear();
      m_viIDMother2.clear();

      std::stringstream ssEventBlock(EventBlock);
      std::string Line0; 
      getline(ssEventBlock, Line0);
      std::vector<double> vdLine0;
      std::istringstream issLine0(Line0);
      while (issLine0)
      {
          std::string sub;
          issLine0 >> sub;
          if (issLine0) {
            vdLine0.push_back(std::stod(sub));
          }
      }

      // fill values
      m_dWeight      = vdLine0[2];
      m_dEventscale  = vdLine0[3];
      m_dQEDCoupling = vdLine0[4];
      m_dQCDCoupling = vdLine0[5];

      m_iNParticles = vdLine0[0];
      for (int i = 0; i < vdLine0[0]; i++) {
        std::string Particle; 
        getline(ssEventBlock, Particle);
        std::vector<double> vdParticle;
        std::istringstream issParticle(Particle);
        while (issParticle)
        {
            std::string sub;
            issParticle >> sub;
            if (issParticle) {
              vdParticle.push_back(std::stod(sub));
            }
        }
        vdParticle.pop_back();

        // fill values in vector
        m_vdPDGID.push_back(vdParticle[0]);
        m_vdStatus.push_back(vdParticle[1]);
        m_viIDMother1.push_back(vdParticle[2]);
        m_viIDMother2.push_back(vdParticle[3]);
        m_viIDColor1.push_back(vdParticle[4]);
        m_viIDColor2.push_back(vdParticle[5]);
        m_vdPx.push_back(vdParticle[6]);
        m_vdPy.push_back(vdParticle[7]);
        m_vdPz.push_back(vdParticle[8]);
        m_vdE.push_back(vdParticle[9]);
        m_vdGeneratedMass.push_back(vdParticle[10]);
        m_vdHelicity.push_back(vdParticle[12]);
      }
    } // end of method ParseEventBlock

    void LHEFWriter::CreateNodeAttrNamePairs_w(){ // @patmasid 
      
      std::vector<const char*> vec_weightgroup{"name","combine"};//, vec_weight, vec_pdfrwt, vec_wgt;                                                                                                        
      std::vector<const char*> vec_weight{"id","MUR","MUF","DYN_SCALE","PDF"};
      std::vector<const char*> vec_pdfrwt{"beam"};
      std::vector<const char*> vec_wgt{"id"};
      
      std::pair <const char*, std::vector<const char*> > pair_weightgroup = std::make_pair("weightgroup",vec_weightgroup);
      std::pair <const char*, std::vector<const char*> > pair_weight = std::make_pair("weight",vec_weight);
      std::pair <const char*, std::vector<const char*> > pair_pdfrwt = std::make_pair("pdfrwt",vec_pdfrwt);
      std::pair <const char*, std::vector<const char*> > pair_wgt = std::make_pair("wgt",vec_wgt);
      
      m_vNodeAttrNamePairs.push_back(pair_weightgroup);
      m_vNodeAttrNamePairs.push_back(pair_weight);
      m_vNodeAttrNamePairs.push_back(pair_pdfrwt);
      m_vNodeAttrNamePairs.push_back(pair_wgt);
    }
  
    void LHEFWriter::print_xmlNodes_w(xmlNode * a_node) // @patmasid 
    {
      xmlNode *cur_node = NULL;

      for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {

          bool nodeFoundInPairs = false;
          //std::cout<<m_vNodeAttrNamePairs.size()<<std::endl;                                                                                                                                            
          for(uint iPair=0; iPair<m_vNodeAttrNamePairs.size(); iPair++){
            //std::cout<<(const xmlChar *)m_vNodeAttrNamePairs.at(iPair).first<<std::endl;                                                                                                                
            if ((!xmlStrcmp(cur_node->name, (const xmlChar *)m_vNodeAttrNamePairs.at(iPair).first))) {
              nodeFoundInPairs= true;
              m_strOutput<<"<"<<cur_node->name<<" ";
              for(uint iAttr=0; iAttr<m_vNodeAttrNamePairs.at(iPair).second.size(); iAttr++){
                if( (xmlHasProp(cur_node,(const xmlChar *)m_vNodeAttrNamePairs.at(iPair).second.at(iAttr))) == NULL) continue;
                xmlChar *Attr = xmlGetProp(cur_node, (const xmlChar *)m_vNodeAttrNamePairs.at(iPair).second.at(iAttr));
                m_strOutput<<m_vNodeAttrNamePairs.at(iPair).second.at(iAttr)<<'='<<'"'<<Attr<<'"'<<' ';
              }
              m_strOutput<<"> ";
              break;
            }
            else continue;
          }

          if(nodeFoundInPairs==false) m_strOutput<<"<"<<cur_node->name<<">";
          m_strOutput<<cur_node->children->content;

          if(xmlStrcmp(cur_node->name,(const xmlChar *)"MG5ProcCard")==0 || xmlStrcmp(cur_node->name,(const xmlChar *)"MGRunCard")==0){
            if(cur_node->children->next != NULL){
              m_strOutput<<cur_node->children->next->content;
            }
          }

          print_xmlNodes_w(cur_node->children);

          m_strOutput<<"</"<<cur_node->name<<">"<<std::endl;
        }
      }
    }

    void LHEFWriter::WriteHeader(xmlNode* header, const char* generator) { // @patmasid 
        m_strOutput << "<LesHouchesEvents version=\"1.0\">" << std::endl;
        m_strOutput << "<header>" << std::endl;
        if (generator != 0) {
          m_strOutput << "  <generator_name>" << generator << "</generator_name>" << std::endl;
        }
	print_xmlNodes_w(header->children); // @patmasid 
        m_strOutput << "</header>" << std::endl;
    }

    void LHEFWriter::SetDefaultInit() {
      // init
      m_iIDBeam1 = 2212;        // proton beam
      m_dEnergyBeam1 = 0;
      m_iPDFGroupBeam1 = 0;
      m_iPDFSetBeam1 = 0;

      m_iIDBeam2 = 2212;        // proton beam
      m_dEnergyBeam2 = 0;
      m_iPDFGroupBeam2 = 0;
      m_iPDFSetBeam2 = 0;
      
      m_iWeightingStrategy = 3;
      m_iNProcesses = 0;
      m_vdCrosssection =      std::vector<double>({});
      m_vdCrosssectionError = std::vector<double>({});
      m_vdCrosssectionMax =   std::vector<double>({});
    }

    void LHEFWriter::SetDefaultEvent() {
      m_iNParticles = 0;
      m_iIDProcess = 0;
      m_dWeight = 0;
      m_dEventscale = 0;
      m_dQEDCoupling = 0;
      m_dQCDCoupling = 0;
      m_iNParticles = 0;
      m_viPDGID = std::vector<int>({});
      m_viStatus = std::vector<int>({});
      m_viIDMother1 = std::vector<int>({});
      m_viIDMother2 = std::vector<int>({});
      m_viIDColor1 = std::vector<int>({});
      m_viIDColor2 = std::vector<int>({});
      m_vdPx = std::vector<double>({});
      m_vdPy = std::vector<double>({});
      m_vdPz = std::vector<double>({});
      m_vdE  = std::vector<double>({});
      m_vdGeneratedMass = std::vector<double>({});
      m_vdLifetime = std::vector<double>({});
      m_vdHelicity = std::vector<double>({});
    }

    void LHEFWriter::WriteInitBlock() {
      m_strOutput << "<init>" << std::endl;
      m_strOutput << " " << m_iIDBeam1;
      m_strOutput << "\t" << m_iIDBeam2;
      m_strOutput << "\t" << m_dEnergyBeam1;
      m_strOutput << "\t" << m_dEnergyBeam2;
      m_strOutput << "\t" << m_iPDFGroupBeam1; 
      m_strOutput << "\t" << m_iPDFGroupBeam2;
      m_strOutput << "\t" << m_iPDFSetBeam1;
      m_strOutput << "\t" << m_iPDFSetBeam2;
      m_strOutput << "\t" << m_iWeightingStrategy;
      m_strOutput << "\t" << m_iNProcesses;
      m_strOutput << std::endl;
      for (int i = 0; i < m_iNProcesses; i++) {
      m_strOutput << "  " << m_vdCrosssection[i];
      m_strOutput << "\t"  << m_vdCrosssectionError[i];
      m_strOutput << "\t"  << m_vdCrosssectionMax[i];
      m_strOutput << "\t"  << i;
      m_strOutput << std::endl;
      }
      m_strOutput << "</init>" << std::endl;
    }

    void LHEFWriter::WriteEventBlock(xmlNode* EventMgwt) { // @patmasid 
      m_strOutput << "<event>" << std::endl;
      m_strOutput << " "  << m_iNParticles;
      m_strOutput << "\t" << m_iIDProcess;
      m_strOutput << "\t" << m_dWeight;
      m_strOutput << "\t" << m_dEventscale;
      m_strOutput << "\t" << m_dQEDCoupling;
      m_strOutput << "\t" << m_dQCDCoupling;
      m_strOutput << std::endl;
      for (int i = 0; i < m_iNParticles; i++) {
      m_strOutput << "   " << m_viPDGID[i];
      m_strOutput << "\t"  << m_viStatus[i];
      m_strOutput << "\t"  << m_viIDMother1[i];
      m_strOutput << "\t"  << m_viIDMother2[i];
      m_strOutput << "\t"  << m_viIDColor1[i];
      m_strOutput << "\t"  << m_viIDColor2[i];
      m_strOutput << "\t"  << m_vdPx[i];
      m_strOutput << "\t"  << m_vdPy[i];
      m_strOutput << "\t"  << m_vdPz[i];
      m_strOutput << "\t"  << m_vdE[i];
      m_strOutput << "\t"  << m_vdGeneratedMass[i];
      m_strOutput << "\t"  << m_vdLifetime[i];
      m_strOutput << "\t"  << m_vdHelicity[i];
      m_strOutput << std::endl;
      }
      print_xmlNodes_w(EventMgwt); 
      m_strOutput << "</event>" << std::endl;
      SetDefaultEvent();
    }

} // end of namespace LHEF
