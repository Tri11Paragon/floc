#pragma once
/*
 *  Copyright (C) 2024  Brett Terpstra
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BLT_FLOC_ANALYZER_H
#define BLT_FLOC_ANALYZER_H

#include <string>
#include <string_view>
#include <functional>
#include <vector>
#include <blt/std/hashmap.h>
#include <blt/std/types.h>

/*
 * This file provides an analyzer class for counting lines of code in a file along with a recursive parser for finding files
 * it also provides a utility class to build a structure which mimics the file system directories alongside relevant code
 *
 * maybe split this into a few header files?
 */

namespace blt::floc
{
    static inline const constexpr auto EMPTY_FUNCTION = [](std::string_view contents) {
        return true;
    };
    
    struct associated_file_t
    {
        std::string file_extension;
        std::string file_name;
        
        associated_file_t() = default;
        
        associated_file_t(std::string_view file_extension, std::string_view file_name): file_extension(file_extension), file_name(file_name)
        {}
    };
    
    class language_t
    {
        private:
            // extension -> file name
            blt::hashmap_t<std::string, std::string> language_files;
            std::function<bool(std::string_view)> matches_func;
        public:
            language_t(): matches_func(EMPTY_FUNCTION)
            {}
            
            explicit language_t(std::function<bool(std::string_view)> matches_func): matches_func(std::move(matches_func))
            {}
            
            inline bool matches(std::string_view file_extension, std::string_view file_contents)
            {
                return language_files.find(std::string(file_extension)) != language_files.end() && matches_func(file_contents);
            }
            
            inline language_t& add(const associated_file_t& file)
            {
                language_files[file.file_extension] = file.file_name;
                return *this;
            }
            
            inline language_t& add(std::string_view extension, std::string_view name)
            {
                return add(associated_file_t{extension, name});
            }
            
            inline language_t& add(const std::vector<std::string>& extensions, std::string_view name)
            {
                for (const auto& v : extensions)
                    add(v, name);
                return *this;
            }
    };
    
    static inline const std::vector<language_t> BASE_LANGUAGES = {
            language_t().add("c", "C Source").add("h", "C/C++ Header"),
            language_t().add({"cpp", "cxx", "cc"}, "C++ Source").add("h", "C/C++ Header").add("hpp", "C++ Header"),
            language_t().add("java", "Java Source"),
            language_t().add("rs", "Rust Source"),
            language_t().add({"py", "pyo"}, "Python Source"),
            language_t().add("rb", "Ruby Source"),
            language_t().add("pl", "Perl Source"),
            
    };
    
    
}

#endif // BLT_FLOC_ANALYZER_H
