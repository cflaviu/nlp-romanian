#pragma once
#ifndef PCH
    #include "nlp/romanian/base.hpp"
    #include <span>
#endif

namespace nlp::romanian::phonetics
{
    enum class sound_group : uint8_t
    {
        consonant,   // C
        vowel,       // V
        diphthong,   // 2T
        triphthong,  // 3T
        group_3,     // G3
        x_consonant, // X
    };

    const std::string_view& notation_of(sound_group g) noexcept;

    bool is_vowel_or_semivowel(const chr c) noexcept;
    bool is_diphthong(const word_span& word_section);
    bool is_triphthong(const word_span& word_section);

    using sound_group_span = std::span<sound_group>;

    void sound_group_analysis(const word_span& word, sound_group_span& sound_group);
}
