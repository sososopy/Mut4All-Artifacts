
class Chunk { };
using ChunkMetaData = std::tuple<void, Chunk, void>;

template <typename MessageT> ChunkMetaData chunkMetaData(MessageT&&);


template <typename MessageChunkT>
class ChunkSeq
{
public:


  using good_type = ChunkMetaData;
  
 
#ifdef SHOWBUG
  using bad_type = decltype(chunkMetaData(std::declval<MessageChunkT>()));
  static_assert(std::is_same_v<good_type, bad_type>);
  using MetaData = bad_type;
#else
  using MetaData = good_type;
#endif

