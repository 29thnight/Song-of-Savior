#pragma once
#include <CoreDefine.h>

template<typename T>
inline T convertFromString(const std::string& str);

template<>
inline int convertFromString<int>(const std::string& str) 
{
    return std::stoi(str);
}

template<>
inline float convertFromString<float>(const std::string& str) 
{
    return std::stof(str);
}

template<>
inline bool convertFromString<bool>(const std::string& str) 
{
    return (str == "true" || str == "TRUE");
}

template<>
inline long double convertFromString<long double>(const std::string& str)
{
	return std::stold(str);
}

template<>
inline std::string convertFromString<std::string>(const std::string& str) 
{
    return str;
}

template<>
inline _uint convertFromString<_uint>(const std::string& str)
{
	return static_cast<_uint>(std::stoi(str));
}

template<typename... Types>
class CSVReader 
{
public:
    using RowType = std::tuple<Types...>;
    CSVReader(const std::string& filename) 
    {
        std::ifstream file(filename);
        if (!file.is_open()) 
        {
            throw std::runtime_error("Could not open file");
        }

        std::string line;
        if (!std::getline(file, line)) 
        {
            throw std::runtime_error("File is empty or cannot read the first line");
        }

        while (std::getline(file, line)) 
        {
            std::vector<std::string> row;
            std::istringstream stream(line);
            std::string cell;

            while (std::getline(stream, cell, ',')) 
            {
                row.push_back(cell);
            }

            if (!row.empty()) 
            {
                data.push_back(parseRow(row));
            }
        }

        file.close();
    }

public:
    void printData() const 
    {
        for (const auto& row : data) 
        {
            printRow(row);
            std::cout << std::endl;
        }
    }

    template<typename Func>
	void forEach(Func&& func) const
	{
		for (const auto& row : data)
		{
			std::apply(std::forward<Func>(func), row);
		}
	}

public:
    template<std::size_t... I>
    RowType parseRowImpl(const std::vector<std::string>& row, std::index_sequence<I...>) const 
    {
        return RowType{convertFromString<typename std::tuple_element<I, RowType>::type>(row[I])...};
    }

    RowType parseRow(const std::vector<std::string>& row) const 
    {
        return parseRowImpl(row, std::make_index_sequence<sizeof...(Types)>{});
    }

    template<std::size_t... I>
    void printRowImpl(const RowType& row, std::index_sequence<I...>) const 
    {
        ((std::cout << std::get<I>(row) << " "), ...);
    }

    void printRow(const RowType& row) const 
    {
        printRowImpl(row, std::make_index_sequence<sizeof...(Types)>{});
    }

private:
    std::vector<RowType> data;
};

template<typename... Types>
class CSVWriter 
{
public:
    using RowType = std::tuple<Types...>;

    CSVWriter(const std::wstring& filename) 
    {
        file.imbue(std::locale("Korean"));
        file.open(filename, std::ios::trunc);
        file.imbue(std::locale(""));
        if (!file.is_open()) 
        {
            throw std::runtime_error("Could not open file for writing");
        }
    }

    ~CSVWriter() 
    {
        if (file.is_open()) 
        {
            file.close();
        }
    }

    void addRow(const RowType& row) 
    {
        data.push_back(row);
    }

    void writeFile(std::vector<std::wstring> titleRow) 
    {
        if (!file.is_open()) 
        {
            throw std::runtime_error("File is not open");
        }
		for (const auto& title : titleRow)
		{
			file << title << ',';
		}
        file << '\n';

        for (const auto& row : data) 
        {
            writeRow(row);
            file << '\n';
        }
    }

private:
    std::wofstream file;
    std::vector<RowType> data;

    template<std::size_t... I>
    void writeRowImpl(const RowType& row, std::index_sequence<I...>) 
    {
        ((file << (I == 0 ? "" : ",") << std::get<I>(row)), ...);
    }

    void writeRow(const RowType& row) 
    {
        writeRowImpl(row, std::make_index_sequence<sizeof...(Types)>{});
    }
};