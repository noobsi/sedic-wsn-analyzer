export function getRandomColor() {
  let samples = [
    "#cc4049",
    "#10cc4b",
    "#1311cc",
    "#06bdcc",
    "#c101cc",
    "#cc0371",
  ];
  return samples[Math.floor(Math.random() * 5)]
}
