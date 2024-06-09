console.log(
  (await stdin())
    .trim()
    .split(/\r?\n/)
    .map(x => x.trim())
    .map(format)
    .join("\n")
)

async function stdin() {
  const result = [];
  for await (const chunk of Bun.stdin.stream()) {
    result.push(Buffer.from(chunk).toString("utf8"));
  }
  return result.join("");
}

function format(s) {
  if (s.length === 0) return "";
  if (s.startsWith("# ")) return s;
  return `# ${s}`;
}
