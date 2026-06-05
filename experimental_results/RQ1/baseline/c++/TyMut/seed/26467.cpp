
#include <optional>
#include <variant>

class RefGradient {};
class RefPattern {};
class AffineTransform {};

class SourceBrush {
  public:
    struct Brush {
        struct LogicalGradient {
            RefGradient gradient;
            AffineTransform spaceTransform;
        };

        std::variant<LogicalGradient, RefPattern> brush;
    };

    void setGradient(RefGradient &&, const AffineTransform & spaceTransform = { });
    void setPattern(RefPattern &&);

  private:
    std::optional<Brush> m_brush;
};

void SourceBrush::setGradient(RefGradient&& gradient, const AffineTransform& spaceTransform)
{
    m_brush = { Brush::LogicalGradient { std::move(gradient), spaceTransform } };
}

void SourceBrush::setPattern(RefPattern&& pattern)
{
    m_brush = { std::move(pattern) };
}

