const stbjs = require("./build/Release/stbjs.node");
const util = require("util");

function black(width, height, numChannels, opacity) {
	const pngData = {
		width,
		height,
		numChannels,
		data: new Uint8Array(numChannels * w * h)
	};

	const data = pngData.data;

	if (opacity !== 0) {
		for (let i = 0; i < width * height; ++i) {
			data[i * numChannels + numChannels] = opacity;
		}
	}
	return pngData;
}

function loadPng(fileName) {
	const image = stbjs.loadPng(fileName, 3);
	console.log(util.inspect(image));
	console.assert(stbjs.writePng(image, `${fileName}_saved.png`));
}

function writePng(pngData, fileName) {
	return stbjs.writePng(pngData, fileName);
}

loadPng("image.png");

module.exports = {
	loadPng,
	writePng,
	createImage
};