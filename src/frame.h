#pragma once
#include <memory>
#include <vector>
#include <draw.h>
#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif
#include <wx/wxprec.h>
#include <wx/frame.h>
#include <wx/timer.h>
#if defined(_MSC_VER)
#pragma warning(default : 4996)
#endif

class wxTimer;
namespace draw { class WXCanvas; }

class Frame : public wxFrame {

    wxDECLARE_EVENT_TABLE();

public:
    Frame(wxFrame* parent,
        const wxString& title = wxT("dogma"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE);
    virtual ~Frame() = default;

    Frame(const Frame&) = delete;
    Frame& operator = (const Frame&) = delete;

private:
    void onTimer(wxTimerEvent& event);
    void createShapes();

    std::unique_ptr<draw::WXCanvas> canvas_;
    std::unique_ptr<wxTimer> timer_;

    template <typename Ptr>
    struct Element {
        int32_t dir;
        Ptr ptr;
        Element(Ptr ptr) : dir(1), ptr(ptr) {};
    };

    std::vector<Element<draw::ShapePtr>> shapes_;
    std::vector<Element<draw::TextPtr>> texts_;
};
