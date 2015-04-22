#ifndef __WZdecay_LORENTZVECTOR_H__
#define __WZdecay_LORENTZVECTOR_H__ 1

// standard include
#include <iostream>
#include <cmath>
#include "threevector.h"


namespace WZdecay {

  /**
   * @brief Class containing a fourvector with energy / momentum functionality. energy is last component
   */
  class CLorentzVector : public CThreeVector  
  {
  protected:
    /**
     * @brief time-like component interpretation as energy 
     */
    double m_dT;

  public:
    // constructors and destructors
    CLorentzVector():CThreeVector(),m_dT(0)
    {}

    CLorentzVector(const CThreeVector & vector, double dT):
    CThreeVector(vector),
    m_dT(dT)
    {}

    CLorentzVector(double dX, double dY, double dZ, double dT):
    CThreeVector(dX,dY,dZ),
    m_dT(dT)
    {}

    ~CLorentzVector() = default;

    // Getter and Setter

    /**
     * @brief Get $T$-component
     */
    const double & T() const
    {
      return this->m_dT;
    }
    /**
     * @brief Set $T$-component
     */
    void SetT(double dTnew)
    {
      m_dT = dTnew;
    }

    /**
     * @brief Get Momentum
     */
    CThreeVector ThreeVector() const
    {
      return CThreeVector(m_dX, m_dY, m_dZ);
    }

    // physical member functions
    double Magnitude2() const
    {
      return (m_dT * m_dT - ThreeVector().Magnitude2());
    }
    double Magnitude() const
    {
      return (std::sqrt(std::abs(Magnitude2())));
    }
    double Abs2() const
    {
      return Magnitude2();
    }
    double Abs() const
    {
      return Magnitude();
    }

    // Pseudorapidity
    double Rapidity() const
    {
      return 0.5*std::log((m_dT + m_dZ)/(m_dT - m_dZ));
    }

    double Pseudorapidity() const
    {
      return -std::log(std::tan(0.5*Theta()));
    }

    /**
     * @brief Alias for Pseudorapidity()
     */
    double Eta() const
    {
      return Pseudorapidity();
    }

    /**
     * @brief Returns boost vector
     */
    CThreeVector BoostVector() const
    {
      double dT = T();
      return (this->ThreeVector()/= dT);
    }

    /**
     * @brief Boost Lorentzvector using Boostvector (x,y,z)
     * @todo  check if boost works as expected
     */
    void Boost(double x, double y, double z); 
    CLorentzVector& Boost(const CThreeVector& kBoostvector)
    {
      Boost(kBoostvector.X(), kBoostvector.Y(), kBoostvector.Z());
      return *this;
    }

    // overloading of arithmetic operators
    CLorentzVector & operator += (const CLorentzVector & vector2)
    {
      CThreeVector::operator+=(vector2);
      m_dT += vector2.m_dT;
      return *this;
    }

    CLorentzVector & operator -= (const CLorentzVector & vector2)
    {
      CThreeVector::operator-=(vector2);
      m_dT -= vector2.m_dT;
      return *this;
    }

    CLorentzVector & operator *= (const double &dFactor)
    {
      CThreeVector::operator*=(dFactor);
      m_dT *= dFactor;
      return *this;
    }

    CLorentzVector & operator /= (const double &dFactor)
    {
      CThreeVector::operator /=(dFactor);
      m_dT /= dFactor;
      return *this;
    }

  }; //end of class CLorentzVector

  inline std::ostream & operator << (std::ostream & stream, const CLorentzVector & vector)
  {
    stream << "LorentzVector["<<vector.X()<<","<<vector.Y()<<","<<vector.Z() <<","<<vector.T() <<"]";
    return stream;
  }

  double Dot(const CLorentzVector & vector1, const CLorentzVector & vector2);

  // arithmetic operations

  inline CLorentzVector operator + (const CLorentzVector& vector1, const CLorentzVector & vector2)
  {
    return CLorentzVector(
      vector1.ThreeVector() + vector2.ThreeVector(),
      vector1.T() + vector2.T());
  }

  inline CLorentzVector operator - (const CLorentzVector& vector1, const CLorentzVector & vector2)
  {
    return CLorentzVector(
      vector1.ThreeVector() - vector2.ThreeVector(),
      vector1.T() - vector2.T());
  }

  inline double operator * (const CLorentzVector& vector1, const CLorentzVector & vector2)
  {
    return Dot(vector1, vector2);
  }

  // operations with doubles
  inline CLorentzVector operator * (const CLorentzVector& vector1, const double& dFactor)
  {
    return CLorentzVector(
      vector1.ThreeVector() * dFactor,
      vector1.T() * dFactor);
  }

  inline CLorentzVector operator / (const CLorentzVector& vector1, const double &dFactor)
  {
    return vector1 * (1/dFactor);
  }

  inline CLorentzVector operator + (const CLorentzVector& vector1, const double & dSummand)
  {
    return CLorentzVector(
      vector1.ThreeVector() + dSummand,
      vector1.T() + dSummand);
  }

  inline CLorentzVector operator - (const CLorentzVector& vector1, const double & dSummand)
  {
    return vector1 + (-1*dSummand);
  }

  inline CLorentzVector operator * (const double& dFactor, const CLorentzVector& vector1)
  {
    return CLorentzVector(
      vector1.ThreeVector() * dFactor,
      vector1.T() * dFactor);
  }

  inline CLorentzVector operator / (const double &dFactor, const CLorentzVector& vector1)
  {
    return vector1 * (1 / dFactor);
  }

  inline CLorentzVector operator + (const double & dSummand, const CLorentzVector& vector1)
  {
    return CLorentzVector(
      vector1.ThreeVector() + dSummand,
      vector1.T() + dSummand);
  }

  inline CLorentzVector operator - (const double & dSummand, const CLorentzVector& vector1)
  {
    return vector1 + (-1*dSummand);
  }


}; // end of namespace WZdecay

#endif

