#pragma once

#include <UI/Components/BaseWidgets.h>

#include <QIcon>

#include <memory>

QT_BEGIN_NAMESPACE
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
QT_END_NAMESPACE

namespace UI::Components
{
enum class Style
{
    Flat,
    Raised,
    Outlined,
    Filled
};

enum class Size
{
    Small,
    Medium,
    Large,
    Auto
};

enum class Status
{
    None,
    Success,
    Info,
    Warning,
    Error
};

class CardWidget : public BaseWidget
{
public:
    explicit CardWidget(QWidget* pParent = nullptr);
    ~CardWidget() override;

    // Header configuration
    void SetTitle(const QString& title);
    void SetSubtitle(const QString& subtitle);
    void SetIcon(const QIcon& icon);
    void SetIcon(const QString& iconPath);
    void SetContent(QWidget* pWidget);
    void ClearContent();

    QString GetTitle() const;
    QString GetSubtitle() const;
    QIcon GetIcon() const;
    QWidget& GetContent() const;

    // Footer/Actions
    void AddAction(const QString& text, std::function<void()> callback);
    void AddAction(QWidget* pWidget);
    void ClearActions();

    // Style
    void SetStyle(Style style);
    void SetSize(Size size);

    Style GetStyle() const;
    Size GetSize() const;

    // Expandable behavior
    void SetExpandable(bool expandable);
    bool IsExpandable() const;

    void SetExpanded(bool expanded, bool animated = true);
    bool IsExpanded() const;
    void ToggleExpanded();

    // Interactive
    void SetClickable(bool clickable);
    void SetSelectable(bool selectable);
    void SetSelected(bool selected);

    bool IsClickable() const;
    bool IsSelectable() const;
    bool IsSelected() const;

    // Badge support
    void SetBadge(const QString& text);
    void SetBadge(int count);
    void ClearBadge();
    void SetBadgeColor(const QColor& color);

    // Status Indicator
    void SetStatus(Status status);
    Status GetStatus() const;

signals:
    void Clicked();
    void DoubleClicked();
    void ExpandedChanged(bool expanded);
    void SelectedChanged(bool selected);
    void ActionTriggered(const QString& actionName);

protected:
    void mousePressEvent(QMouseEvent* pEvent) override;
    void mouseReleaseEvent(QMouseEvent* pEvent) override;
    void mouseDoubleClickEvent(QMouseEvent* pEvent) override;
    void resizeEvent(QResizeEvent* pEvent) override;

private:
    void CreateUi_();
    void UpdateStyle_();
    void UpdateExpandedState_(bool animated):

    Q_OBJECT
    Q_PROPERTY(QString title READ GetTitle WRITE SetTitle)
    Q_PROPERTY(QString subtitle READ GetSubtitle WRITE SetSubtitle)
    Q_PROPERTY(bool expandable READ IsExpandable WRITE SetExpandable)
    Q_PROPERTY(bool expanded READ IsExpanded WRITE SetExpanded)

    class Impl;
    std::unique_ptr<Impl> pImpl;
};
} // namespace UI::Components
