const stbjs = require("../build/Release/stbjs.node");

function black(width, height, numChannels, opacity)
{
    const pngData = {width, height, numChannels, data : new ArrayBuffer(numChannels * width * height)};

    const data = pngData.data;

    if (opacity !== 0) {
        for (let i = 0; i < width * height; ++i) {
            data[i * numChannels + numChannels] = opacity;
        }
    }

    return pngData;
}

module.exports = {
    loadPng: stbjs.loadPng,
    writePng: stbjs.writePng,
    black
};
