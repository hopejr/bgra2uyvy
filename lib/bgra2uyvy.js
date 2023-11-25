const addon = require('../build/Release/bgra2uyvy');

module.exports = {
  convert (image) {
    const size = image.getSize()
    return addon.Convert(image.toBitmap(), size.width, size.height)
  }
}
