#pragma once
#ifndef PCH
    #include "nlp/romanian/base.hpp"
    #include <cstdint>
    #include <span>
#endif

namespace nlp::romanian::phonetics
{
    enum class sound_group : uint8_t
    {
        consonant,   // noted as C
        vowel,       // noted as V
        diphthong,   // noted as 2T
        triphthong,  // noted as 3T
        group_3,     // noted as G3
        x_consonant, // noted as X
    };

    const std::string_view& notation_of(sound_group g) noexcept;

    bool is_vowel_or_semivowel(const chr c) noexcept;
    bool is_diphthong(const word_span& word_section) noexcept;
    bool is_triphthong(const word_span& word_section);

    using sound_group_span = std::span<sound_group>;

    void sound_group_analysis(const word_span& word, sound_group_span& sound_group);
}
