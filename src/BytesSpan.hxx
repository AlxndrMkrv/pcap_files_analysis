#pragma once

#include <cstdint>
// #include <initializer_list>
#include <span>

class BytesSpan : public std::span<const uint8_t> {
public:
    using CByte = const uint8_t;
    using Underlying = const std::span<CByte>;

    using std::span<CByte>::span;

    // cppcheck-suppress [noExplicitConstructor]
    BytesSpan(std::span<CByte> span);
    BytesSpan(CByte dump[], size_t size);

    /*explicit BytesSpan(std::span<CByte> span);
    explicit BytesSpan(const std::span<CByte> & span);
    explicit BytesSpan(std::span<CByte> && span);*/
    /*explicit BytesSpan(const std::span<Underlying> span);
    explicit BytesSpan(const std::initializer_list<Underlying> bytes);
    explicit BytesSpan(const std::span<Underlying> & span);
    explicit BytesSpan(std::span<Underlying> && span);*/

    bool operator==(const BytesSpan & other) const;

    // const BytesSpan subspan(size_t offset) const;
    // BytesSpan subspan(size_t offset, size_t size) const;
};
