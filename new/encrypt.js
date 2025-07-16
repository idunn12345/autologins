const CryptoJS = require("crypto-js");

const value = process.argv[2] || "";

if (!value) {
    console.log("");
    process.exit(0);
}

const key = CryptoJS.enc.Utf8.parse("53419923-KPLR-LKRO-PLAP-0F9A0R0V");
const iv  = CryptoJS.enc.Utf8.parse("2382329482395861");

const encrypted = CryptoJS.AES.encrypt(
    CryptoJS.enc.Utf8.parse(value),
    key,
    {
        keySize: 128,
        iv: iv,
        mode: CryptoJS.mode.CBC,
        padding: CryptoJS.pad.Pkcs7
    }
).toString();

console.log(encrypted);
