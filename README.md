# FDWaveShaper32

FDWaveShaper32 is an audio processing plugin designed to offer sound shaping and enhancement capabilities. It’s partly a mastering tool, combining multiple signal-processing techniques into a single interface. This project was developed in 2024 as part of an MSc in Sound and Music Computing at Maynooth University, Ireland.

## Features

- **High-Quality Signal Processing**: 4x oversampling on the effected signal path.
- **Customisable Waveshaping**: Unique waveshaping algorithm for harmonic enhancement.
- **Tonal Balance Control**: Incorporates a Tilt EQ.
- **High Frequency Taming**: Variable lowpass filter for smooth high-frequency attenuation.
- **Parallel Dynamics Processing**: Two fixed parallel compression settings.
- **Clipping**: Final-stage clipper for peak limiting and saturation.
- **User Interface**: User-friendly GUI for efficient workflow and real-time adjustments.

## Technical Implementation

FDWaveShaper32 is built using the JUCE framework and developed in C++, ensuring cross-platform compatibility and high performance.

## Third-Party Contributions

The development of FDWaveShaper32 has benefited from the open-source community. Key components have been adapted or inspired by the following projects:

- "Sonnox Inflator" by Paul Frindle.
- "Peak Eater" Clipper by Vladyslav Voinov.
- Tilt-eq Plugin by Jacob Curtis.
- Lowpass/Highpass Filter implementations by Jan Wilczek.
- SimpleCompressor by Daniel Rudrich.
- "ff meters" by Foleys Finest Audio Ltd. - Daniel Walz

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/FDWaveShaper32.git
    ```
2. Open the project in your preferred development environment.
3. Build the project according to the provided build scripts.

## Usage

To use FDWaveShaper32, load the plugin in your digital audio workstation (DAW) or use it as a stand-alone application. Adjust the parameters to achieve the desired audio effects.

## License

This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

Special thanks to the open-source community and the developers of the libraries and projects that made this plugin possible.

