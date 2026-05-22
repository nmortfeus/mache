# mache

An experimental 256-bit hashing function written in C, built from scratch as a learning project.
Implements custom diffusion, confusion and avalanche techniques.

## Build

```bash
git clone https://github.com/nmortfeus/mache
cd mache
make
```

## Usage

```bash
./mache <text>
```

```
./mache "hello world"
mached: 347ec9451244b7cb6cac3e21c3d121d7326392832d94c53f1e1bc91b5324adc8
```

## How it works

Each input is split into 32-byte blocks, then put through 10 000 iterations of:
- **Circular shift** — each byte is rotated based on its neighbor
- **Rotation** — bytes are rotated across the block
- **Diffusion** — blocks are XORed together
- **Expansion** — each byte is mixed with the last

The final hash is produced by XORing all blocks into a 256-bit (32-byte) output.

## License

MIT
