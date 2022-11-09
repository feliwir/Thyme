/**
 * @file
 *
 * @author feliwir
 *
 * @brief Utility class for dealing with FFmpeg.
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#include "always.h"
#include "file.h"

struct AVFormatContext;
struct AVIOContext;
struct AVCodecContext;

namespace Thyme
{
class FFmpegFile
{
public:
    ~FFmpegFile();

    bool Open_FFmpeg_Contexts(File *file);
    void Close_FFmpeg_Contexts();

    AVFormatContext *m_format_ctx = nullptr;
    AVIOContext *m_io_ctx = nullptr;
    AVCodecContext *m_codec_ctx = nullptr;

private:
    static int Read_FFmpeg_Packet(void *opaque, uint8_t *buf, int buf_size);
};
} // namespace Thyme