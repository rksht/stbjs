export as namespace stbjs;

export interface pngData {
	width: number;
	height: number;
	numChannels: number;
	data: ArrayBuffer;
}

export interface loadedPng {
	desiredChannels: number;
}

export function loadPng(fileName: string): pngData;

export function writePng(png: pngData, fileName: string): boolean;

export function black(
	width: number,
	height: number,
	numChannels: number,
	opacity: number
): pngData;
