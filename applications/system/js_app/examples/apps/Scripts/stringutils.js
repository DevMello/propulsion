let sampleText = "Hello, World!";

let lengthOfText = "Length of text: " + to_string(sampleText.length);
print(lengthOfText);

let start = 7;
let end = 12;
let substringResult = sampleText.slice(start, end);
print(substringResult);

let searchStr = "World";
let result2 = to_string(sampleText.indexOf(searchStr));
print(result2);

let upperCaseText = "Text in upper case: " + toUpperCase(sampleText);
print(upperCaseText);

let lowerCaseText = "Text in lower case: " + toLowerCase(sampleText);
print(lowerCaseText);
