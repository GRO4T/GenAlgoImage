#pragma once

#include "config.hpp"

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include <fstream>

#include "rapidjson/document.h"

namespace gro4t {

struct State {
    State(const ImageGeneratorConfig& config)
        : config(config),
          generation(0),
          current_circle_progress(0),
          current_circle(0),
          generated_image(config.image_props),
          last_sigma_evaluation(config.sigma_evaluation_frequency),
          sigma(config.base_sigma) {}
    int generation;
    int current_circle_progress;
    int current_circle;
    ImageGeneratorConfig config;
    double sigma;
    std::vector<bool> result_table;
    int last_sigma_evaluation;
    GeneratedImage generated_image;

    bool nextCircle() { return current_circle_progress == config.next_circle_frequency; }
    void nextGeneration() {
        ++generation;
        ++current_circle_progress;
    }

    void saveToJSON(const std::string& path) {
        using namespace rapidjson;
        Document document;
        document.SetObject();
        auto& alloc = document.GetAllocator();
        document.AddMember("generation", generation, alloc);
        document.AddMember("current_circle", current_circle, alloc);
        Value generated_image_json;
        generated_image_json.SetObject();
        generated_image_json.AddMember("id", generated_image.getId(), alloc);
        Value json_circle_prop_list;
        json_circle_prop_list.SetArray();
        for (const auto& circle_prop : generated_image.getCirclePropList()) {
            Value json_circle_prop;
            json_circle_prop.SetObject();
            json_circle_prop.AddMember("radius", circle_prop.radius, alloc);
            Value json_position;
            json_position.SetObject();
            json_position.AddMember("x", circle_prop.position.x, alloc);
            json_position.AddMember("y", circle_prop.position.y, alloc);
            json_circle_prop.AddMember("position", json_position, alloc);
            Value json_color;
            json_color.SetObject();
            json_color.AddMember("r", circle_prop.color.r, alloc);
            json_color.AddMember("g", circle_prop.color.g, alloc);
            json_color.AddMember("b", circle_prop.color.b, alloc);
            json_circle_prop.AddMember("color", json_color, alloc);
            json_circle_prop_list.PushBack(json_circle_prop, alloc);
        }
        generated_image_json.AddMember("circle_prop_list", json_circle_prop_list, alloc);
        document.AddMember("generated_image", generated_image_json, alloc);

        // save document
        std::ofstream ofs(path);
        OStreamWrapper osw(ofs);
        Writer<OStreamWrapper> writer(osw);
        document.Accept(writer);
        std::cout << "Saved state to " << path << std::endl;
    }

    void loadFromJSON(const std::string& path) {

    }
};

}