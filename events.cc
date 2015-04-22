#include "events.h"

namespace WZdecay
{

  unsigned int CEvent::sm_uiCntEvent = 0;

  std::ostream & operator << (std::ostream & stream, const CEvent & event)
  {
    stream << "Event<"<< event.EventID() << "> (weight: "<< event.Weight()<<"), with xA = " << event.XParticleA() << ", with xB = " << event.XParticleB()<<std::endl;

    for (CEvent::particle_const_iterator iter = event.particles_cbegin(); iter != event.particles_cend(); ++iter)
    {
      stream << "   " << *iter << std::endl;
    }
    CLorentzVector itot, otot;

    // Print the incommings

    for (const CParticle& iter : event.VecParticles()) {
      if (iter.Status() == 4) {
        itot += iter.Momentum();
      }
      // subtract momentum of beam remnants using whizard convention
      if (iter.Flavor() == -92) {
        itot -= iter.Momentum();
      }
    }

    // Print the outgoings

    for (const CParticle& iter : event.VecParticles()) {
      if (iter.Status() == 1) {
        otot += iter.Momentum();
      }
    }

    // Print momentum summary

    stream << "Total incoming momentum: " << itot      << std::endl;
    stream << "Total outgoing momentum: " << otot      << std::endl;
    stream << "Momentum conservation:   " << itot-otot << std::endl;

    return stream;
  }


} // end of namespace WZdecay