#ifndef HUD_HPP
#define HUD_HPP

#include "pch.hpp"

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

class Hud : public Util::GameObject {
public:
    explicit Hud(const std::string &fontPath);

    void SetContent(const std::string &content);

private:
    std::shared_ptr<Util::Text> m_Text;
};

#endif
