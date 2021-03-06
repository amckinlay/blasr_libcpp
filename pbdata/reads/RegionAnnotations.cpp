// Author: Yuan Li

#include "RegionAnnotations.hpp"
#include <algorithm>
#include <cassert>

RegionAnnotations::RegionAnnotations(const UInt holeNumber,
                                     const std::vector<RegionAnnotation>& annotations,
                                     const std::vector<RegionType>& types)
    : table_(annotations), holeNumber_(holeNumber), types_(types)
{
    for (auto annotation : annotations) {
        // Only allow RegionAnnotations of a single ZMW
        if (holeNumber_ != static_cast<UInt>(annotation.GetHoleNumber())) {
            assert(false && "RegionAnnotations must contain regions from a single ZMW");
        }
    }

    std::sort(table_.begin(), table_.end(), compare_region_annotation_by_type);
}

RegionAnnotations::RegionAnnotations(const RegionAnnotations& rhs)
    : table_(rhs.table_), holeNumber_(rhs.holeNumber_), types_(rhs.types_)
{
}

UInt RegionAnnotations::HoleNumber(void) const { return holeNumber_; }

std::vector<RegionAnnotation> RegionAnnotations::RegionAnnotationsOfType(RegionType type) const
{

    std::vector<RegionAnnotation> ret;
    int typeIndex = RegionTypeMap::ToIndex(type, types_);
    if (typeIndex >= 0) {
        for (auto ra : table_)
            if (ra.GetTypeIndex() == typeIndex) ret.push_back(ra);
        sort(ret.begin(), ret.end());
    }
    return ret;
}

std::vector<RegionAnnotation> RegionAnnotations::Adapters() const
{
    return RegionAnnotationsOfType(Adapter);
}

bool RegionAnnotations::HasHQRegion() const
{
    return (HQRegions().size() >= 1 and HQEnd() - HQStart() > 0);
}

std::vector<RegionAnnotation> RegionAnnotations::HQRegions() const
{
    return RegionAnnotationsOfType(HQRegion);
}

RegionAnnotation RegionAnnotations::TheHQRegion() const
{
    std::vector<RegionAnnotation> hqs_ = HQRegions();
    if (hqs_.size() == 0)
        return RegionAnnotation(holeNumber_, RegionTypeMap::ToIndex(HQRegion, types_), 0, 0, 0);
    else if (hqs_.size() == 1)
        return hqs_[0];
    else
        assert(false && "Zmw has more than one HQRegion.");
}

DNALength RegionAnnotations::HQStart() const { return TheHQRegion().GetStart(); }

DNALength RegionAnnotations::HQEnd() const { return TheHQRegion().GetEnd(); }

int RegionAnnotations::HQScore() const { return TheHQRegion().GetScore(); }

std::vector<RegionAnnotation> RegionAnnotations::Inserts() const
{
    return RegionAnnotationsOfType(Insert);
}

std::vector<ReadInterval> RegionAnnotations::AdapterIntervals() const
{
    std::vector<ReadInterval> ret;
    for (auto adapter : Adapters()) {
        ret.push_back(ReadInterval(adapter));
    }
    return ret;
}

std::vector<ReadInterval> RegionAnnotations::SubreadIntervals(const DNALength wholeLength,
                                                              const bool byAdapter,
                                                              const bool byHQRegion) const
{
    std::vector<RegionAnnotation> inserts;
    if (not byAdapter) {
        inserts = Inserts();
    } else {
        if (Adapters().size() != 0) {
            // Must have at least one adapter in order find inserts by adapter.
            std::vector<DNALength> starts, ends;
            starts.push_back(0);
            for (auto adapter : Adapters()) {
                assert(wholeLength >= static_cast<DNALength>(adapter.GetStart()) and
                       wholeLength >=
                           static_cast<DNALength>(adapter.GetEnd()));  // bug if fail assert
                starts.push_back(adapter.GetEnd());
                ends.push_back(adapter.GetStart());
            }
            ends.push_back(wholeLength);

            for (size_t i = 0; i < starts.size(); i++) {
                // Use adapter to infer subreads, read score considered unknown.
                if (ends[i] > starts[i]) {
                    inserts.push_back(RegionAnnotation(holeNumber_, Insert, starts[i], ends[i], 0));
                }
            }
        }  // else no inserts can be found
    }

    std::vector<ReadInterval> ret;
    for (auto insert : inserts) {
        if (byHQRegion) {
            if (HasHQRegion()) {
                DNALength s = std::max(static_cast<UInt>(insert.GetStart()), HQStart());
                DNALength e = std::min(static_cast<UInt>(insert.GetEnd()), HQEnd());
                if (s < e) {
                    // subreads' read score = HQRegion score.
                    ret.push_back(ReadInterval(s, e, HQScore()));
                }
            }  // else ret = {}
        } else {
            ret.push_back(ReadInterval(insert));
        }
    }
    return ret;
}
