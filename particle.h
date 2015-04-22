#ifndef __WZdecay_PARTICLE_H__
#define __WZdecay_PARTICLE_H__ 1

// standard include
#include <iostream>
#include <cmath>
#include "lorentzvector.h"


namespace WZdecay {
  /**
   * @brief Flavour type, containing PDG-IDs
   */
  typedef int FlavorType;

  /**
   * @brief Class to store information of particles
   */
  class CParticle 
  {
  private:
    /**
     * Flavor of particle
     */
    FlavorType m_Flavor;

    /**
     * Four-Momentum of particle 
     */
    CLorentzVector m_Momentum;

    /**
     * helicity of particle
     */
    int m_iHelicity;

    /**
     * generated Mass, following hepmc convention
     */
    double m_dGeneratedMass;

    /**
     * Status of particle
     */
    int m_iStatus;

    int m_iColorID1;
    int m_iColorID2;
    int m_iMotherID1;
    int m_iMotherID2;
    int m_iParticleID;


  public:
    // constructors and destructors
    CParticle():m_Flavor(0),m_iStatus(2),m_Momentum(), m_iColorID1(0), m_iColorID2(0), m_iMotherID1(0), m_iMotherID2(0), m_iParticleID(0) {}

    CParticle(FlavorType flaFlavor, int iStatus, CLorentzVector lvMomentum, int iHelicity):m_Flavor(flaFlavor), m_iStatus(iStatus), m_Momentum(lvMomentum), m_iHelicity(iHelicity), m_iColorID1(0), m_iColorID2(0), m_iMotherID1(0), m_iMotherID2(0), m_iParticleID(0) {}

    ~CParticle() = default;

    // Getter and Setter

    const FlavorType& Flavor() const
    {
      return m_Flavor;
    }

    const CLorentzVector& Momentum() const
    {
      return m_Momentum;
    }

    const int & Status() const
    {
      return m_iStatus;
    }

    const int & Helicity() const
    {
      return m_iHelicity;
    }

    const int & ColorID1() const
    {
      return m_iColorID1;
    }

    void SetColorID1(int newValue) 
    {
      m_iColorID1 = newValue;
    }

    const int & ColorID2() const
    {
      return m_iColorID2;
    }

    void SetColorID2(int newValue) 
    {
      m_iColorID2 = newValue;
    }


    const int & MotherID1() const
    {
      return m_iMotherID1;
    }

    void SetMotherID1(int newValue)
    {
      m_iMotherID1 = newValue;
    }

    const int & MotherID2() const
    {
      return m_iMotherID2;
    }

    void SetMotherID2(int newValue)
    {
      m_iMotherID2 = newValue;
    }

    const int& ID() const
    {
      return m_iParticleID;
    }

    void SetID(int newValue)
    {
      m_iParticleID = newValue;
    }

    const double& GeneratedMass() const
    {
      return m_dGeneratedMass;
    }

    CParticle* SetGeneratedMass(double newMass)
    {
      m_dGeneratedMass = newMass;
      return this;
    }

    FlavorType& SetFlavor()
    {
      return m_Flavor;
    }

    void SetFlavor(FlavorType newFlavor)
    {
      m_Flavor = newFlavor;
    }

    CLorentzVector& SetMomentum()
    {
      return m_Momentum;
    }

    int &SetStatus()
    {
      return m_iStatus;
    }

    void SetStatus(int status)
    {
      m_iStatus = status;
    }

    int &SetHelicity()
    {
      return m_iHelicity;
    }

  }; // end of class CParticle

  /**
   * @brief Streaming operator for CParticle
   */
  inline std::ostream & operator << (std::ostream & stream, const CParticle & particle)
  {
    stream << "Particle["<< particle.Flavor() <<",Status["<< particle.Status() <<"],"<<",Helicity["<< particle.Helicity() <<"],"<< particle.Momentum() <<"]";
    return stream;
  }

}; // end of namespace WZdecay

#endif