const fb = require('./build/Release/fullbowody.node');
const engine = fb.createEngine('c:/Users/furwa/Documents/FurWaz/FullBowody/Engine/build/Release/Engine.dll');
const plugins = engine.getPlugins();
console.log("plugins", plugins);
if (plugins.length < 1) {
    console.log("No plugins found");
    return;
}
const plugin = plugins[0];
console.log("plugin:");
console.log(" - name:", plugin.getName());
console.log(" - description:", plugin.getDescription());
console.log(" - version:", plugin.getVersion());
console.log(" - author:", plugin.getAuthor());

const camera = engine.createCamera();
console.log("camera:");
console.log(" - name:", camera.getName());

const handle = plugin.createHandle();
console.log("handle:", handle);
camera.useCapturePlugin(handle);

const capture = camera.getCapture()
console.log("capture:");
console.log(" - width:", capture.getWidth());
console.log(" - height:", capture.getHeight());

const b64_data = capture.getPreviewImage();
console.log("b64_data", b64_data);

const params = capture.getParams();
console.log("params", params.map(p => p.name));
console.log("Setting param index");
params.filter(p => p.name === "index")[0].value = 0;
console.log("Setting param model");
params.filter(p => p.name === "model")[0].value = "c:/Users/furwa/Documents/FurWaz/FullBowody/Engine/build/Release/thunder";

camera.startPreview();
camera.startTracking();

console.log("updating engine");
const interval = setInterval(() => {
    engine.update(0.1);
    console.log("updated");
    // const b64_data_new = camera.getCapture().getPreviewImage();
    // console.log("b64_data_new", b64_data_new);

    const skeleton = camera.getCapture().getCaptureSkeleton();
    console.log("skeleton", skeleton);
    if (!skeleton) return;
    const joints = skeleton.getJoints();
    console.log("joints", joints.map(j => ({position: {x: j.getPosition().x, y: j.getPosition().y, z: j.getPosition().z}, confidence: j.getConfidence()})));
}, 100);
setTimeout(() => {
    clearInterval(interval);
}, 5000);
