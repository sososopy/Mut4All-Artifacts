
namespace Playlists {
class Playlist;
}
namespace Playlist {
class ModelStack {
public:
  static ModelStack instance;
};
}
using namespace Playlists;
void fn1() { ::Playlist::ModelStack::instance; }

