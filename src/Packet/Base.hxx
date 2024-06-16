#pragma once

#include "BytesSpan.hxx"
#include <algorithm>
#include <cstdint>
#include <format>
#include <initializer_list>
#include <iostream>
#include <span>

namespace Packet {

template <size_t _Size>
class Address : public std::array<uint8_t, _Size> {
public:
    static constexpr size_t Size() { return _Size; }

    struct Compare {
        bool operator()(const Address & lhs, const Address & rhs) const
        {
            return lhs < rhs;
        };
    };

    using std::array<uint8_t, _Size>::array;
    Address(std::initializer_list<uint8_t> list)
    {
        std::copy(list.begin(), list.end(), this->begin());
    }

    auto operator<=>(const Address & other) const = default;

    /*operator BytesSpan() const
    {
        return BytesSpan{reinterpret_cast<const uint8_t *>(this), _Size};
    }

    operator const BytesSpan() const
    {
        return BytesSpan{reinterpret_cast<const uint8_t *>(this), _Size};
    }*/

private:
    // std::array<uint8_t, _Size> _octets = {};
};

class Abstract {
    template <class Header>
    friend class Base;

public:
    virtual ~Abstract() = default;

    virtual bool isSane() const = 0;
    virtual bool isChecksumOk() const = 0;
    virtual BytesSpan payload() const = 0;
    virtual void print(std::ostream & os) const = 0;

private:
    Abstract() = default;
};

template <class Header>
struct Base : public Abstract {
public:
    friend std::ostream & operator<<(std::ostream & os,
                                     const Base<Header> & obj)
    {
        obj.print(os);

        return os;
    }

protected:
    explicit Base(const BytesSpan data) :
        _data(data), _header(*reinterpret_cast<const Header *>(data.data())){};

    void printBytes(std::ostream & os) const
    {
        static constexpr size_t bytesInLine = 16;

        std::cout << "SIZE: " << _data.size() << std::endl;
        for (size_t r = 0; r < _data.size(); r += bytesInLine) {
            const auto line =
                _data.subspan(r, std::min(bytesInLine, _data.size() - r));

            if (line.empty())
                return;

            os << std::format("{:04x}: ", r);

            for (size_t c = 0; c < line.size(); ++c) {
                os << std::format("{:02x}", line[c]);
                if (c < line.size() - 1)
                    os << (c == 7 ? "  " : " ");
            }

            os << std::endl;
        }
    }

    const BytesSpan _data;
    const Header & _header;
};

} // namespace Packet
