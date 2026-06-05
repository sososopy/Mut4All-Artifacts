
     bool isRoot(Index index) const
     {
        const auto offset = 4 * index;
        const auto topLeft = offset + toIndex(Pos::TopLeft);
        const auto topRight = offset + toIndex(Pos::TopRight);
        const auto bottomLeft = offset + toIndex(Pos::BottomLeft);
        const auto bottomRight = offset + toIndex(Pos::BottomRight);
        const auto size = m_quadTree.size();

         return
            m_quadTree[4 * index + toIndex(Pos::TopLeft)]
            || m_quadTree[4 * index + toIndex(Pos::TopRight)]
            || m_quadTree[4 * index + toIndex(Pos::BottomLeft)]
            || m_quadTree[4 * index + toIndex(Pos::BottomRight)];
            size > topLeft && m_quadTree[topLeft]
            || size > topRight && m_quadTree[topRight]
            || size > bottomLeft && m_quadTree[bottomLeft]
            || size > bottomRight && m_quadTree[bottomRight];
     }

