#include "canvas.h"
#include <algorithm>
#include <wx/dcclient.h>

namespace draw {

wxBEGIN_EVENT_TABLE(WXCanvas, wxGLCanvas)
EVT_SIZE(WXCanvas::onSize)
EVT_PAINT(WXCanvas::onPaint)
wxEND_EVENT_TABLE()

WXCanvas::WXCanvas(wxWindow* parent, wxWindowID id, int* attribs) :
    wxGLCanvas(parent, id, attribs, wxDefaultPosition,
        wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {

    using namespace draw;

    renderer_ = makeRenderer(std::move(std::unique_ptr<WXContext>(new WXContext(*this))));
    font_ = renderer_->makeFont("cour.ttf", 10);
}

ShapePtr WXCanvas::addRect(const Point& position, const Size& size, const Color& color) {

    auto obj = renderer_->makeRect();
    obj->position(position);
    obj->size(size);
    obj->color(color);
    obj->visibility(true);
    return obj;
}

TextPtr WXCanvas::addText(const Point& position, const Color& color, const wchar_t* text) {

    auto obj = renderer_->makeText();
    obj->position(position);
    obj->font(font_);
    obj->color(color);
    obj->text(text);
    obj->visibility(true);
    return obj;
}

void WXCanvas::repaint() {

    if (!IsShownOnScreen())
        return;

    wxClientDC(this);
    doRepaint();
}

void WXCanvas::onSize(wxSizeEvent& event) {

    const auto& sz = event.GetSize();
    renderer_->resize(draw::Size((uint32_t)sz.x, (uint32_t)sz.y));
}

void WXCanvas::onPaint(wxPaintEvent&) {

    if (!IsShownOnScreen())
        return;

    wxPaintDC(this);
    doRepaint();
}

void WXCanvas::doRepaint() {

    renderer_->draw(clearColor_);
    SwapBuffers();
}

} // namespace draw {
