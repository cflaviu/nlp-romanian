#include "nlp/romanian/phonetics.hpp"
#include <stdexcept>

// https://ro.wikipedia.org/wiki/Wikipedia:Alfabetul_fonetic_rom%C3%A2nesc
// https://ro.wikipedia.org/wiki/Desp%C4%83r%C8%9Birea_%C3%AEn_silabe_(limba_rom%C3%A2n%C4%83)
// https://www.literparc.ro/cum-deosebim-semivocala-vocala/
// https://www.scientia.ro/homo-humanus/44-dificultati-ale-limbii-romane/1068-regulile-despartirii-cuvintelor-in-silabe.html

// sv - ascendent diphthong: ea, eo, ia, ie, io, iu, oa, ua, uă
// vs - descendent diphthong: ai, au, ei, eu, ii, iu, oi, ou, ui, âi, âu, ăi, ău
// svs - centered triphthong: eai, eau, iai, iau, iei, oai
// ssv - ascendent triphthong: ioa, eoa

namespace nlp::romanian::phonetics
{
    const std::string_view& notation_of(sound_group g) noexcept
    {
        static const std::string_view texts[] {
            {"C", 1}, {"V", 1}, {"2T", 2}, {"3T", 2}, {"G3", 2}, {"X", 1},
        };

        return texts[+g];
    }

    bool is_vowel_or_semivowel(const chr c) noexcept
    {
        switch (c)
        {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
            case +s_chr::aa:
            case +s_chr::i_a:
            case +s_chr::i_i:
                return true;
            default:
                return false;
        }
    }

    bool is_diphthong(const word_span& word_section)
    {
        bool result = false;
        auto c1 = word_section[0];
        auto c2 = word_section[1];
        switch (c1)
        {
            case 'a':
            {
                switch (c2)
                {
                    case 'i':
                    case 'u':
                    {
                        result = true;
                        break;
                    }
                    default:;
                }
                break;
            }
            case 'e':
            {
                switch (c2)
                {
                    case 'a':
                    case 'o':
                    case 'i':
                    case 'u':
                    {
                        result = true;
                        break;
                    }
                    default:;
                }
                break;
            }
            case 'i':
            {
                switch (c2)
                {
                    case 'a':
                    case 'e':
                    case 'o':
                    case 'u':
                    case 'i':
                    {
                        result = true;
                        break;
                    }
                    default:;
                }
                break;
            }
            case 'o':
            {
                switch (c2)
                {
                    case 'a':
                    case 'i':
                    case 'u':
                    {
                        result = true;
                        break;
                    }
                    default:;
                }
                break;
            }
            case 'u':
            {
                switch (c2)
                {
                    case 'a':
                    case +s_chr::aa:
                    case 'i':
                    {
                        result = true;
                        break;
                    }
                    default:;
                }
                break;
            }
            case +s_chr::aa:
            case +s_chr::i_a:
            {
                switch (c2)
                {
                    case 'i':
                    case 'u':
                    {
                        result = true;
                        break;
                    }
                    default:;
                }
                break;
            }
            default:;
        }

        return result;
    }

