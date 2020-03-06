export as namespace stbjs;

export interface pngData {
	width: number;
	height: number;
	numChannels: number;
	data: ArrayBuffer;
}

export interface loadedPng {
	channelsInFile: number;
}

export function loadPng(fileName: string): loadedPng;

export function writePng(png: pngData, fileName: string): boolean;

export function black(
	width: number,
	height: number,
	numChannels: number,
	opacity: number
): pngData;
