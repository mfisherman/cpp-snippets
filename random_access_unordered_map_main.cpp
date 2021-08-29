#include <iostream>
#include <vector>
#include <unordered_map>
#include <optional>
#include <random>
#include <assert.h>

// An std::map provides the follownig properties:
// - keys are unique
// - elements in a map are always sorted by its key following a specific strict weak ordering
//   criterion indicated by its internal comparison object (of type Compare).
// - insert, find, and remove require O(log(n)) runtime in best, worst and average case
// A std::unordered_map provides the following properties:
// - keys are unique
// - elements in the unordered_map are not sorted in any particular order with respect to either their key or mapped values
// - insert, and remove, find require O(1) runtime in best and average case, O(n) in worst case

// Both containers do not provide a way to access a random element in constant time.
// This is what the following implementation does:
// - It relies on an std::unordered_map and provides the same runtime for insert, remove and find
// - It also provides O(1) time to access a random element. This can come in handy if you need to draw a random subset

template <class K, class V>
class RandomAccessUnorderedMap
{
private:
    std::optional<uint32_t> find_index(const K &key)
    {
        auto it = index_map.find(key);
        if (it == index_map.end())
        {
            return std::nullopt;
        }
        return it->second;
    }

    struct Element
    {
        K key;
        V value;
    };

    void print_element_set()
    {
        for (const Element &element : element_set)
        {
            std::cout << "(" << element.key << " " << element.value << ") ";
        }

        std::cout << std::endl;
    }

    void print_index_map()
    {
        for (const auto &elem : index_map)
        {
            std::cout << "(" << elem.first << " " << elem.second << ") ";
        }
        std::cout << std::endl;
    }

public:
    RandomAccessUnorderedMap()
    {
        std::random_device rd;
        random_number_generator = std::mt19937(rd());
    }

    ~RandomAccessUnorderedMap() = default;

    std::optional<V> find(const K &key)
    {
        auto index_optional = find_index(key);
        if (index_optional.has_value())
        {
            return element_set[index_optional.value()].value;
        }
        return std::nullopt;
    }

    void remove(const K &key)
    {
        // Checks, if the key exists.
        auto index_optional = find_index(key);
        if (index_optional.has_value())
        {

            // Removes the element from the key set.
            const uint32_t index = index_optional.value();
            if (index == element_set.size() - 1)
            {
                element_set.pop_back();
            }
            else
            {
                // Swaps positions and removes the element from the key set.
                std::swap(element_set[index], element_set[element_set.size() - 1]);
                element_set.pop_back();

                // Now, we need to update the index map, since the swapped element has changed its position.
                // Note that std::map::insert ignores the update if the element already exists, so the bracket operator is used.
                const K &local_key = element_set[index].key;
                index_map[local_key] = index;
            }

            index_map.erase(key);
        }
    }

    void insert(const K &key, const V &value)
    {
        // Removes the value, if it already exists.
        remove(key);

        // Inserts the data at the end of the element set.
        Element element{key, value};
        element_set.emplace_back(element);
        index_map.insert(std::pair<K, uint32_t>(key, element_set.size() - 1));
    }

    K random_key()
    {
        std::uniform_int_distribution<> distrib(0, element_set.size() - 1);
        int random_index = distrib(random_number_generator);
        return element_set[random_index].key;
    }

    std::vector<Element> element_set;
    std::unordered_map<K, uint32_t> index_map;
    std::mt19937 random_number_generator;
};

int main(int argc, char **argv)
{
    RandomAccessUnorderedMap<std::string, std::string> map;
    assert(!map.find("Hello World").has_value());
    map.insert("hello", "world");
    assert(map.find("hello").value() == "world");
    std::cout << "Value of hello: " << map.find("hello").value() << std::endl;

    map.insert("hello2", "world2");
    map.insert("hello3", "world3");
    std::cout << "Value of hello3: " << map.find("hello3").value() << std::endl;
    assert(map.find("hello").has_value() && map.find("hello2").has_value() && map.find("hello3").has_value());
    std::cout << "Random key: " << map.random_key() << std::endl;

    map.remove("hello");
    map.remove("blubsi");
    assert(map.find("hello2").value() == "world2" && map.find("hello3").value() == "world3" && !map.find("hello").has_value());
    std::cout << "Value of hello3: " << map.find("hello3").value() << std::endl;

    map.insert("hello3", "world4");
    assert(map.find("hello3").value() == "world4");
    std::cout << "Value of hello3: " << map.find("hello3").value() << std::endl;

    return 0;
}