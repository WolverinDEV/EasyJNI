//
// Created by wolverindev on 18.01.17.
//

#pragma once

#include <cstdio>
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include <vector>

template<typename Ch, typename Traits = std::char_traits<Ch>, typename Sequence = std::vector<Ch> >
struct basic_seqbuf : std::basic_streambuf<Ch, Traits> {
        typedef std::basic_streambuf<Ch, Traits> base_type;
        typedef typename base_type::int_type int_type;
        typedef typename base_type::traits_type traits_type;

        virtual int_type overflow(int_type ch) {
            if(traits_type::eq_int_type(ch, traits_type::eof()))
                return traits_type::eof();
            c.push_back(traits_type::to_char_type(ch));
            return ch;
        }

        Sequence const& get_sequence() const {
            return c;
        }
    protected:
        Sequence c;
};




/*
class membuf : public std::basic_streambuf<char> {
    public:
        membuf(const uint8_t *p, size_t l) {
            setg((char*) p, (char*) p, (char*) p + l);
        }
};

class memstream : public std::istream {
    public:
        memstream(const uint8_t *p, size_t l) : std::istream(&_buffer), _buffer(p, l) {
            rdbuf(&_buffer);
        }

    private:
        membuf _buffer;
};
*/

//
// http://www.mr-edd.co.uk/blog/beginners_guide_streambuf
//

class byte_array_buffer : public std::streambuf
{
    public:
        byte_array_buffer(const uint8_t *begin, const size_t size);

    private:
        int_type underflow();
        int_type uflow();
        int_type pbackfail(int_type ch);
        std::streamsize showmanyc();
        std::streampos seekoff ( std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out );
        std::streampos seekpos ( std::streampos sp, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);

        // copy ctor and assignment not implemented;
        // copying not allowed
        byte_array_buffer(const byte_array_buffer &);
        byte_array_buffer &operator= (const byte_array_buffer &);

    private:
        const uint8_t * const begin_;
        const uint8_t * const end_;
        const uint8_t * current_;
};

class memstream : public std::istream {
    public:
        memstream(const uint8_t *p, size_t l) : std::istream(&_buffer), _buffer(p, l) {
            rdbuf(&_buffer);
        }

    private:
        byte_array_buffer _buffer;
};