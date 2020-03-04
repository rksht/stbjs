#include <filesystem>
#include <napi.h>
#include <string>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace fs = std::filesystem;

struct StbImageLoadResult {
    void *data;
    int width;
    int height;
    int channels;
    int desired_channels;
    const char *error_message = nullptr;
};

StbImageLoadResult load_image(const std::string &filename, int desired_channels) {
    StbImageLoadResult result{};
    result.desired_channels = desired_channels;

    auto path = fs::path(filename);
    if (!fs::exists(path)) {
        result.error_message = "File not found";
        return result;
    }

    void *data = stbi_load(filename.c_str(), &result.width, &result.height, &result.channels, desired_channels);
    if (data == nullptr) {
        result.error_message = "Failed to load image";
        return result;
    }

    result.data = data;
    return result;
}

// loadPng(fileName) -> {width: Int, height: Int, numChannels: Int, desiredChannels: Int, data: ArrayBuffer(w
// * h * numChannels)}
Napi::Object load_png(const Napi::CallbackInfo &info) {
    auto env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(
            env, fmt::format("{} - Expected Argument types (filename :: String)", __PRETTY_FUNCTION__))
            .ThrowAsJavaScriptException();
    }

    const std::string filename = info[0].As<Napi::String>();

    const int desired_channels =
        info.Length() >= 2 && info[1].IsNumber() ? info[1].As<Napi::Number>().Int32Value() : 0;

    auto result = load_image(filename, desired_channels);

    if (result.error_message) {
        Napi::Error::New(env, fmt::format("loadPng({}) - {}", filename, result.error_message))
            .ThrowAsJavaScriptException();
    }

    size_t byte_length = size_t(result.channels * result.width * result.height);
    auto array_buffer = Napi::ArrayBuffer::New(env, byte_length);

    memcpy(array_buffer.Data(), result.data, byte_length);
    STBI_FREE(result.data);

    auto imageObject = Napi::Object::New(env);
    imageObject.Set("width", result.width);
    imageObject.Set("height", result.height);
    imageObject.Set("numChannels", result.channels);
    imageObject.Set("desiredChannels", result.desired_channels);
    imageObject.Set("data", array_buffer);
    return imageObject;
}

// writePng(imageObject :: <see above>, fileName) -> boolean.
Napi::Boolean write_png(const Napi::CallbackInfo &info) {
    auto env = info.Env();

    if (info.Length() != 2 || !info[0].IsObject() || !info[1].IsString()) {
        Napi::Error::New(env, "Argument Error - writePng(imageObject :: <see above>, fileName) -> boolean")
            .ThrowAsJavaScriptException();
    }

    auto imageObject = info[0].As<Napi::Object>();
    auto width = imageObject.Get("width").As<Napi::Number>().Int32Value();
    auto height = imageObject.Get("height").As<Napi::Number>().Int32Value();
    auto num_channels = imageObject.Get("numChannels").As<Napi::Number>().Int32Value();
    auto file_name = std::string(info[1].As<Napi::String>());
    auto stride = width * num_channels;

    const void *data = imageObject.Get("data").As<Napi::ArrayBuffer>().Data();
    int ok = stbi_write_png(file_name.c_str(), width, height, num_channels, data, stride);
    return ok ? Napi::Boolean::New(env, true) : Napi::Boolean::New(env, false);
}

Napi::Object add_exports(Napi::Env env, Napi::Object exports) {
    exports.Set("loadPng", Napi::Function::New(env, load_png));
    exports.Set("writePng", Napi::Function::New(env, write_png));
    return exports;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) { return add_exports(env, exports); }

NODE_API_MODULE(testaddon, InitAll);
