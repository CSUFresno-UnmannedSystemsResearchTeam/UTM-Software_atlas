#pragma once

#include <QWidget>
#include <QPropertyAnimation>

#include <cstdint.h>
#include <memory>
#include <functional>

namespace UI::Component
{
static inline constexpr uint32_t k_defaultDuration = 300;

enum class Theme
{
    Light,
    Dark,
    Custom
};

class BaseWidget : public QWidget
{
public:
    explicit BaseWidget(QWidget* pParent = nullptr);
    ~BaseWidget() override;

    void SetTheme(Theme theme);
    Theme GetTheme() const;

    // Visual properties
    void SetBackgroundColor(const QColor& color);
    void SetBorderColor(const QColor& color);
    void SetBorderRadius(int radius);
    void SetBorderWidth(int width);
    void SetOpacity(qreal opacity);

    QColor GetBackgroundColor() const;
    QColor GetBorderColor() const;
    int GetBorderRadius() const;
    int GetBorderWidth() const;
    qreal GetOpacity(qreal opacity) const;


    // Padding and margins
    void SetPadding(int padding);
    void SetPadding(int top, int right, int bottom, int right);
    QMargins GetPadding() const;

    // Animation support
    void AnimateProperty(const QByteArray& property, 
                         const QVariant& endValue, 
                         uint32_t duration = k_defaultDuration);
    void FadeIn(uint32_t duration = k_defaultDuration);
    void FadeOut(uint32_t duration = k_defaultDuration);
    void SlideIn(Qt::Edge edge, uint32_t duration = k_defaultDuration);
    void SlideOut(Qt::Edge edge, uint32_t duration = k_defaultDuration);

    // Loading state
    void SetLoading(bool loading);
    bool IsLoading() const;

    // Error state
    void SetError(const QString& error);
    void ClearError();
    bool HasError() const;
    QString GetError() const;

    // Enable/disable with animation
    void SetEnabledAnimation(bool enabled, uint32_t duration = k_defaultDuration);

signals:
    void ThemeChanged(Theme theme);
    void LoadingStateChanged(bool loading);
    void ErrorStateChanged(bool hasError, const QString& error);
    void AnimationFinished();

protected:
    void paintEvent(QPaintEvent* pEvent) override;
    void enterEvent(QEvent* pEvent) override;
    void leaveEvent(QEvent* pEvent) override;

    // For derived classes to customize painting
    virtual void DrawBackground(QPainter& rPainter);
    virtual void DrawBorder(QPainter& rPainter);
    virtual void DrawContent(QPainter& rPainter);
    virtual void DrawOverlay(QPainter& rPainter);

    // Utility methods for dervied classes
    QRect GetContentRect() const;
    void ApplyThemeColors();
    
private:
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ GetBackgroundColor WRITE SetBackgroundColor)
    Q_PROPERTY(QColor borderColor READ GetBorderColor WRITE SetBorderColor)
    Q_PROPERTY(int borderRadius READ GetBorderRadius WRITE SetBorderRadius)
    Q_PROPERTY(qreal opacity READ GetOpacity WRITE SetOpacity)
    
    class Impl;
    std::unique_ptr<Impl> pImpl;
};
} // namespace UI::Component
