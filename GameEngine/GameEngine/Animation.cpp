#include "Animation.h"

void Animation::CreateNewCereal(std::filesystem::path localfile)
{
    std::ofstream ofs(localfile.string().c_str(), std::ios::binary);
    cereal::BinaryOutputArchive serialization(ofs);
    serialization(local, name, sampling_rate, sequence);

}

void Animation::LoadDataFromCereal(std::filesystem::path localfile)
{
    std::ifstream ifs(localfile.string().c_str(), std::ios::binary);
    cereal::BinaryInputArchive deserialization(ifs);
    deserialization(local, name, sampling_rate, sequence);
}

void Animation::SetName(std::string name)
{
    this->name = name;
}
