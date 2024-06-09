import { $ } from "bun";
import { afterAll, describe, expect, test } from "bun:test";

const Lang = {
  C: "c",
  Cpp: "cpp",
  Js: "js",
  Rust: "rs",
}
await $`mkdir -p tmp`;
const result = await Promise.allSettled([
  buildC(),
  buildCpp(),
  buildJs(),
  buildRust(),
]);
console.warn(
  result
    .filter(x => x.status === "rejected")
    .map(x => x.reason)
);

const availableLangs = result
  .filter(x => x.status === "fulfilled")
  .map(x => x.value);

afterAll(async () => {
  await $`rm -r tmp`;
});

describe("empty string", () => {
  the("").toBe("");
});

describe("multiline", () => {
  the("abc\ndef\n\nghi").toBe("# abc\n# def\n\n# ghi");
});

describe("unicode", () => {
  the("우리가족👨‍👩‍👧‍👦").toBe("# 우리가족👨‍👩‍👧‍👦");
});

describe("unicode", () => {
  the("우리가족👨‍👩‍👧‍👦").toBe("# 우리가족👨‍👩‍👧‍👦");
});

describe("trim", () => {
  the("         감지              \n      금니", { skips: [Lang.C] }).toBe("# 감지\n# 금니");
});

describe("crlf (keep)", () => {
  the("ab\r\ncd\nef", { skips: [Lang.Cpp, Lang.Js, Lang.Rust] }).toBe("# ab\r\n# cd\n# ef");
});
describe("crlf (into \\n)", () => {
  the("ab\r\ncd\nef", { skips: [Lang.C] }).toBe("# ab\n# cd\n# ef");
})

function the(testcase, options = {}) {
  return {
    toBe(answer) {
      for (const lang of availableLangs) {
        if (options?.skips?.some(skip => skip === lang) ?? false) continue;
        test(lang, async () => {
          expect(await $`echo ${testcase} | tmp/hd${lang}`.text())
            .toBe(answer + "\n");
        });
      }
    }
  }
}

async function buildC() {
  await $`clang hd.c -o tmp/hdc`;
  return Lang.C;
}

async function buildCpp() {
  await $`clang++ hd.cpp -Oz --std=c++23 -o tmp/hdcpp`;

  return Lang.Cpp;
}

async function buildJs() {
  await $`bun build hd.js --compile --outfile tmp/hdjs`;
  return Lang.Js;
}

async function buildRust() {
  await $`rustc hd.rs -C debuginfo=0 -C opt-level=z -C strip=symbols -o tmp/hdrs`;
  return Lang.Rust;
}