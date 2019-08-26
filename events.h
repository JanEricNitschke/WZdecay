#ifndef __WZdecay_EVENT_H__
#define __WZdecay_EVENT_H__ 1

// standard include
#include <iostream>
#include <cmath>
#include <vector>
#include "lorentzvector.h"
#include "particle.h"
#include "vertex.h"
#include "logger.h"
#include <libxml/parser.h>
#include <libxml/tree.h>


namespace WZdecay
{
  /**
   * @brief Class for storing information about single event
   */
  class CEvent
  {
  public:

    typedef std::vector<CParticle>::iterator particle_iterator;
    typedef std::vector<CParticle>::const_iterator particle_const_iterator;

    typedef std::vector<CVertex>::iterator vertex_iterator;
    typedef std::vector<CVertex>::const_iterator vertex_const_iterator;

  private:
    /**
     * Vector storing particles of event
     */
    std::vector<CParticle> m_vecParticles;

    /**
     * Vector storing vertices of event
     */
    std::vector<CVertex> m_vecVertices;

    /**
     * Bjorken scale of quark of first parton
     */
    double m_XParticleA;

    /**
     * Bjorken scale of quark of second parton
     */
    double m_XParticleB;

    /**
     * energy scale
     */
    double m_dScale;

    /**
     * Event weight
     */
    double m_dWeight;

    /**
     * @brief QCD coupling
     */
    double m_dAlphaStrong;

    /**
     * @brief QED coupling
     */
    double m_dAlphaWeak;

    unsigned int m_uiEventID;
    static unsigned int sm_uiCntEvent;

    // CMatrixElement* pm_MatrixElement;

    /**
      Event Weights
     */

    xmlNode* m_EvtMgwt; //@patmasid

  public:
    // constructors and destructors

    // constructor of initial + n outgoing particles
    explicit CEvent(unsigned int uiNumberParticles = 1u):
      m_vecParticles(uiNumberParticles),
      m_vecVertices(0),
      m_XParticleA(0),
      m_XParticleB(0),
      m_dScale(0),
      m_dWeight(0),
      m_dAlphaStrong(0),
      m_dAlphaWeak(0) {
        sm_uiCntEvent++;
        m_uiEventID = sm_uiCntEvent;
      }

    // copy constructor
    CEvent(const CEvent &CpEvent):
      m_vecParticles(CpEvent.VecParticles()),
      m_vecVertices(0),
      m_XParticleA(CpEvent.XParticleA()),
      m_XParticleB(CpEvent.XParticleB()){
        sm_uiCntEvent++;
        m_uiEventID = sm_uiCntEvent;
      }

    // Getter and Setter

    const unsigned int EventID() const
    {
      return m_uiEventID;
    }

    const unsigned int CntEvent() const
    {
      return sm_uiCntEvent;
    }

    const double Weight() const
    {
      return m_dWeight;
    }

    const double Scale() const
    {
      return m_dScale;
    }

    const double AlphaStrong() const
    {
      return m_dAlphaStrong;
    }

    const double AlphaWeak() const
    {
      return m_dAlphaWeak;
    }

    const std::vector<CParticle> VecParticles() const
    {
      return m_vecParticles;
    }

    const std::vector<CVertex> VecVertices() const
    {
      return m_vecVertices;
    }

    const double XParticleA() const
    {
      return m_XParticleA;
    }

    const double XParticleB() const
    {
      return m_XParticleB;
    }

    const CParticle & Particle(int iIndex) const
    {
      return m_vecParticles[iIndex];
    }

    CParticle & SetParticle(int iIndex)
    {
      return m_vecParticles[iIndex];
    }
    void SetXParticleA(double dNewXParticleA)
    {
      m_XParticleA = dNewXParticleA;
    }

    void SetXParticleB(double dNewXParticleB)
    {
      m_XParticleB = dNewXParticleB;
    }

    double SetXParticleA()
    {
      return m_XParticleA;
    }

    double SetXParticleB()
    {
      return m_XParticleB;
    }

    CEvent* SetWeight(double newWeight)
    {
      m_dWeight = newWeight;
      return this;
    }

    CEvent* SetScale(double newScale)
    {
      m_dScale = newScale;
      return this;
    }

    CEvent* SetAlphaStrong(double newAlpha)
    {
      m_dAlphaStrong = newAlpha;
      return this;
    }

    CEvent* SetAlphaWeak(double newAlpha)
    {
      m_dAlphaWeak = newAlpha;
      return this;
    }

    CEvent* SetEventID(unsigned int newID)
    {
      m_uiEventID = newID;
      return this;
    }

    // iterators

    particle_iterator particles_begin()
    {
      return m_vecParticles.begin();
    };
    const particle_const_iterator particles_cbegin() const
    {
      return m_vecParticles.cbegin();
    };

    particle_iterator particles_end()
    {
      return m_vecParticles.end();
    };

    const particle_const_iterator particles_cend() const
    {
      return m_vecParticles.cend();
    };

    vertex_iterator vertices_begin()
    {
      return m_vecVertices.begin();
    };
    const vertex_const_iterator vertices_cbegin() const
    {
      return m_vecVertices.cbegin();
    };

    vertex_iterator vertices_end()
    {
      return m_vecVertices.end();
    };

    const vertex_const_iterator vertices_cend() const
    {
      return m_vecVertices.cend();
    };

    // other methods

    void ResizeParticles(unsigned int uiNewSize)
    {
      m_vecParticles.resize(uiNewSize + 2);
    }

    void ResizeVertices(unsigned int uiNewSize)
    {
      m_vecVertices.resize(uiNewSize + 2);
    }

    int ResizeParticles()
    {
      return m_vecParticles.size();
    }

    int ResizeVertices()
    {
      return m_vecVertices.size();
    }

    void AddParticle(CParticle& newParticle)
    {
      logger::CLogger log("CEvent::AddParticle()");
      m_vecParticles.push_back(newParticle);
    }

    void AddVertex(CVertex & newVertex)
    {
      m_vecVertices.push_back(newVertex);
    }

    CParticle & operator [] (int iIndex)
    {
      return m_vecParticles[static_cast<int>(iIndex + 1)];
    }

    const CParticle& operator [] (int iIndex) const 
    {
      return m_vecParticles[static_cast<int>(iIndex + 1)];
    }

    xmlNode* ReturnMgwt()
    {
      return m_EvtMgwt;
    }

    void SetMgwt(xmlNode* EvtMgwt)
    {
      m_EvtMgwt = EvtMgwt;
    }

  }; // end of class CEvent

  /**
   * @brief Streaming operator for CParticle
   */
  std::ostream & operator << (std::ostream & stream, const CEvent & event);


}; // end of namespace WZdecay

#endif
