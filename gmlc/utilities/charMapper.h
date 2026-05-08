/*
 * LLNS Copyright Start
 * Copyright (c) 2017, Lawrence Livermore National Security
 * This work was performed under the auspices of the U.S. Department
 * of Energy by Lawrence Livermore National Laboratory in part under
 * Contract W-7405-Eng-48 and in part under Contract DE-AC52-07NA27344.
 * Produced at the Lawrence Livermore National Laboratory.
 * All rights reserved.
 * For details, see the LICENSE file.
 * LLNS Copyright End
 */
#pragma once

#include <array>
#include <initializer_list>
namespace gmlc::utilities {
/** small helper class to map characters to values*/
template<typename V>
class CharMapper {
  private:
    std::array<V, 256> key;  //!< the character map
  public:
    /** default constructor*/
    constexpr explicit CharMapper(V defVal = V{0}) noexcept
    {
        key.fill(defVal);
    }
    /** update the value returned from a key query
@details this is purposely distinct from the [] operator to make it an error
to try to assign something that way
*/
    constexpr void addKey(unsigned char keyChar, V val) noexcept
    {
        key[keyChar] = val;
    }
    /** get the value assigned to a character
     * @param keyChar the character to test or convert
     * @return the resulting value,  0 if nothing in particular is specified
     * in a given map
     */
    [[nodiscard]] constexpr V at(unsigned char keyChar) const noexcept
    {
        return key[keyChar];
    }
    /** get the value assigned to a character by bracket notation
     * @param keyChar the character to test or convert
     * @return the resulting value,  0 if nothing in particular is specified
     * in a given map
     */
    [[nodiscard]] constexpr V operator[](unsigned char keyChar) const noexcept
    {
        return key[keyChar];
    }
};

namespace detail {
    constexpr void markNumericDigits(CharMapper<bool>& mapper) noexcept
    {
        for (unsigned char indexChar = '0'; indexChar <= '9'; ++indexChar) {
            mapper.addKey(indexChar, true);
        }
    }

    constexpr void markKeys(
        CharMapper<bool>& mapper,
        std::initializer_list<unsigned char> keys) noexcept
    {
        for (auto key : keys) {
            mapper.addKey(key, true);
        }
    }
}  // namespace detail
/** map that translates all characters that could be in numbers to true all
 * others to false*/
constexpr CharMapper<bool> numericMapper() noexcept
{
    CharMapper<bool> mapper(false);
    detail::markNumericDigits(mapper);
    detail::markKeys(mapper, {'+', '-', ' ', 'e', 'E', '.'});
    return mapper;
}
/** map that translates all characters that could start a number to true, all
 * others to false*/
constexpr CharMapper<bool> numericStartMapper() noexcept
{
    CharMapper<bool> mapper(false);
    detail::markNumericDigits(mapper);
    detail::markKeys(mapper, {'+', '-', ' ', '\t', '.', '\n', '\r', '\0'});
    return mapper;
}
/** map that translates all characters that could end a number to true, all
 * others to false*/
constexpr CharMapper<bool> numericEndMapper() noexcept
{
    CharMapper<bool> mapper(false);
    detail::markNumericDigits(mapper);
    detail::markKeys(mapper, {' ', '\t', '\n', '\r', '\0'});
    return mapper;
}
/** map that translates all base 64 characters to the appropriate numerical
 * value*/
constexpr CharMapper<unsigned char> base64Mapper() noexcept
{
    CharMapper<unsigned char> b64(0xFF);
    unsigned char val{0};
    for (unsigned char indexChar = 'A'; indexChar <= 'Z'; ++indexChar) {
        b64.addKey(indexChar, val);
        ++val;
    }
    for (unsigned char indexChar = 'a'; indexChar <= 'z'; ++indexChar) {
        b64.addKey(indexChar, val);
        ++val;
    }
    for (unsigned char indexChar = '0'; indexChar <= '9'; ++indexChar) {
        b64.addKey(indexChar, val);
        ++val;
    }
    b64.addKey('+', val++);
    b64.addKey('/', val);
    return b64;
}
/** map that translates numerical characters to the appropriate numerical
 * value*/
constexpr CharMapper<unsigned char> digitMapper() noexcept
{
    CharMapper<unsigned char> dMap(0xFF);
    unsigned char val = 0;
    for (unsigned char indexChar = '0'; indexChar <= '9'; ++indexChar) {
        dMap.addKey(indexChar, val);
        ++val;
    }
    return dMap;
}
/** map that translates all hexadecimal characters to the appropriate
 * numerical value*/
constexpr CharMapper<unsigned char> hexMapper() noexcept
{
    CharMapper<unsigned char> dMap(0xFF);
    unsigned char val = 0;
    for (unsigned char indexChar = '0'; indexChar <= '9'; ++indexChar) {
        dMap.addKey(indexChar, val);
        ++val;
    }
    for (unsigned char indexChar = 'A'; indexChar <= 'F'; ++indexChar) {
        dMap.addKey(indexChar, val);
        ++val;
    }
    val = 10;
    for (unsigned char indexChar = 'a'; indexChar <= 'f'; ++indexChar) {
        dMap.addKey(indexChar, val);
        ++val;
    }
    return dMap;
}
/** map that all containing characters that come in pairs to the appropriate
 * match '{' to '}'*/
constexpr CharMapper<unsigned char> pairMapper() noexcept
{
    CharMapper<unsigned char> dMap(0);
    for (unsigned char ii = 0; ii < 255; ++ii) {
        dMap.addKey(ii, ii);
    }
    dMap.addKey('(', ')');
    dMap.addKey('<', '>');
    dMap.addKey('[', ']');
    dMap.addKey('{', '}');
    dMap.addKey(')', '(');
    dMap.addKey('>', '<');
    dMap.addKey(']', '[');
    dMap.addKey('}', '{');
    dMap.addKey(0x02, 0x03);
    dMap.addKey(0x03, 0x02);
    dMap.addKey(0x0E, 0x0F);
    dMap.addKey(0x0F, 0x0E);
    dMap.addKey(0x11, 0x13);
    dMap.addKey(0x13, 0x11);
    return dMap;
}

inline constexpr CharMapper<bool> numericMap = numericMapper();
inline constexpr CharMapper<bool> numericStartMap = numericStartMapper();
inline constexpr CharMapper<bool> numericEndMap = numericEndMapper();
inline constexpr CharMapper<unsigned char> base64Map = base64Mapper();
inline constexpr CharMapper<unsigned char> digitMap = digitMapper();
inline constexpr CharMapper<unsigned char> hexMap = hexMapper();
inline constexpr CharMapper<unsigned char> pairMap = pairMapper();
inline constexpr CharMapper<bool> numCheck = numericStartMap;
inline constexpr CharMapper<bool> numCheckEnd = numericEndMap;

}  // namespace gmlc::utilities
