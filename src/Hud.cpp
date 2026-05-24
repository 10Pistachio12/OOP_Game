#include "Hud.hpp"

#include "config.hpp"

Hud::Hud(const std::string &fontPath)
    : m_Text(std::make_shared<Util::Text>(fontPath, 22, "",
                                          Util::Color(238, 238, 238))) {
    SetDrawable(m_Text);
    SetZIndex(100.0F);
}

void Hud::SetContent(const std::string &content) {
    m_Text->SetText(content);

    const glm::vec2 size = m_Text->GetSize();
    m_Transform.translation = {
        -static_cast<float>(WINDOW_WIDTH) * 0.5F + size.x * 0.5F + 16.0F,
        static_cast<float>(WINDOW_HEIGHT) * 0.5F - size.y * 0.5F - 16.0F,
    };
}
