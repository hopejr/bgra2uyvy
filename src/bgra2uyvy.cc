#include <napi.h>

using namespace Napi;

uint8_t getY(uint8_t r, uint8_t g, uint8_t b) {
  return 16 + (((r << 6) + (r << 1) + (g << 7) + g + (b << 4) + (b << 3) + b) >> 8);
}

uint8_t getU(uint8_t r, uint8_t g, uint8_t b) {
  return 128 + ((-((r << 5) + (r << 2) + (r << 1)) - ((g << 6) + (g << 3) + (g << 1)) + (b << 7) - (b << 4)) >> 8);
}

uint8_t getV(uint8_t r, uint8_t g, uint8_t b) {
  return 128 + (((r << 7) - (r << 4) - ((g << 6) + (g << 5) - (g << 1)) - ((b << 4) + (b << 1))) >> 8);
}

Value Convert(const CallbackInfo& info) {
  Env env = info.Env();

  if (info.Length() != 3) {
    Error::New(env, "Expected three arguments: bitmap, width, height")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[0].IsBuffer()) {
    Error::New(env, "First parameter must be a buffer (i.e. bitmap)")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[1].IsNumber()) {
    Error::New(env, "Second parameter must be a number (i.e. width of bitmap)")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  if (!info[2].IsNumber()) {
    Error::New(env, "Third parameter must be a number (i.e. height of bitmap)")
        .ThrowAsJavaScriptException();
    return env.Undefined();
  }

  Buffer<uint8_t> bgraBuffer = info[0].As<Buffer<uint8_t>>();
  uint8_t* bgradata = bgraBuffer.Data();
  size_t length = bgraBuffer.Length();

  auto width = info[1].As<Number>().Uint32Value();
  auto height = info[2].As<Number>().Uint32Value();

  uint8_t bpp = (uint8_t)((length / width) / height);

  uint8_t yuvdata[(width * 16 / 8) * height];

  size_t pos = 0;

  for (size_t i = 0; i < (length / bpp) - 1; i += 2) {
    uint8_t b1 = bgradata[(i * bpp)];
    uint8_t g1 = bgradata[(i * bpp) + 1];
    uint8_t r1 = bgradata[(i * bpp) + 2];
    uint8_t b2 = bgradata[(i * bpp) + bpp];
    uint8_t g2 = bgradata[(i * bpp) + bpp + 1];
    uint8_t r2 = bgradata[(i * bpp) + bpp + 2];
    uint8_t y1 = getY(r1, g1, b1);
    uint8_t u1 = getU(r1, g1, b1);
    uint8_t v1 = getV(r1, g1, b1);
    uint8_t y2 = getY(r2, g2, b2);
    yuvdata[pos++] = u1;
    yuvdata[pos++] = y1;
    yuvdata[pos++] = v1;
    yuvdata[pos++] = y2;
  }

  return Buffer<uint8_t>::Copy(env, yuvdata, sizeof(yuvdata));
}

Object Init(Env env, Object exports) {
  exports.Set(String::New(env, "Convert"),
              Function::New(env, Convert));
  return exports;
}

NODE_API_MODULE(addon, Init)
