import Engine from './Engine';

export class Wrapper {
    private bridge: any;

    constructor(bridge: any) {
        this.bridge = bridge;
    }

    async loadEngine(path: string): Promise<Engine|null> {
        try {
            const engine = await this.bridge.loadEngine(path);
            return new Engine(engine);
        } catch (e) {
            console.error(e);
        }
        return null;
    }
}

export async function loadWrapper(path: string): Promise<Wrapper|null> {
    try {
        const bridge = await import(path);
        return new Wrapper(bridge);
    } catch (e) {
        console.error(e);
    }
    return null;
}