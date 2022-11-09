/**
 * @file
 *
 * @author feliwir
 *
 * @brief Video player implementation using FFmpeg.
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "ffmpegvideoplayer.h"
#include "audiomanager.h"
#include "ffmpegvideostream.h"
#include "filesystem.h"
#include "globaldata.h"
#include "registry.h"
#include <captainslog.h>

namespace Thyme
{

FFmpegVideoPlayer::FFmpegVideoPlayer() {}

FFmpegVideoPlayer::~FFmpegVideoPlayer()
{
    Deinit();
}

/**
 * Initilise the class.
 */
void FFmpegVideoPlayer::Init()
{
    VideoPlayer::Init();
}

/**
 * Uninitilise the class.
 */
void FFmpegVideoPlayer::Deinit()
{
    g_theAudio->Release_Bink_Handle();
}

/**
 * Opens a video stream from a video title.
 */
VideoStream *FFmpegVideoPlayer::Open(Utf8String title)
{
    Video *vid = Get_Video(title);

    if (vid == nullptr) {
        return nullptr;
    }

    File *file = nullptr;
    Utf8String path;

    // First check the custom user "mod" directory if set.
    if (g_theWriteableGlobalData->m_userModDirectory.Is_Not_Empty()) {
        path.Format(
            "%s%s/%s.%s", g_theWriteableGlobalData->m_userModDirectory.Str(), "Data/Movies", vid->file_name.Str(), "bik");
        // Load the file from disk
        file = g_theFileSystem->Open(path, File::READ | File::BINARY | File::BUFFERED);
    }

    // Next check for a language specific directoy Data/%language%/Movies.
    if (file == nullptr) {
        path.Format("Data/%s/Movies/%s.%s", Get_Registry_Language().Str(), vid->file_name.Str(), "bik");
        // Load the file from disk
        file = g_theFileSystem->Open(path, File::READ | File::BINARY | File::BUFFERED);
    }

    // Finally check Data/Movies.
    if (file == nullptr) {
        path.Format("Data/Movies/%s.%s", vid->file_name.Str(), "bik");
        // Load the file from disk
        file = g_theFileSystem->Open(path, File::READ | File::BINARY | File::BUFFERED);
    }

    return Create_Stream(file);
}

VideoStream *Create_Stream(File *file)
{
    return nullptr;
}

}