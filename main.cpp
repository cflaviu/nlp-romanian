#include "nlp/romanian/phonetics.hpp"
#include "to_utf8/from_utf16.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void print_separated(const auto& items, const char separator = '|')
{
    for (auto i: items)
    {
        std::cout << notation_of(i) << separator;
    }

    std::cout << std::endl;
}

void print_word(const std::u16string& word)
{
    std::array<char, sizeof(nlp::romanian::chr) * nlp::romanian::max_word_length> buffer;
    auto p = buffer.data();
    for (auto c: word)
    {
        p = to_utf8::from_utf16(c, p);
    }

    std::cout << std::string_view(buffer.data(), p - buffer.begin()) << std::endl;
}

void read_words(const char file_name[], std::vector<std::u16string>& words)
{
    std::ifstream input_file(file_name, std::ios::binary);
    std::vector<nlp::romanian::chr> buffer;
    if (input_file.is_open())
    {
        input_file.seekg(0, std::ifstream::end);
        const auto file_size = input_file.tellg();
        input_file.seekg(0, std::ifstream::beg);
        buffer.resize(file_size / 2);
        if (file_size > 3u)
        {
            input_file.read(reinterpret_cast<char*>(buffer.data()), file_size);
            if (input_file.gcount() == file_size)
            {
                input_file.close();

                const char16_t* p1 = buffer.data();
                constexpr char16_t bom = 0xFEFFu;
                if (*p1 == bom)
                    ++p1;

                std::u16string word;
                word.resize(nlp::romanian::max_word_length);

                for (const char16_t *p2, *end = buffer.data() + buffer.size();;)
                {
                    p2 = std::find(p1, end, L'\n');
                    const auto size = std::distance(p1, p2);
                    if (size != 0)
                    {
                        word.resize(size);
                        std::copy(p1, p2, word.begin());
                        words.push_back(word);
                        p1 = p2 + 1;
                    }
                    else
                        break;
                }
            }
            else
            {
                input_file.close();
                throw std::runtime_error("Failed reading input file");
            }
        }
        else
        {
            input_file.close();
            throw std::runtime_error("Too short input file");
        }
    }
    else
        throw std::runtime_error("Failed opening input file");
}

int main(int argc, const char* argv[]) noexcept
{
    if (argc == 2)
    {
        try
        {
            std::vector<std::u16string> words;
            read_words(argv[1u], words);

            using namespace nlp::romanian;
            using namespace nlp::romanian::phonetics;

            std::array<sound_group, max_word_length> sound_group_buffer;
            sound_group_span sound_groups(sound_group_buffer.data(), 0u);

            for (auto& word: words)
            {
                std::cout << "word        : ";
                print_word(word);

                sound_group_analysis(word, sound_groups);

                std::cout << "sound groups: ";
                print_separated(sound_groups);
                std::cout << "----------------------------------------------\n";
            }
        }
        catch (const std::exception& ex)
        {
            std::cout << ex.what() << '\n';
        }
    }
    else
    {
        std::cerr << "Expected input file containing one word per line and encoded as UTF-16 LE (without BOM)\n";
    }

    return 0;
}
