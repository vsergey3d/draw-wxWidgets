#pragma once
#include <memory>
#include <vector>
#include <draw.h>
#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif
#include <wx/wxprec.h>
#include <wx/glcanvas.h>
#if defined(_MSC_VER)
#pragma warning(default : 4996)
#endif

namespace draw {

class WXContext : public Context {

public:
    WXContext(wxGLCanvas& canvas) :
        glCanvas_(canvas), glContext_(new wxGLContext(&canvas)) {}
    virtual ~WXContext() = default;

    virtual void setCurrent() final { glCanvas_.SetCurrent(*glContext_.get()); }

private:
    wxGLCanvas& glCanvas_;
    std::unique_ptr<wxGLContext> glContext_;
};

class WXCanvas final : public wxGLCanvas {

wxDECLARE_EVENT_TABLE();

public:
    WXCanvas(wxWindow* parent, wxWindowID id = wxID_ANY, int* attribs = nullptr);
    virtual ~WXCanvas() = default;

    WXCanvas(const WXCanvas &) = delete;
    WXCanvas & operator = (const WXCanvas &) = delete;

    ShapePtr addRect(const Point& position, const Size& size, const Color& color);
    TextPtr addText(const Point& position, const Color& color, const wchar_t* text);

    void repaint();

private:
    void onSize(wxSizeEvent& event);
    void onPaint(wxPaintEvent&);
    void doRepaint();

    RendererPtr renderer_;
    FontPtr font_;
    Size screenSize_ {0, 0};
    Color clearColor_ {0.8f, 0.8f, 0.8f, 1.0f};
};

}; // namespace draw
