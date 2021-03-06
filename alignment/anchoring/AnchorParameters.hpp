#ifndef _BLASR_ANCHOR_PARAMETERS_HPP_
#define _BLASR_ANCHOR_PARAMETERS_HPP_

#include <fstream>
#include <iostream>
#include "../../pbdata/DNASequence.hpp"
#include "../../pbdata/qvs/QualityValue.hpp"

class AnchorParameters
{
public:
    QualityValue branchQualityThreshold;
    DNALength minMatchLength;
    int maxMatchScore;
    int expand;
    int contextAlignLength;
    bool useLookupTable;
    int numBranches;
    int maxAnchorsPerPosition;
    int advanceExactMatches;
    DNALength maxLCPLength;
    bool stopMappingOnceUnique;
    int verbosity;
    bool removeEncompassedMatches;
    std::ostream *lcpBoundsOutPtr;
    int branchExpand;

    AnchorParameters();

    AnchorParameters &Assign(const AnchorParameters &rhs);

    AnchorParameters &operator=(const AnchorParameters &rhs);
};

#endif  // _BLASR_ANCHOR_PARAMETERS_HPP_
