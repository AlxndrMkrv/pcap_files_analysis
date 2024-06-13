#pragma once

#include "Packet/Base.hxx"
#include <iostream>

namespace Statistics {

class ChecksumValidity {
public:
    void update(const ::Packet::Abstract & packet);

    size_t valids() const;
    size_t invalids() const;
    size_t total() const;

    friend std::ostream & operator<<(std::ostream & os,
                                     const ChecksumValidity & obj);

private:
    size_t _valids = 0;
    size_t _invalids = 0;
};

} // namespace Statistics
