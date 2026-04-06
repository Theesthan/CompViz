import { Codec } from "./Codec.js";

let codecObj;

document.addEventListener("DOMContentLoaded", () => {
	// DOM elements
	const decodeBtn = document.getElementById("decode");
	const encodeBtn = document.getElementById("encode");
	const uploadFile = document.getElementById("uploadfile");
	const submitBtn = document.getElementById("submitbtn");
	const step1 = document.getElementById("step1");
	const step2 = document.getElementById("step2");
	const step3 = document.getElementById("step3");
	const startAgainBtn = document.getElementById("startagain");

	// Init codec
	codecObj = new Codec();

	// File submit button
	submitBtn.onclick = function () {
		const uploadedFile = uploadFile.files[0];
		if (!uploadedFile) {
			alert("No file uploaded.\nPlease upload a file and try again");
			return;
		}
		const extension = uploadedFile.name.split('.').pop().toLowerCase();
		if (extension !== "txt") {
			alert("Invalid file type (." + extension + ") \nPlease upload a valid .txt file and try again");
			return;
		}
		step1.style.display = "none";
		step2.style.display = "inline-flex";
		startAgainBtn.style.visibility = "visible";
	};

	// Encode button
	encodeBtn.onclick = function () {
		const uploadedFile = uploadFile.files[0];
		if (!uploadedFile) {
			alert("No file uploaded.\nPlease upload a file and try again");
			return;
		}

		if (uploadedFile.size === 0) {
			alert("You have uploaded an empty file!");
		} else if (uploadedFile.size <= 350) {
			alert("The uploaded file is very small (" + uploadedFile.size + " bytes). Compression may be ineffective.");
		} else if (uploadedFile.size < 1000) {
			alert("The uploaded file is small (" + uploadedFile.size + " bytes). Compression ratio may be low.");
		}

		onclickChanges2("Compressing your file ...\n", "Compressed");

		const fileReader = new FileReader();
		fileReader.onload = function (event) {
			const text = event.target.result;
			const [encodedString, outputMsg] = codecObj.encode(text);
			myDownloadFile(uploadedFile.name.split('.')[0] + "_compressed.txt", encodedString);
			ondownloadChanges(outputMsg);
		};
		fileReader.readAsText(uploadedFile, "UTF-8");

		step2.style.display = "none";
		step3.style.display = "inline-flex";
	};

	// Decode button
	decodeBtn.onclick = function () {
		const uploadedFile = uploadFile.files[0];
		if (!uploadedFile) {
			alert("No file uploaded.\nPlease upload a file and try again!");
			return;
		}

		onclickChanges2("De-compressing your file ...\n", "De-Compressed");

		const fileReader = new FileReader();
		fileReader.onload = function (event) {
			const buffer = new Uint8Array(event.target.result);
			const binaryStr = [...buffer].map(b => String.fromCharCode(b)).join('');
			const [decodedString, outputMsg] = codecObj.decode(binaryStr);
			myDownloadFile(uploadedFile.name.split('.')[0] + "_decompressed.txt", decodedString);
			ondownloadChanges(outputMsg);
		};
		fileReader.readAsArrayBuffer(uploadedFile);

		step2.style.display = "none";
		step3.style.display = "inline-flex";
	};
});

// Show compressing/decompressing message
function onclickChanges2(secMsg, word) {
	const decodeBtn = document.getElementById("decode");
	const encodeBtn = document.getElementById("encode");
	const step3 = document.getElementById("step3");

	decodeBtn.disabled = true;
	encodeBtn.disabled = true;
	step3.innerHTML = "";

	const msg2 = document.createElement("span");
	msg2.className = "text2";
	msg2.innerHTML = secMsg;
	step3.appendChild(msg2);

	const msg3 = document.createElement("span");
	msg3.className = "text2";
	msg3.innerHTML = word + " file will be downloaded automatically!";
	step3.appendChild(msg3);
}

// File download function (safe for binary)
function myDownloadFile(fileName, text) {
	const byteArray = new Uint8Array([...text].map(c => c.charCodeAt(0)));
	const blob = new Blob([byteArray], { type: "application/octet-stream" });

	const a = document.createElement('a');
	a.href = URL.createObjectURL(blob);
	a.download = fileName;
	document.body.appendChild(a);
	a.click();
	document.body.removeChild(a);
	URL.revokeObjectURL(a.href);
}

// Display results after download
function ondownloadChanges(outputMsg) {
	const step3 = document.getElementById("step3");
	step3.innerHTML = "";

	const img = document.createElement("img");
	img.src = "images/done.jpg";
	img.id = "doneImg";
	step3.appendChild(img);

	step3.appendChild(document.createElement("br"));

	const msg3 = document.createElement("span");
	msg3.className = "text2";
	msg3.innerHTML = outputMsg;
	step3.appendChild(msg3);
}

// Live Huffman encoding demo for short input
window.runHuffmanDemo = function () {
	const input = document.getElementById("demoInput").value;

	if (!input) {
		alert("Please enter some text for encoding.");
		return;
	}

	if (input.length > 100) {
		alert("Please limit input to 100 characters.");
		return;
	}

	const result = codecObj.demoEncode(input);

	document.getElementById("demoEncodedOutput").value = result.encoded || "No encoded output.";

	const table = result.codeMap;
	const tableDisplay = Object.entries(table).map(([char, code]) =>
		`'${char}' : ${code}`
	).join("\n");

	document.getElementById("demoCodeTable").textContent = tableDisplay || "Code table is empty.";
};
