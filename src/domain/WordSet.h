#pragma once

#include <vector>
#include <utility>
#include <QString>
#include <QMetaType>

namespace top15words::domain
{

using WordSet = std::vector<std::pair<QString, std::uint32_t>>;

}

Q_DECLARE_METATYPE(top15words::domain::WordSet)
