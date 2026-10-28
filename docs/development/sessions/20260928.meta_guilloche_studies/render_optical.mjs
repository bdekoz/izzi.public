#!/usr/bin/env node
// Export the lab's original SVG geometry without a browser or dependencies.
// This exercises geometry routines, not browser layout or download behavior.
import fs from 'node:fs';
import path from 'node:path';
import vm from 'node:vm';
import {fileURLToPath} from 'node:url';

const directory = path.dirname(fileURLToPath(import.meta.url));
const html = fs.readFileSync(path.join(directory, 'optical-lab.html'), 'utf8');
const script = html.match(/<script>([\s\S]*?)<\/script>/)[1];
const pure = script.slice(0, script.indexOf('function render(){'));
const titles = {current:'Crossing currents', leaf:'Veins inside a leaf',
  wall:'Straight lines, offset tiles', facets:'Faceted hobnail',
  drift:'Luminance around an eddy', ouchi:'A floating water medallion'};
const context = vm.createContext({document:{getElementById:id=>({
  querySelector:()=>({textContent:titles[id]})
})}});
vm.runInContext(pure, context);
const outputIndex = process.argv.indexOf('--output');
const output = outputIndex >= 0 ? process.argv[outputIndex+1] : path.join(directory,'optical.svg');
if (!output) throw Error('--output requires a path');
const expressions = {
  current:'current(CONTROL,4,16)', leaf:'leaf(CONTROL,13)', wall:'wall(CONTROL)',
  facets:'facets(CONTROL,false)', drift:'drift(CONTROL)', ouchi:'ouchi(CONTROL)'
};
const notes = {
  current:'Shared bend; two layers at 4°', leaf:'Two displaced fans inside a notched blade',
  wall:'Every mortar line remains horizontal', facets:'Face tones change depth cues',
  drift:'Static luminance ordering; perceived drift varies', ouchi:'Inner texture rotates 90°; geometry remains still'
};
const panels = [];
let comparisons = 0;
for (const [id, expression] of Object.entries(expressions)) {
  const effect = vm.runInContext(`wrap('${id}',${expression.replace('CONTROL','false')})`,context);
  const control = vm.runInContext(`wrap('${id}',${expression.replace('CONTROL','true')})`,context);
  if (effect === control) throw Error(`${id}: comparison has no effect`);
  if (/NaN|Infinity|undefined/.test(effect+control)) throw Error(`${id}: invalid geometry`);
  comparisons++;
  const index=panels.length,x=30+(index%3)*462,y=123+Math.floor(index/3)*415;
  panels.push(`<g transform="translate(${x},${y})"><rect width="442" height="389" rx="8" fill="white" stroke="#d7dedb"/>
    <text x="11" y="27" font-size="19" font-weight="600">${titles[id]}</text>
    <svg x="11" y="45" width="420" height="300">${effect}</svg>
    <text x="11" y="370" font-size="12">${notes[id]}</text></g>`);
}
for (const angle of [0,12]) for (const bend of [0,30]) {
  const result=vm.runInContext(`current(false,${angle},${bend})`,context);
  if (/NaN|Infinity|undefined/.test(result)) throw Error('Current slider endpoint failed');
}
for (const shift of [0,30]) vm.runInContext(`leaf(false,${shift})`,context);
const regular=vm.runInContext('facets(false,false)',context);
const reverse=vm.runInContext('facets(false,true)',context);
if(regular===reverse)throw Error('Reversing facet tones had no effect');
const svg=`<svg xmlns="http://www.w3.org/2000/svg" width="1440" height="1010" viewBox="0 0 1440 1010" role="img" aria-labelledby="title desc">
  <title id="title">Water, leaves and six optical treatments</title>
  <desc id="desc">Two interference fields, a Cafe Wall study, hobnail shading, a peripheral drift study and an Ouchi texture study. Static original constructions with comparison controls in the companion HTML lab.</desc>
  <style>text{font-family:DejaVu Sans,sans-serif;fill:#23464d}</style><rect width="1440" height="1010" fill="#f5f6f2"/>
  <text x="30" y="48" font-size="29" font-weight="600">Water, leaves and optical effects</text>
  <text x="30" y="82" font-size="16">Original constructions · static geometry · open optical-lab.html for parameters and comparison controls</text>
  ${panels.join('\n')}
  <text x="30" y="983" font-size="13">Perceptual effects depend on viewing scale and contrast. These sketches do not establish perceptual performance of future Izzi presets.</text>
</svg>\n`;
fs.writeFileSync(output,svg);
console.log(`${output}: ${comparisons} distinct effect/control pairs; current/leaf slider endpoints and facet reversal checked`);
