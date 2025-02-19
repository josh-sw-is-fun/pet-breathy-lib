#pragma once

namespace pb {

class PointGroupStatus {
public:
    void Reset() {
        m_outOfFrame = false;
        m_jumped = false;
    }
    
    void PointWentOutOfFrame() { m_outOfFrame = true; }
    bool DidPointGoOutOfFrame() const { return m_outOfFrame; }
    
    void PointJumped() { m_jumped = true; }
    bool DidPointJump() const { return m_jumped; }

private:
    bool m_outOfFrame{false};
    bool m_jumped{false};
};

}
