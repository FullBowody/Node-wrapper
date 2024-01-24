#include <iostream>
#include <windows.h>
#include "Engine/EngineLoader.hpp"

const std::string ENGINE_PATH = "C:/Users/furwa/Documents/FurWaz/FullBowody/Engine/build/Debug/Engine.dll";

int main(int argc, char const *argv[])
{
    EngineLoader loader(ENGINE_PATH);
    Engine* engine = loader.createEngine();

    engine->Run();

    loader.destroyEngine(engine);
}
