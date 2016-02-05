#include "frame.h"
#include <canvas.h>
#include <wx/timer.h>
#include <algorithm>
#include <chrono>

static const auto kTimerID = 1;
static const auto kFrameMinDuration = (std::chrono::milliseconds(1000) / 30 /*FPS*/).count();

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
EVT_TIMER(kTimerID, Frame::onTimer)
wxEND_EVENT_TABLE()

Frame::Frame(wxFrame* parent, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style) :
        wxFrame(parent, wxID_ANY, title, pos, size, style) {

    canvas_ = std::unique_ptr<draw::WXCanvas>(new draw::WXCanvas(this));
    createShapes();

    timer_ = std::unique_ptr<wxTimer>(new wxTimer(this, kTimerID));
    timer_->Start(kFrameMinDuration);

    Show(true);
    Raise();
}

template <typename Container>
void move(Container& container, int32_t xOffset, int32_t xMax) {

    for (auto& elem : container) {
        auto& dir = elem.dir;
        auto width = elem.ptr->bounds().right - elem.ptr->bounds().left;
        auto pos = elem.ptr->position();

        if (dir > 0 && pos.x >= xMax - width ||
            dir < 0 && pos.x <= 0)
            dir *= -1;

        pos.x += dir * xOffset;
        elem.ptr->position(pos);
    }
}

void Frame::onTimer(wxTimerEvent&) {

    using namespace std::chrono;

    static auto lastTime = high_resolution_clock::now();
    auto curTime = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(curTime - lastTime).count();
    lastTime = curTime;
    auto factor = float(kFrameMinDuration) / std::max(kFrameMinDuration, duration);

    const auto kXMax = 1500;
    move(shapes_, factor * 10, kXMax);
    move(texts_, factor * 5, kXMax);

    canvas_->repaint();
}

void Frame::createShapes() {

    using namespace draw;

    static const wchar_t* kTexts[] = {
        L"686", L"5", L"405", L"955", L"768", L"151", L"982", L"601", L"337", L"525",
        L"632", L"649", L"904", L"223", L"518", L"925", L"826", L"28", L"943", L"873",
        L"220", L"915", L"538", L"190", L"67", L"34", L"601", L"478", L"623", L"265",
        L"259", L"664", L"118", L"480", L"54", L"593", L"445", L"344", L"660", L"559",
        L"103", L"710", L"428", L"696", L"982", L"232", L"980", L"265", L"505", L"564"
    };
    static const auto kTextsMaxIndex = sizeof(kTexts) / sizeof(kTexts[0]) - 1;
    static const auto kRandMax = (float)RAND_MAX;

    for (int32_t y = 0; y < 60; ++y) {
        for (int32_t x = 0; x < 50; ++x) {
            auto rand = std::rand() / kRandMax;
            texts_.emplace_back(canvas_->addText(
                Point(x * 30 + int32_t(rand * 15), y * 15),
                Color(0.0f, rand * 0.4f, 0.0f),
                kTexts[uint32_t(rand * kTextsMaxIndex)]));
        }
    }

    for (int32_t y = 0; y < 240; ++y) {
        for (int32_t x = 0; x < 200; ++x) {
            auto rand = std::rand() / kRandMax;
            shapes_.emplace_back(canvas_->addRect(
                Point(x * 5 + int32_t(rand * 150), y * 3),
                Size(2, 2),
                Color(1.0f - rand * 0.2f, 0.6f, 0.6f)
            ));
        }
    }
}

class App : public wxApp {

public:
    virtual bool OnInit() {

        SetAppName(wxT("draw library - wxWidgets test"));

        if (!wxApp::OnInit())
            return false;

        new Frame(NULL, L"draw library - wxWidgets test", wxPoint(0, 0), wxSize(900, 600));
        return true;
    }
};

IMPLEMENT_APP(App)