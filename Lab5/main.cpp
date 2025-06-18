#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <vector>
#include <tuple>
#include <queue>
#include <limits>
#include <string>
#include <algorithm>
#include <windows.h>
#include <chrono>
#include <set>
#include "tinyxml2.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef std::pair<double, double> Coord;
typedef std::tuple<Coord, Coord, std::string> EdgeItem;
typedef std::map<Coord, std::vector<std::pair<Coord, double>>> Graph;

std::string utf8_to_cp1251(const std::string& utf8_str) {
    if (utf8_str.empty()) return "";
    int wide_len = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    if (wide_len == 0) return "";
    std::wstring wide_str(wide_len, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], wide_len);
    int cp1251_len = WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (cp1251_len == 0) return "";
    std::string cp1251_str(cp1251_len, 0);
    WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, &cp1251_str[0], cp1251_len, nullptr, nullptr);
    return cp1251_str;
}

double haversine(const Coord& coord1, const Coord& coord2) {
    double lon1 = coord1.first, lat1 = coord1.second;
    double lon2 = coord2.first, lat2 = coord2.second;
    double R = 6371.0;

    double phi1 = lat1 * M_PI / 180.0;
    double phi2 = lat2 * M_PI / 180.0;
    double dphi = (lat2 - lat1) * M_PI / 180.0;
    double dlambda = (lon2 - lon1) * M_PI / 180.0;

    double a = sin(dphi / 2.0) * sin(dphi / 2.0) + cos(phi1) * cos(phi2) * sin(dlambda / 2.0) * sin(dlambda / 2.0);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c;
}

std::tuple<std::vector<Coord>, double, std::vector<std::string>> dijkstra(const Graph& graph, const Coord& start, const Coord& end) {
    typedef std::pair<double, Coord> QueueItem;
    auto cmp = [](const QueueItem& left, const QueueItem& right) { return left.first > right.first; };
    std::priority_queue<QueueItem, std::vector<QueueItem>, decltype(cmp)> priority_queue(cmp);

    std::map<Coord, std::pair<Coord, double>> shortest_paths;

    std::set<Coord> visited;

    shortest_paths[start] = { {0.0, 0.0}, 0.0 };
    priority_queue.push({ 0.0, start });

    while (!priority_queue.empty()) {
        double current_distance = priority_queue.top().first;
        Coord current_node = priority_queue.top().second;
        priority_queue.pop();

        if (visited.count(current_node)) {
            continue;
        }
        visited.insert(current_node);

        if (current_node == end) {
            break;
        }

        if (graph.count(current_node) == 0) continue;

        for (const auto& neighbor_pair : graph.at(current_node)) {
            Coord neighbor = neighbor_pair.first;
            double distance_to_neighbor = neighbor_pair.second;
            double total_distance = current_distance + distance_to_neighbor;

            if (shortest_paths.find(neighbor) == shortest_paths.end() || total_distance < shortest_paths[neighbor].second) {
                shortest_paths[neighbor] = { current_node, total_distance };
                priority_queue.push({ total_distance, neighbor });
            }
        }
    }

    std::vector<Coord> path;
    if (shortest_paths.find(end) == shortest_paths.end()) {
        return { {}, std::numeric_limits<double>::infinity(), {} };
    }

    Coord current = end;
    while (current != start) {
        path.push_back(current);
        if (shortest_paths.find(current) == shortest_paths.end()) {
            return { {}, std::numeric_limits<double>::infinity(), {} };
        }
        current = shortest_paths[current].first;
        if (path.size() > graph.size()) {
            return { {}, std::numeric_limits<double>::infinity(), {} };
        }
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return { path, shortest_paths[end].second, {} };
}

std::tuple<std::map<std::string, Coord>, std::vector<EdgeItem>> read_graphml(const std::string& file_path) {
    std::map<std::string, Coord> nodes;
    std::vector<EdgeItem> edges;
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << file_path << std::endl;
        return { {}, {} };
    }

    tinyxml2::XMLElement* graphml = doc.FirstChildElement("graphml");
    if (!graphml) return { {}, {} };
    tinyxml2::XMLElement* graph = graphml->FirstChildElement("graph");
    if (!graph) return { {}, {} };

    for (tinyxml2::XMLElement* node = graph->FirstChildElement("node"); node; node = node->NextSiblingElement("node")) {
        const char* id = node->Attribute("id");
        if (!id) continue;
        double x = 0, y = 0;
        bool has_coords = false;
        for (tinyxml2::XMLElement* data = node->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            const char* key = data->Attribute("key");
            if (key) {
                if (std::string(key) == "d5") x = data->DoubleText();
                else if (std::string(key) == "d4") { y = data->DoubleText(); has_coords = true; }
            }
        }
        if (has_coords) nodes[id] = { x, y };
    }

    for (tinyxml2::XMLElement* edge = graph->FirstChildElement("edge"); edge; edge = edge->NextSiblingElement("edge")) {
        const char* source_id = edge->Attribute("source");
        const char* target_id = edge->Attribute("target");
        if (!source_id || !target_id) continue;
        std::string street_name = "";
        for (tinyxml2::XMLElement* data = edge->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            const char* key = data->Attribute("key");
            if (key && std::string(key) == "d14" && data->GetText()) {
                street_name = data->GetText();
            }
        }
        if (nodes.count(source_id) && nodes.count(target_id)) {
            edges.emplace_back(nodes[source_id], nodes[target_id], street_name);
        }
    }

    std::cout << "Количество вершин " << nodes.size() << " ребер " << edges.size() << std::endl;
    return { nodes, edges };
}

