
#include <wx/event.h>

int main()
{
wxEvent* evt = new wxNotifyEvent();
evt->Clone();
return 0;
}
