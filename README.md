# embedded-template-stm32

*TODO:*
1. Update all badges with the correct repository name.
2. Replace `embedded-template-stm32` anywhere needed.

![GitHub stable release (latest by date)](https://img.shields.io/github/v/release/Arrow-air/embedded-template-stm32?sort=semver&color=green)
![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/Arrow-air/embedded-template-stm32?include_prereleases)
![Sanity Checks](https://github.com/arrow-air/embedded-template-stm32/actions/workflows/sanity_checks.yml/badge.svg?branch=develop)
![Arrow DAO
Discord](https://img.shields.io/discord/853833144037277726?style=plastic)

## :telescope: Overview

This is a template project.

Directory:
- `src/`: Project software
- `docs/`: Module Documentation
- `.env.repo`: Important environment variables
    - `STM32_MCU`: The board used for this project

## :running: First Steps

1) Give your system ST-LINK udev permissions:
    ```
    # Only need to run once forever
    sudo ./udev.sh

    # Replug in your board
    ```
2) Check the `STM32_MCU` variable in `.env.repo` and confirm that you are targeting the correct platform.

## :sparkles: Make Targets

To ensure consistent build and test outputs, Arrow provides several Docker images with all required software installed to build and test STM32 projects.

Using the Makefile, you can easily test and build your code.

```bash
# Build Locally
make build

# Flash to board connected by USB
make flash

# Cleanup
make clean

# Format code
make tidy

# Run spell check (update .cspell.project_words.txt)
make cspell-test
```

There are additional make targets available. You can find all possible targets by running make without a target or use `make help`.

## :books: Documentation
The following documents are relevant to this project:
- [Concept of Operations](./docs/conops.md)
- [Software Design Document (SDD)](./docs/sdd.md)
- [Interface Control Document (ICD)](./docs/icd.md)

## :busts_in_silhouette: Arrow DAO
Learn more about us:
- [Website](https://www.arrowair.com/)
- [Arrow Docs](https://www.arrowair.com/docs/intro)
- [Discord](https://discord.com/invite/arrow)
