const stbjs = require("../build/Release/stbjs.node");

function black(width, height, numChannels, opacity) {
    const data = new Uint8Array(width * height * numChannels);
    const pngData = { width, height, numChannels, data: data.buffer };

    const alphaOffset = numChannels - 1;

    if (opacity !== 0) {
        for (let i = 0; i < width * height; ++i) {
            data[i * numChannels + alphaOffset] = opacity;
        }
    }

    console.log(data);

    return pngData;
}

module.exports = {
    loadPng: stbjs.loadPng,
    writePng: stbjs.writePng,
    black
};
