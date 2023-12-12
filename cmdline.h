#ifndef CMDLINE_H
#define CMDLINE_H

#include <string>
#include <istream>
#include <vector>
#include <sstream>
#include <charconv>
#include <stdexcept>

class CMDLine
{
public:
    enum ErrorMode{
        OutOfRangeException, EmptyString
    };

    struct split_result;

    class ARG {
        friend class CMDLine;
    private:
        std::string m_data;
        std::string opt;
        int idx;
        bool bad;
    public:
        ARG() : m_data(), bad(true), idx(-1) {}
        ARG(std::string data, int id = -1) : m_data(data), bad(false), idx(id) {}

        bool isBad() const{
            return bad;
        }

        operator std::string() const{
            return m_data;
        }

        explicit operator bool() const{
            return !bad;
        }

        template<typename T>
        operator T() const{
            T t;
            std::stringstream ss(m_data);
            ss >> t;
            if(ss.fail()) throw std::invalid_argument("General argument parsing failed");
            return t;
        }

        std::pair<ARG,ARG> splitOnFirst(char sep = '=') const;

        std::string str() const{
            return m_data;
        }


        template<typename T>
        T num(int base = 10) const{
            T value;
            std::to_chars_result r = from_chars(m_data.data(), m_data.data()+m_data.size(),
                    value, base);

            if(r.ec != std::errc()) throw std::invalid_argument("Number argument parsing failed");
            return value;
        }

        int index() const{
            return idx;
        }
    };


    public:
        CMDLine(int argc, const char * argv[], ErrorMode mode = OutOfRangeException, const char * delim = " \t");
        CMDLine(std::string line, ErrorMode mode = OutOfRangeException, const char * delim = " \t");
        CMDLine(std::istream & inputStream, ErrorMode mode = OutOfRangeException, const char * delim = " \t");

        const ARG &nextOption(const ARG & prev);

        const ARG &getOption(std::string option, const ARG &defaultValue, size_t begin = 0); // find an argument of type -option value and return the value or the default
        const ARG &getOption(std::string option, size_t begin = 0); // find an argument of type -option value and return the value or an empty arg
        const ARG &findArgument(std::string option, size_t begin = 0); //find a string inside the set, and return it. otherwise empty arg

        void setErrorMode(ErrorMode mode);

        int argc();
        const ARG & operator[](int index);



    private:
        void parse(std::string str);
        std::vector<ARG> parsedParts;
        ErrorMode mode;
        ARG empty;
        const char *delim;
    };

#endif // CMDLINE_H
