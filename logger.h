#ifndef __logger_H__
#define __logger_H__ 1

#include <iostream>
#include <string>

namespace logger {
  class CLogger {

      // VERBOS  = 0
      // DEBUG   = 1
      // INFO    = 2
      // WARNING = 3
      // ERROR   = 4

  public:
    static unsigned int m_minLevel;

  private:
    static std::ofstream m_OutputToFile;
    std::string m_OutputToTerm;

    std::string levelToColoredString(unsigned int level) {
      switch (level) {
        case 0 : return "VERBOSE";
        case 1 : return "\033[1mDEBUG\033[0m";      // bold text
        case 2 : return "\033[1;32mINFO\033[0m";    // bold green text
        case 3 : return "\033[1;33mWARNING\033[0m"; // bold yellow text
        case 4 : return "\033[1;31mERROR\033[0m";   // bold red text
      }
      return "";
    }

  public:
    CLogger(std::string output):
      m_OutputToTerm(output) {
      toLog("Begin:\t" + m_OutputToTerm, 1);
    }

    ~CLogger() {
      toLog("End:  \t" + m_OutputToTerm, 1);
    }

    void toLog(std::string output, unsigned int loglevel = 2) {
      if (loglevel >= m_minLevel) {
        std::cout << levelToColoredString(loglevel) << ":\b\t  " << output << std::endl;
      }
    }

    // void toLog(std::stringstream output, unsigned int loglevel = 2) {
    //   if (loglevel >= m_minLevel) {
    //     std::cout << levelToColoredString(loglevel) << ":\b\t  " << output << std::endl;
    //   }
    // }

    static void SetLevel(unsigned int newLevel) {
      m_minLevel = newLevel;
    }

  };
}

#endif