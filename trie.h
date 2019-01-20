#pragma once


#include <stdexcept>
#include <initializer_list>
#include <map>
#include <string>
#include <string_view>
#include <variant>
#include <optional>
#include <utility>


namespace ws::structure {

    template <typename T>
    class Trie {
        using nodes_t   = std::map<char, Trie>;
        using size_type = typename nodes_t::size_type;

        using init_elem = std::pair<std::string_view, T>;
        using init_list = std::initializer_list<init_elem>;

        using return_t  = std::optional<T>;



        private:
            std::variant<nodes_t, T> children;
            bool is_leaf_node;



        public:

            Trie(const init_list& children_):
                children{nodes_t{}}
            {
                for (const auto& x: children_)
                    insert(x.first, x.second);
            }







        /* IMPLEMENTATION */

        private:

            // Inserts the nodes we need for a new string.
            void _insert(std::string_view str, T elem) {
                // Check if final character.
                if (str.empty()) {
                    is_leaf_node = true;
                    children = elem;
                    return;
                }


                auto search = str.at(0);
                auto& nodes = std::get<nodes_t>(children);


                // Check if character is in children. Create new node if not.
                if (nodes.find(search) == nodes.end())
                    nodes.emplace(search, Trie{});


                // Recurse with the rest of the string.
                nodes.at(search)._insert(str.substr(1, str.size()), elem);
            }



            // Only matches full words, not partial.
            bool _contains(std::string_view str) const {
                if (str.empty())
                    return is_leaf_node;


                auto search = str.at(0);
                const auto& nodes = std::get<nodes_t>(children);


                // Check if character is in children.
                if (nodes.find(search) == nodes.end())
                    return false;


                // Recurse with the rest of the string.
                return nodes.at(search)._contains(str.substr(1, str.size()));
            }



            // Can match partial strings.
            bool _partial(std::string_view str) const {
                if (str.empty())
                    return true;


                auto search = str.at(0);
                const auto& nodes = std::get<nodes_t>(children);


                // Check if character is in children.
                if (nodes.find(search) == nodes.end())
                    return false;


                // Recurse with the rest of the string.
                return nodes.at(search)._partial(str.substr(1, str.size()));
            }



            // Return T if string is contained.
            return_t _get(std::string_view str) const {
                if (str.empty()) {
                    if (is_leaf_node and std::holds_alternative<T>(children)) {
                        return std::get<T>(children);

                    } else {
                        return {};
                    }
                }


                auto search = str.at(0);
                const auto& nodes = std::get<nodes_t>(children);


                // Check if character is in children.
                if (nodes.find(search) == nodes.end())
                    return {};


                // Recurse with the rest of the string.
                return nodes.at(search)._get(str.substr(1, str.size()));
            }









        /* HELPERS */

        private:
            void checks(std::string_view str) const {
                if (str.empty())
                    throw std::length_error("usage of empty string.");
            }









        /* WRAPPERS */

        private:
            // Wraps _insert function, verify string is valid.
            void insert(std::string_view str, T elem) {
                checks(str);
                _insert(str, elem);
            }



        public:
            // Wraps _contains function, verify string is valid.
            bool contains(std::string_view str) const {
                checks(str);
                return _contains(str);
            }



            // Wraps _contains function, verify string is valid.
            bool partial(std::string_view str) const {
                checks(str);
                return _partial(str);
            }



            // Wraps _contains_part function, verify string is valid.
            return_t get(std::string_view str) const {
                checks(str);
                return _get(str);
            }
    };

}