    bool is_triphthong(const word_span& word_section)
    {
        bool result = false;
        auto c1 = word_section[0];
        auto c2 = word_section[1];
        auto c3 = word_section[2];
        switch (c1)
        {
            case 'e':
            {
                switch (c2)
                {
                    case 'a':
                    {
                        if (c3 == 'i' || c3 == 'u')
                        {
                            result = true;
                            break;
                        }

                        throw std::runtime_error("unknown triphthong case ea-");
                        break;
                    }
                    case 'o':
                    {
                        if (c3 == 'a')
                        {
                            result = true;
                            break;
                        }

                        throw std::runtime_error("unknown triphthong case eo-");
                        break;
                    }
                    default:
                    {
                        throw std::runtime_error("unknown triphthong case e--");
                    }
                }
                break;
            }
            case 'i':
            {
                switch (c2)
                {
                    case 'a':
                    {
                        if (c3 == 'i' || c3 == 'u')
                        {
                            result = true;
                            break;
                        }

                        throw std::runtime_error("unknown triphthong case ia-");
                        break;
                    }
                    case 'e':
                    {
                        if (c3 == 'i')
                        {
                            result = true;
                            break;
                        }

                        throw std::runtime_error("unknown triphthong case ia-");
                        break;
                    }
                    case 'o':
                    {
                        if (c3 == 'a')
                        {
                            result = true;
                            break;
                        }

                        throw std::runtime_error("unknown triphthong case ia-");
                        break;
                    }
                    default:
                    {
                        throw std::runtime_error("unknown triphthong case i--");
                    }
                }
                break;
            }
            case 'o':
            {
                if (c2 == 'a' && c3 == 'i')
                {
                    result = true;
                    break;
                }

                throw std::runtime_error("unknown triphthong case o--");
                break;
            }
            default:;
        }

        return result;
    }

    void sound_group_analysis(const word_span& word, sound_group_span& sound_group)
    {
        auto dest = sound_group.begin();
        for (size_t i = 0u; i != word.size(); ++dest)
        {
            if (is_vowel_or_semivowel(word[i]))
            {
                auto j = i + 1;
                for (; j != word.size() && is_vowel_or_semivowel(word[j]); ++j) {}
                switch (j - i)
                {
                    case 1:
                    {
                        *dest = sound_group::vowel;
                        break;
                    }
                    case 2:
                    {
                        if (is_diphthong({word.data() + i, 2}))
                        {
                            *dest = sound_group::diphthong;
                        }
                        else
                        {
                            *dest = sound_group::vowel;
                            ++dest;
                            *dest = sound_group::vowel;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (is_triphthong({word.data() + i, 3}))
                        {
                            *dest = sound_group::triphthong;
                        }
                        else if (is_diphthong({word.data() + i + 1, 2}))
                        {
                            *dest = sound_group::vowel;
                            ++dest;
                            *dest = sound_group::diphthong;
                        }
                        else
                        {
                            throw std::runtime_error("unexpected case <3>");
                        }
                        break;
                    }
                    case 4:
                    {
                        if (is_triphthong({word.data() + i + 1, 3}))
                        {
                            *dest = sound_group::vowel;
                            ++dest;
                            *dest = sound_group::triphthong;
                        }
                        else if (is_diphthong({word.data() + i, 2}) && is_diphthong({word.data() + i + 2, 2}))
                        {
                            *dest = sound_group::diphthong;
                            ++dest;
                            *dest = sound_group::diphthong;
                        }
                        else
                        {
                            throw std::runtime_error("unexpected case <4>");
                        }
                        break;
                    }
                    case 5:
                    {
                        if (is_diphthong({word.data() + i, 2}) && is_triphthong({word.data() + i + 2, 3}))
                        {
                            *dest = sound_group::diphthong;
                            ++dest;
                            *dest = sound_group::triphthong;
                        }
                        else
                        {
                            throw std::runtime_error("confused <5>");
                        }

                        break;
                    }
                    default:
                    {
                        throw std::runtime_error("unexpected number of vowels/semivowels");
                    }
                }

                i = j;
            }
            else
            {
                auto c = word[i++];
                switch (c)
                {
                    case 'x':
                        *dest = sound_group::x_consonant;
                        break;
                    case 'c':
                    case 'g':
                        if (i != word.size() && word[i] == 'h')
                        {
                            auto j = i + 1;
                            if (j < word.size() && (word[j] == 'e' || word[j] == 'i'))
                            {
                                *dest = sound_group::group_3;
                                i += 2;
                                break;
                            }
                        }

                        [[fallthrough]];
                    default:
                        *dest = sound_group::consonant;
                }
            }
        }

        sound_group = sound_group_span(sound_group.data(), std::distance(sound_group.begin(), dest));
    }
}