Graph build_graph(const std::vector<EdgeItem>& edges) {
    Graph graph;
    for (const auto& edge : edges) {
        Coord start, end;
        std::string street;
        std::tie(start, end, street) = edge;
        double dist = haversine(start, end);
        graph[start].push_back({ end, dist });
        graph[end].push_back({ start, dist });
    }
    return graph;
}

std::pair<int, std::string> find_street_index(const std::vector<EdgeItem>& edges, const std::string& street_name_query) {
    std::string query_lower = street_name_query;
    std::transform(query_lower.begin(), query_lower.end(), query_lower.begin(), [](unsigned char c) { return std::tolower(c); });

    for (size_t i = 0; i < edges.size(); ++i) {
        std::string name = std::get<2>(edges[i]);
        if (!name.empty()) {
            std::string name_lower = name;
            std::transform(name_lower.begin(), name_lower.end(), name_lower.begin(), [](unsigned char c) { return std::tolower(c); });
            if (name_lower == query_lower) {
                return { static_cast<int>(i), name };
            }
        }
    }
    return { -1, "" };
}

void visualize_path_with_network(const std::map<std::string, Coord>& nodes, const std::vector<EdgeItem>& edges, const std::vector<Coord>& path, const std::vector<std::string>& street_names) {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Кратчайший маршрут");

    double minX = std::numeric_limits<double>::max(), maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max(), maxY = std::numeric_limits<double>::lowest();
    if (nodes.empty()) return;

    for (const auto& node_pair : nodes) {
        minX = std::min(minX, node_pair.second.first);
        maxX = std::max(maxX, node_pair.second.first);
        minY = std::min(minY, node_pair.second.second);
        maxY = std::max(maxY, node_pair.second.second);
    }

    auto transform_coord = [&](const Coord& c) -> sf::Vector2f {
        double scaleX = (window.getSize().x - 40.0) / (maxX - minX + 1e-6);
        double scaleY = (window.getSize().y - 40.0) / (maxY - minY + 1e-6);
        float x = static_cast<float>((c.first - minX) * scaleX + 20);
        float y = static_cast<float>(window.getSize().y - ((c.second - minY) * scaleY + 20));
        return sf::Vector2f(x, y);
        };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(sf::Color::White);
        for (const auto& edge : edges) {
            sf::Vertex line[] = {
                sf::Vertex(transform_coord(std::get<0>(edge)), sf::Color(200, 200, 200)),
                sf::Vertex(transform_coord(std::get<1>(edge)), sf::Color(200, 200, 200)) };
            window.draw(line, 2, sf::Lines);
        }
        if (path.size() > 1) {
            for (size_t i = 0; i < path.size() - 1; ++i) {
                sf::Vertex line[] = {
                    sf::Vertex(transform_coord(path[i]), sf::Color::Red),
                    sf::Vertex(transform_coord(path[i + 1]), sf::Color::Red) };
                window.draw(line, 2, sf::Lines);
            }
        }
        window.display();
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    auto [nodes, edges] = read_graphml("bucharest_road_network.graphml");

    std::string start_street_query = "Bulevardul Iuliu Maniu";
    std::string end_street_query = "Bulevardul Pache Protopopescu";

    auto start_result = find_street_index(edges, start_street_query);
    auto end_result = find_street_index(edges, end_street_query);

    if (start_result.first == -1 || end_result.first == -1) {
        std::cout << "Не удалось найти заданную улицу для начала или конца маршрута" << std::endl;
    }
    else {
        std::cout << "Стартовая улица: " << utf8_to_cp1251(start_result.second) << std::endl;
        std::cout << "Конечная улица: " << utf8_to_cp1251(end_result.second) << std::endl;

        Coord start_node = std::get<0>(edges[start_result.first]);
        Coord end_node = std::get<1>(edges[end_result.first]);

        Graph graph = build_graph(edges);
        std::vector<Coord> path;
        double distance;
        std::vector<std::string> street_names;

        auto t1 = std::chrono::high_resolution_clock::now();
        std::tie(path, distance, street_names) = dijkstra(graph, start_node, end_node);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms_double = t2 - t1;

        if (path.empty()) {
            std::cout << "Путь не найден" << std::endl;
        }
        else {
            std::cout << "Найден путь длиной " << distance << " км" << std::endl;
            std::cout << "Время работы алгоритма: " << ms_double.count() << "ms" << std::endl;

            std::cout << "Улицы на пути: ";
            bool first = true;
            for (const auto& s : street_names) {
                if (!s.empty()) {
                    if (!first) {
                        std::cout << ", ";
                    }
                    std::cout << utf8_to_cp1251(s);
                    first = false;
                }
            }
            std::cout << std::endl;

            visualize_path_with_network(nodes, edges, path, street_names);
        }
    }

    std::cout << "Нажмите Enter для выхода...";
    std::cin.get();
    return 0;
}