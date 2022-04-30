/*!
  Goblin Library

  @file  gob_pcm_stream.cpp
  @brief PCM stream that using Stream.
*/
#include "gob_pcm_stream.hpp"

namespace goblib
{

bool PcmStream::fetch()
{
    _dataHead = 0;
    _dataSize = 0;
    if(!_stream || !_stream->is_open()) { return false; }

    _stream->seek(0, goblib::seekdir::beg);
    
    // RiffChunk
    wave::RIFFChunk rchunk;;
    if(_stream->read(&rchunk, sizeof(rchunk)) != sizeof(rchunk))
    {
        return false;
    }
    if(rchunk.identifier != wave::RIFFChunk::RIFF)
    {
        return false;
    }
    if(rchunk.format != wave::RIFFChunk::WAVE)
    {
        return false;
    }

    // fmtChunk
    if(_stream->read(&_format, sizeof(_format)) != sizeof(_format))
    {
        return false;
    }
    if(_format.identifier != wave::fmtSubChunk::FMT)
    {
        return false;
    }
    // Only support PCM
    if(_format.format != wave::fmtSubChunk::WAVE_FORMAT_PCM)
    {
        return false;
    }

    // Read and skip sub chunks until data found.
    wave::SubChunk sub;
    for(;;)
    {
        if(_stream->read(&sub, sizeof(sub)) != sizeof(sub))
        {
            return false;
        }

        if(sub.identifier == wave::SubChunk::DATA) { break; } // "data"

        if(!_stream->seek(sub.size, goblib::seekdir::cur))
        {
            return false;
        }
    }

    // Head of PCM and size.
    _dataHead = _stream->position();
    _dataSize = sub.size;

    return true;
}

//
}
