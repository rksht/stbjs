import { loadPng, writePng, black } from "../src/index";

writePng(black(1024, 1024, 4, 255), "hello.png");
