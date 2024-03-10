let math = require("math");

let absResult = math.abs(-5);
let acosResult = math.acos(0.5);
let acoshResult = math.acosh(2);
let asinResult = math.asin(0.5);
let asinhResult = math.asinh(2);
let atanResult = math.atan(1);
let atan2Result = math.atan2(1, 1);
let atanhResult = math.atanh(0.5);
let cbrtResult = math.cbrt(27);
let ceilResult = math.ceil(5.3);
let clz32Result = math.clz32(1);
let cosResult = math.cos(math.PI);
let expResult = math.exp(1);
let floorResult = math.floor(5.7);
let maxResult = math.max(3, 5);
let minResult = math.min(3, 5);
let powResult = math.pow(2, 3);
let randomResult = math.random();
let signResult = math.sign(-5);
let sinResult = math.sin(math.PI / 2);
let sqrtResult = math.sqrt(25);
let truncResult = math.trunc(5.7);

print("math.abs(-5):", absResult);
print("math.acos(0.5):", acosResult);
print("math.acosh(2):", acoshResult);
print("math.asin(0.5):", asinResult);
print("math.asinh(2):", asinhResult);
print("math.atan(1):", atanResult);
print("math.atan2(1, 1):", atan2Result);
print("math.atanh(0.5):", atanhResult);
print("math.cbrt(27):", cbrtResult);
print("math.ceil(5.3):", ceilResult);
print("math.clz32(1):", clz32Result);
print("math.cos(math.PI):", cosResult);
print("math.exp(1):", expResult);
print("math.floor(5.7):", floorResult);
print("math.max(3, 5):", maxResult);
print("math.min(3, 5):", minResult);
print("math.pow(2, 3):", powResult);
print("math.random():", randomResult);
print("math.sign(-5):", signResult);
print("math.sin(math.PI/2):", sinResult);
print("math.sqrt(25):", sqrtResult);
print("math.trunc(5.7):", truncResult);