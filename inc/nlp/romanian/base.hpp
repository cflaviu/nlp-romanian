#pragma once
#ifndef PCH
    #include <string_view>
#endif

template <typename T>
constexpr auto operator+(T e) noexcept //-> std::enable_if_t<std::is_enum<T>::value, std::underlying_type_t<T>>
{
    return static_cast<std::underlying_type_t<T>>(e);
}

namespace nlp::romanian
{
    using chr = char16_t; // Romanian character
    using word_span = std::basic_string_view<chr>;
    constexpr word_span::size_type max_word_length = 32u;

    enum class s_chr : chr // special Romanian character (only small caps for now)
    {
        aa = 0x0103,  // ă 0103
        i_a = 0x00E2, // â 00E2
        i_i = 0x00EE, // î 00EE
        sh = 0x015F,  // ş 015F
        tz = 0x0163,  // ţ 0163
    };
}
