#include "cmdline.h"

CMDLine::CMDLine(int argc, const char *argv[], CMDLine::ErrorMode mode,  const char *delim)
{
    this->mode = mode;
    this->delim = delim;
    for(int i = 0; i < argc; i++)parsedParts.push_back(ARG(argv[i],i));
}

CMDLine::CMDLine(std::string line,  ErrorMode mode,  const char *delim)
{
    this->delim = delim;
    this->mode = mode;
    parse(line);
}

CMDLine::CMDLine(std::istream &inputStream,  ErrorMode mode, const char *delim)
{
    this->mode = mode;
    this->delim = delim;
    std::string str;
    std::getline(inputStream,str);
    parse(str);
}

const CMDLine::ARG &CMDLine::nextOption(const CMDLine::ARG &prev){
    if(prev.opt.empty()){
        throw std::invalid_argument("The arg passed to nextOption was not returned by getOption or nextOption");
    }
    const ARG & r = getOption(prev.opt,prev.index()+1);
    
    if(!r)return empty;
    ARG & nonconst = parsedParts[r.index()];
    nonconst.opt = prev.opt;
    return nonconst;
}

const CMDLine::ARG & CMDLine::getOption(std::string option, const ARG & defaultValue, size_t begin)
{
    for(size_t i = begin; i < parsedParts.size(); i++){
        if(parsedParts[i].m_data == option){
            if(i+1 < parsedParts.size()){
                parsedParts[i+1].opt = option;
                return parsedParts[i+1];
            }else{
                return defaultValue;
            }
        }
    }
    return defaultValue;
}

const CMDLine::ARG &CMDLine::getOption(std::string option, size_t begin)
{
    for(size_t i = begin; i < parsedParts.size(); i++){
        if(parsedParts[i].m_data == option){
            if(i+1 < parsedParts.size()){
                parsedParts[i+1].opt = option;
                return parsedParts[i+1];
            }else{
                return empty;
            }
        }
    }
    return empty;
}

const CMDLine::ARG &CMDLine::findArgument(std::string option, size_t begin)
{
    for(size_t i = begin; i < parsedParts.size(); i++){
        if(parsedParts[i].m_data == option){
            return parsedParts[i];
        }
    }
    return empty;
}

void CMDLine::setErrorMode(CMDLine::ErrorMode mode)
{
    this->mode = mode;
}

int CMDLine::argc()
{
    return parsedParts.size();
}

const CMDLine::ARG & CMDLine::operator[](int index){
    if(index >= parsedParts.size()){
        if(mode == OutOfRangeException)throw std::out_of_range("Invalid argument index");
        return empty;
    }
    return parsedParts[index];
}

inline bool isDelim(char c, const char * delim){
    while(*delim != 0){
        if(*delim == c)return true;
        delim++;
    }
    return false;
}


void CMDLine::parse(std::string str)
{
    int state = 0;
    for(auto i = str.begin(); i != str.end(); i++){
        char c = *i;
        if(state == 0){ // Whitespace
            if(!isDelim(c,delim)){
                if(c == '"'){
                    parsedParts.push_back(ARG());
                    state = 2;
                }else{
                    parsedParts.push_back(ARG());
                    parsedParts.back().m_data += c;
                    state = 1;
                }
                parsedParts.back().bad = false;
                parsedParts.back().idx = parsedParts.size()-1;
            }
        }else if(state == 1){ // Normal String
            if(!isDelim(c,delim)){
                parsedParts.back().m_data += c;
            }else{
                state = 0;
            }
        }else{ // String delimited by "
            if(c != '"'){
                parsedParts.back().m_data += c;
            }else{
                state = 0;
            }
        }
    }
}

std::pair<CMDLine::ARG, CMDLine::ARG> CMDLine::ARG::splitOnFirst(char sep) const{
    auto idx = m_data.find_first_of(sep);

    if(idx == std::string::npos){
        auto r = std::make_pair<ARG,ARG>(ARG(),ARG());
        return r;
    }

    if(idx < m_data.length()-1){
        return std::make_pair<ARG,ARG>(m_data.substr(0,idx), m_data.substr(idx+1,m_data.length()));
    }
    else{
        return std::make_pair<ARG,ARG>(m_data.substr(0,idx),ARG());
    }
}
