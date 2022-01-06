/*!
  Goblin Library

  @file  gob_pcm_stream.hpp
  @brief PCM stream that using Stream
*/
#ifndef GOBLIB_PCM_STREAM_HPP
#define GOBLIB_PCM_STREAM_HPP

#include "gob_stream.hpp"

namespace goblib
{

/*! @brief RIFF WAVE file format structures */
namespace wave
{
#pragma pack(push,1)

/*! @brief RIFF chunk */
struct RIFFChunk
{
    std::uint32_t identifier;
    std::uint32_t size;
    std::uint32_t format;

    constexpr static std::uint32_t RIFF = 0x46464952; //!< "RIFF" identifier
    constexpr static std::uint32_t WAVE = 0x45564157; //!< "WAVE" identifier
};

/*! @brief fmt Sub-Chunk */
struct fmtSubChunk
{
    uint32_t identifier;
    uint32_t size;
    uint16_t format;
    uint16_t channels;
    uint32_t rate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bits;

    constexpr static std::uint32_t FMT =  0x20746d66; //!< "Fmt " identifier
    
    // format code
    constexpr static uint16_t WAVE_FORMAT_PCM = 0x0001;
    constexpr static uint16_t WAVE_FORMAT_IEEE_FLOAT = 0x0003;
    constexpr static uint16_t WAVE_FORMAT_ALAW = 0x0006;
    constexpr static uint16_t WAVE_FORMAT_MULAW = 0x0007;
    constexpr static uint16_t WAVE_FORMAT_EXTENSIBLE = 0xFFFE;
};

struct fmtChunkExtra
{
    uint16_t size;
    //    uint8_t _data[size];
};

/*! @brief Sub chunk */
struct SubChunk
{
    uint32_t identifier; 
    uint32_t size;
    //  uint8_t _data[size];
    constexpr static std::uint32_t DATA = 0x61746164; //!< "data" identifier
};

#pragma pack(pop)
}

/*!
  @brief PCM stream that using Stream.
*/
class PcmStream
{
  public:
    PcmStream() : PcmStream(nullptr) {}
    explicit PcmStream(Stream* s)
            : _stream(s), _format{0,0,0,0,0,0,0,0}, _dataHead(0), _dataSize(0)
    {
        if(s) { fetch(); }
    }

    bool assign(Stream* s)
    {
        _stream = s;
        return fetch();
    }

    /// @name Property
    /// @{
    bool valid() const { return _stream && _dataHead && _dataSize; }
    Stream::pos_type head() const { return _dataHead; }
    Stream::pos_type position() const { return valid() ? _stream->position() : 0; }
    std::size_t dataSize() const { return _dataSize; }
    bool is_tail() const { return valid() && _stream->is_tail(); }
    /// @}

    /// @name Property of PCM
    /// @{
    std::uint16_t channels() const { return _format.channels; }
    std::uint32_t sampleRate() const { return _format.rate; }
    std::uint16_t bitsPerSample() const { return _format.bits; }
    /// @}

    std::size_t read(std::uint8_t* buf, std::size_t sz)
    {
        if(!valid() || _stream->is_tail()) { return 0; }
        return _stream->read(buf, sz);
    }
    
    /// @name Seek
    /// @{
    /*! @brief Rewind to head of data. */
    bool rewind() { return valid() ? _stream->seek(_dataHead, goblib::seekdir::beg) : false; }
    /*! @brief Seek data offset */
    bool seek(std::size_t doff) { return valid() ? _stream->seek(_dataHead + doff, goblib::seekdir::beg) : false; }
    /// @}

  protected:
    bool fetch();
    
  private:
    Stream* _stream;
    wave::fmtSubChunk _format;
    Stream::pos_type _dataHead;
    std::size_t _dataSize;
};

//
}
#endif
