export interface PluginHandle {};

export interface PluginDescription {
    getName(): string;
    getDescription(): string;
    getType(): string;
    getAuthor(): string;
    getVersion(): string;
    createHandle(): PluginHandle;
};

export enum ParamType {
    BOOL = "BOOL",
    INT = "INT",
    FLOAT = "FLOAT",
    STRING = "STRING",
    ENUM = "ENUM",
};

export interface Param {
    name: string;
    value: any;
    type: ParamType;
};

export interface Vec3 {
    x: number;
    y: number;
    z: number;
}

export interface CaptureJoint {
    getPosition(): Vec3;
    getConfidence(): number;
    setPosition(position: Vec3): void;
    setConfidence(confidence: number): void;
};

export interface CaptureSkeleton {
    setJoint(index: number, captureJoint: CaptureJoint): void;
    getJoints(): CaptureJoint[];
    getJoint(index: number): CaptureJoint;
    getJointCount(): number;
}

export interface Capture {
    getWidth(): number;
    getHeight(): number;
    getParams(): Param[];
    getPreviewImage(): string;
    getCaptureSkeleton(): CaptureSkeleton;
};

export interface Camera {
    getCapture(): Capture;
    getName(): string;
    useCapturePlugin(pluginHandle: PluginHandle): void;
    setName(name: string): void;
    startPreview(): void;
    stopPreview(): void;
    startTracking(): void;
    stopTracking(): void;
};

export interface Engine {
    update(deltaTime: number);
    createCamera(): Camera;
    destroyCamera(camera: Camera);
    getCameras(): Camera[];
    getCamera(): Camera;
    getPlugins(): PluginDescription[];
    getPlugin(): PluginDescription;
};

export class Wrapper {
    private bridge: any;

    constructor(bridge: any) {
        this.bridge = bridge;
    }

    loadEngine(path: string): Promise<Engine> {
        return new Promise((resolve, reject) => {
            try {
                const engine = this.bridge.createEngine(path);
                if (engine) {
                    resolve(engine as Engine);
                } else {
                    reject("Cannot load engine from '" + path + "'");
                }
            } catch (e) { reject(e); }
        });
    }
}

export async function loadWrapper(path: string): Promise<Wrapper> {
    return new Promise(async (resolve, reject) => {
        try {
            const bridge = await import(path);
            resolve(new Wrapper(bridge));
        } catch (e) { reject(e); }
    });
}