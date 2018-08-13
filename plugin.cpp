/**
 * Evoplex <https://evoplex.org>
 * Copyright (C) 2016-present
 */

#include "plugin.h"

namespace evoplex {

bool MinimalModel::init()
{
    m_stateAttrId = node(0).attrs().indexOf("state");
    return m_stateAttrId >= 0;
}

bool MinimalModel::algorithmStep()
{
    std::vector<bool> nextStates;
    nextStates.reserve(nodes().size());

    for (Node node : nodes()) {
        bool left_cell, right_cell;
        if (node.id() == 0) { // Edge case 1
            // Toroidal
            left_cell = nodes().at(nodes().size()-1).attr(m_stateAttrId).toBool();
            right_cell = nodes().at(1).attr(m_stateAttrId).toBool();
        } else if (node.id() == (nodes().size()-1)) {
            left_cell = nodes().at(nodes().size()-2).attr(m_stateAttrId).toBool();
            right_cell = nodes().at(0).attr(m_stateAttrId).toBool();
        } else {
//            left_cell XOR (central_cell OR right_cell)
            left_cell = nodes().at(node.id()-1).attr(m_stateAttrId).toBool();
            right_cell = nodes().at(node.id()+1).attr(m_stateAttrId).toBool();
        }

        bool central_cell = node.attr(m_stateAttrId).toBool();

        bool central_cell_next_state = left_cell ^ (central_cell || right_cell);
        nextStates.emplace_back(central_cell_next_state);
    }
    return true;
}

} // evoplex
REGISTER_PLUGIN(MinimalModel)
#include "plugin.moc"
