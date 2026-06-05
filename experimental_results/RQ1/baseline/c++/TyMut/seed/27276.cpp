

#include <tuple>

template <typename SessionT>
struct SomeQuery {
    SessionT& session_;
    SomeQuery(SessionT& session) : session_(session) {}
};

template <typename SessionT>
struct Handler {
    std::tuple<SomeQuery<SessionT>> queries_;
    Handler(SessionT& session) : queries_(session) {}
};

struct Session {
    Handler<Session> handler_;
    Session() : handler_{*this} {}
};

int main() {
    Session session;
}


