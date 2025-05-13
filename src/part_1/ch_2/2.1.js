function find_root(graph, v) {
  let i = 0;
  for (i = graph[v]; i != graph[i]; i = graph[i]);

  return i;
}

function find_root_with_compression(graph, v) {
  let i;
  for (i = graph[v]; i != graph[i]; i = graph[i]) {
    graph[i] = graph[graph[i]];
  }

  return i;
}

const process = require('node:process');
const N = process.argv[2] == undefined ? 100000000 : process.argv[2];
const ARR = [N];
const SZ = [N];
for (let i = 0; i < N; i++) {
  ARR[i] = i;
  SZ[i] = 1;
}

function is_valid(v) {
  return v >= 0 && v < N;
}

function getRandomInt(max) {
  return Math.floor(Math.random() * max);
}

const readline = require('node:readline');
const rl = readline.createInterface({ input: process.stdin, output: process.stdoit});

let union_count = 0;

rl.on('line', (input) => {
  const vals = input.split(' ');

  try {
    if (vals.length != 2) {
      throw new Error('Illegal args length, expected 2');
    }

    const p = Number.parseInt(vals[0]);
    const q = Number.parseInt(vals[1]);
    if (!is_valid(p) || !is_valid(q)) {
      throw new Error(`Out of range [0, ${N})`);
    }

    let i, j;
    for (i = p; i != ARR[i]; i = ARR[i]) {
      ARR[i] = ARR[ARR[i]];
    }

    for (j = q; j != ARR[j]; j = ARR[j]) {
       ARR[j] = ARR[ARR[j]];
    }
 
    if (i == j) { 
      return;
    }

    if (SZ[i] < SZ[j]) {
      ARR[i] = ARR[j];
      SZ[j] += SZ[i];
      union_count = SZ[j] > union_count ? SZ[j] : union_count; 
    } else {
      ARR[j] = ARR[i];
      SZ[i] += SZ[j];
     union_count = SZ[i] > union_count ? SZ[i] : union_count;
    }

    console.log(`${p}-${q}`);
    if (union_count == N - 1) {
      rl.close();
    }
  } catch(e) {
    console.error('Failed: ', e);
    rl.close();
    return;
  }
});

const start = Date.now();
rl.on('close', () => {
  console.log(`Took: ${Date.now() - start}`);
});
