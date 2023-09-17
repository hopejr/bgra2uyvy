const Bgra2uyvy = require("../lib/binding.js");
const assert = require("assert");

assert(Bgra2uyvy.convert, "The expected function is undefined");

function testBasic()
{
    const result =  Bgra2uyvy.convert({
        getSize() {
            return { width: 2, height: 2 }
        },
        toBitmap() {
            return Buffer.from([0, 23, 82, 255, 54, 54, 54, 255, 128, 100, 80, 255, 34, 54, 180, 255])
        }
    });
    assert.deepStrictEqual(result, Buffer.from([0x6d, 0x30, 0x9b, 0x3e, 0x8f, 0x63, 0x75, 0x5c]), "Unexpected value returned");
}

assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");

console.log("Tests passed- everything looks OK!");