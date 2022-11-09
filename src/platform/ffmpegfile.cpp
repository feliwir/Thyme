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
#include "ffmpegfile.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

namespace Thyme
{

FFmpegFile::~FFmpegFile()
{
    Close_FFmpeg_Contexts();
}

/**
 * Read an FFmpeg packet from file
 */
int FFmpegFile::Read_FFmpeg_Packet(void *opaque, uint8_t *buf, int buf_size)
{
    File *file = static_cast<File *>(opaque);
    int read = file->Read(buf, buf_size);

    // Streaming protocol requires us to return real errors - when we read less equal 0 we're at EOF
    if (read <= 0)
        return AVERROR_EOF;

    return read;
}

/**
 * Open all the required FFmpeg handles for a required file.
 */
bool FFmpegFile::Open_FFmpeg_Contexts(File *file)
{
#if LOGGING_LEVEL != LOGLEVEL_NONE
    av_log_set_level(AV_LOG_INFO);
#endif

// This is required for FFmpeg older than 4.0 -> deprecated afterwards though
#if LIBAVFORMAT_VERSION_MAJOR < 58
    av_register_all();
#endif

    // FFmpeg setup
    m_format_ctx = avformat_alloc_context();
    if (!m_format_ctx) {
        captainslog_error("Failed to alloc AVFormatContext");
        return false;
    }

    size_t avio_ctx_buffer_size = 0x10000;
    uint8_t *buffer = static_cast<uint8_t *>(av_malloc(avio_ctx_buffer_size));
    if (!buffer) {
        captainslog_error("Failed to alloc AVIOContextBuffer");
        Close_FFmpeg_Contexts();
        return false;
    }

    m_io_ctx = avio_alloc_context(buffer, avio_ctx_buffer_size, 0, file, &Read_FFmpeg_Packet, nullptr, nullptr);
    if (!m_io_ctx) {
        captainslog_error("Failed to alloc AVIOContext");
        Close_FFmpeg_Contexts();
        return false;
    }

    m_format_ctx->pb = m_io_ctx;
    m_format_ctx->flags |= AVFMT_FLAG_CUSTOM_IO;

    int result = 0;
    result = avformat_open_input(&m_format_ctx, nullptr, nullptr, nullptr);
    if (result < 0) {
        char error_buffer[1024];
        av_strerror(result, error_buffer, sizeof(error_buffer));
        captainslog_error("Failed 'avformat_open_input': %s", error_buffer);
        Close_FFmpeg_Contexts();
        return false;
    }

    result = avformat_find_stream_info(m_format_ctx, NULL);
    if (result < 0) {
        char error_buffer[1024];
        av_strerror(result, error_buffer, sizeof(error_buffer));
        captainslog_error("Failed 'avformat_find_stream_info': %s", error_buffer);
        Close_FFmpeg_Contexts();
        return false;
    }

    if (m_format_ctx->nb_streams != 1) {
        captainslog_error("Expected exactly one audio stream per file");
        Close_FFmpeg_Contexts();
        return false;
    }

    AVCodec *input_codec = avcodec_find_decoder(m_format_ctx->streams[0]->codecpar->codec_id);
    if (!input_codec) {
        captainslog_error("Audio codec not supported: '%u'", m_format_ctx->streams[0]->codecpar->codec_tag);
        Close_FFmpeg_Contexts();
        return false;
    }

    m_codec_ctx = avcodec_alloc_context3(input_codec);
    if (!m_codec_ctx) {
        captainslog_error("Could not allocate codec context");
        Close_FFmpeg_Contexts();
        return false;
    }

    result = avcodec_parameters_to_context(m_codec_ctx, m_format_ctx->streams[0]->codecpar);
    if (result < 0) {
        char error_buffer[1024];
        av_strerror(result, error_buffer, sizeof(error_buffer));
        captainslog_error("Failed 'avcodec_parameters_to_context': %s", error_buffer);
        Close_FFmpeg_Contexts();
        return false;
    }

    result = avcodec_open2(m_codec_ctx, input_codec, NULL);
    if (result < 0) {
        char error_buffer[1024];
        av_strerror(result, error_buffer, sizeof(error_buffer));
        captainslog_error("Failed 'avcodec_open2': %s", error_buffer);
        Close_FFmpeg_Contexts();
        return false;
    }

    return true;
}

/**
 * Close all the open FFmpeg handles for an open file.
 */
void FFmpegFile::Close_FFmpeg_Contexts()
{
    if (m_format_ctx) {
        avformat_close_input(&m_format_ctx);
    }

    if (m_codec_ctx) {
        avcodec_free_context(&m_codec_ctx);
    }

    if (m_io_ctx && m_io_ctx->buffer) {
        av_freep(&m_io_ctx->buffer);
    }

    if (m_io_ctx) {
        avio_context_free(&m_io_ctx);
    }
}

} // namespace Thyme
