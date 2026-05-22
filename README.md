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

### Example

```bash
$ ./mache "hello world"
mashed: 3f2a1bd4e8c6a092f1e3b5d7c9a0f2e4b6d8c1a3e5f7b9d2c4a6e8f0b2d4c6a8
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
