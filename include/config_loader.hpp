#pragma once

#include <fstream>
#include <sstream>
#include <variant>

#include "image_generator.hpp"

namespace gro4t {

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

using StringConstIt = std::string::const_iterator;

enum ConfigValueType { STRING, LOGICAL, INTEGER, DECIMAL };
using ConfigValue = std::variant<std::string, bool, int, double>;
using ConfigEntry = std::pair<std::string, ConfigValue>;

std::ostream& operator<<(std::ostream& os, const ConfigValueType config_value_type) {
    switch (config_value_type) {
        case ConfigValueType::STRING:
            os << "std::string";
            break;
        case ConfigValueType::LOGICAL:
            os << "bool";
            break;
        case ConfigValueType::INTEGER:
            os << "int";
            break;
        case ConfigValueType::DECIMAL:
            os << "double";
            break;
    }
    return os;
}

template <typename T>
T getConfigValue(const ConfigEntry & entry, ConfigValueType expectedType) {
    try {
        return std::get<T>(entry.second);
    } catch (std::exception& e) {
        std::stringstream ss;
        ss << "Config entry named " << entry.first << " expected value of type " << expectedType
           << " got " << (ConfigValueType)entry.second.index();
        throw std::runtime_error(ss.str());
    }
}

std::ostream& operator<<(std::ostream& os, const ConfigValue& value) {
    std::visit(overload{
        [&os](const std::string& s) { os << s; },
        [&os](double d) { os << d; },
        [&os](int i) { os << i; },
        [&os](bool b) { os << b; }
    }, value);
    return os;
}

class ConfigLoader {
public:
    static ImageGeneratorConfig loadConfig(const std::string& path) {
        std::cout << "Started loading config..." << std::endl;
        std::map<std::string, ConfigValue> config;
        std::ifstream config_file(path);
        std::string line;
        while (std::getline(config_file, line)) {
            config.insert(parseEntry(line));
        }
        auto image_generator_config = createImageGeneratorConfig(config);
        std::cout << "Finished loading config..." << std::endl;
        return image_generator_config;
    }

private:
    static ConfigEntry parseEntry(const std::string& line) {
        std::string name;
        ConfigValue value;
        auto line_it = line.begin();
        auto line_end = line.end();

        while (*line_it != '=') {
            name += *line_it++;
        }

        ++line_it;

        if (*line_it == 't' || *line_it == 'f') {
            return {name, parseLogicalValue(line_it, line_end)};
        } else if (*line_it == '"') {
            return {name, parseStringValue(line_it, line_end)};
        } else if (isdigit(*line_it)) {
            auto integer_or_decimal = parseIntegerOrDecimalValue(line_it, line_end);
            if (integer_or_decimal.index() == 0)
                return {name, std::get<int>(integer_or_decimal)};
            else
                return {name, std::get<double>(integer_or_decimal)};
        }
        throw std::runtime_error(
            "Error parsing config entry! Cannot recognize value type."
            "First character not expected.");
    }

    static std::variant<int, double> parseIntegerOrDecimalValue(StringConstIt& line_it,
                                                                const StringConstIt& line_end) {
        std::string value;
        bool is_integer = true;
        value += *line_it;
        while (++line_it != line_end) {
            if (*line_it == '.')
                is_integer = false;
            value += *line_it;
        }
        if (is_integer) {
            int int_value = std::stoi(value);
            return int_value;
        }
        else {
            double decimal_value = std::stod(value);
            return decimal_value;
        }
    }

    static std::string parseStringValue(StringConstIt& line_it, const StringConstIt& line_end) {
        std::string value;
        while (*(++line_it) != '"') {
            if (line_it == line_end) throw std::runtime_error("No closing parenthesis");
            value += *line_it;
        }
        if (++line_it != line_end)
            throw std::runtime_error("Error. Found characters after closing parenthesis");
        return value;
    }

    static bool parseLogicalValue(StringConstIt& line_it, const StringConstIt& line_end) {
        throw std::runtime_error("Not implemented");
    }

    static ImageGeneratorConfig createImageGeneratorConfig(
        std::map<std::string, ConfigValue>& config) {
        GeneratedImageProps image_props;
        ImageGeneratorConfig image_generator_config;
        for (const auto& entry : config) {
            auto& name = entry.first;
            auto& value = entry.second;
            std::cout << name << "=" << value << std::endl;
            if (name == "max_circles")
                image_props.max_circles = getConfigValue<int>(entry, ConfigValueType::INTEGER);
            else if (name == "sigma_evaluation_frequency")
                image_generator_config.sigma_evaluation_frequency = getConfigValue<int>(entry, ConfigValueType::INTEGER);
            else if (name == "next_circle_frequency")
                image_generator_config.next_circle_frequency = getConfigValue<int>(entry, ConfigValueType::INTEGER);
            else if (name == "display_info_frequency")
                image_generator_config.display_info_frequency = getConfigValue<int>(entry, ConfigValueType::INTEGER);
            else if (name == "max_radius")
                image_props.max_radius = getConfigValue<double>(entry, ConfigValueType::DECIMAL);
            else if (name == "min_radius")
                image_props.min_radius = getConfigValue<double>(entry, ConfigValueType::DECIMAL);
            else if (name == "base_sigma")
                image_generator_config.base_sigma = getConfigValue<double>(entry, ConfigValueType::DECIMAL);
            else if (name == "original_image_path") {
                auto original_image_path = getConfigValue<std::string>(entry, ConfigValueType::STRING);
                sf::Image original_image;
                if (!original_image.loadFromFile(original_image_path))
                    throw std::runtime_error("error loading original image");
                auto original_image_size = original_image.getSize();
                image_props.width = original_image_size.x;
                image_props.height = original_image_size.y;
                image_generator_config.original_image = original_image;
            }
        }
        image_generator_config.image_props = image_props;
        return image_generator_config;
    }
};

}  // namespace gro4t
