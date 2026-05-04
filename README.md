# REAL-EL RM-780 Gaming RGB — Linux Driver

Open-source Linux driver for REAL-EL RM-780 Gaming RGB mouse

> ⚠️ Work in progress

## About

REAL-EL doesn't provide a Linux driver for this mouse, and the Windows version doesn't work under Wine. So I decided to write one from scratch in C by reverse engineering the original Windows driver. The goal is to fully support all mouse driver functionality.

## Goals

- [x] Catalogue all Windows driver features
- [x] Reverse engineer all features
- [ ] Implement backend (C + libusb)
- [ ] Implement frontend (GUI/CLI)

## License

MIT
