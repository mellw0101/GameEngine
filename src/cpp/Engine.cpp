#include "../include/Engine.hpp"

using namespace std;

auto Engine::Object::init ( const ObjectData& data ) -> void
{
    this->data = data;
}
