#pragma once
#include<unordered_map>
#include<iostream>

#include"mpvcpp/mpvProperty.hpp"
#include"mpvcpp/mpvCommand.hpp"
#include"mpvcpp/mpvEvent.hpp"

using namespace std;
using namespace mpv;

std::string Path = "E:\\testVideo";

Handle handle(Handle::Init);

std::unordered_map<std::string, int> charMap = 
{
    { "quit", 0 },
    { "volume", 1 },
    { "speed", 2 },
    { "playlist", 3 }
};

static inline void outputPlaylist(const Node& node)
{
    for (auto& file : node.Array)
    {
        for (auto& it : file.Map)
        {
            if (it.first == "filename") {
                cout << it.second.String << "\n";
            }
        }
    }
    cout << "\n";
}

static inline int testVideo()
{
    Command::Sync(handle,
    {
        "loadfile",
        Path,
    });

    Property::Apply(handle, "geometry", "640x320");
    Property::Apply(handle, "volume", 40.0);
    Node node;

    bool quitState = false;
    std::string input;
    while (!quitState)
    {
        cin >> input;
        
        auto ret = charMap.find(input);
        if (ret == charMap.end())
            continue;
        switch(ret->second)
        {
        case 0:
            quitState = true;
            break;
        case 1:
            cout << "Press the number:";
            cin >> input;
            Property::Apply(handle, "volume",
                std::stod(input));
            break;
        case 2:
            cout << "Press the number:";
            cin >> input;
            Property::Apply(handle, "speed",
                std::stod(input));
            break;
        case 3:
            Property::Acquire(handle, "playlist", node);
            outputPlaylist(node);
            break;
        case 4:
            break;
        }
    }

    return 0;
}
