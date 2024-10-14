#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <cassert>
#include <functional>
#include <fstream>
#include <memory>
#include <thread>

using namespace std;

constexpr int ConsoleNumColumns = 200;
constexpr int ConsoleNumRows = 40;

// Author: // TODO: eigenen Namen hinschreiben

struct Header {
    char riff[4];
    uint32_t fileSize;
    // TODO: Erweitern Sie diese Struktur für den Dateikopf.
};

vector<vector<int16_t>> read(const string& filename, Header& header, int32_t &sampleRate, int32_t &sampleCount, int16_t &channelCount) {
    ifstream ifs;

    ifs.open(filename, ios::binary);

    if (ifs) {
        // TODO: Lesen Sie den Dateikopf und den Datenteil ein und füllen Sie die Rückgabewerte aus.

        cout << "RIFF: " << header.riff[0] << header.riff[1] << header.riff[2] << header.riff[3] << endl;
        assert(header.riff[0] == 'R' && header.riff[1] == 'I' && header.riff[2] == 'F' && header.riff[3] == 'F');
        cout << "file size: " << header.fileSize << endl;

        unique_ptr<int16_t[]> samples = make_unique<int16_t[]>(0); // TODO: Erstellen Sie hier einen Array der korrekten Grösse und lesen Sie den Datenteil der Datei da hinein.

        vector<vector<int16_t>> rearrangedSamples(channelCount);
        uint32_t s = 0;
        for (int32_t sample = 0; sample < sampleCount; sample++) {
            for (int16_t channel = 0; channel < channelCount; channel++) {
                rearrangedSamples[channel].push_back(samples[s++]);
            }
        }
        return rearrangedSamples;
    } else {
        cout << "File " << filename << " could not be opened for reading." << endl;
        return vector<vector<int16_t>>();
    }
}

vector<int> summarize(const vector<int16_t> &samples, int from, int until, int numBuckets) {
    // TODO: Fassen Sie die samples von Index `from` bis Index `until` (exklusive) in `numBuckets` vielen Einträgen zusammen
    // indem Sie die durchschnittliche Distanz von einem Sample zum nächsten innerhalb des Buckets bestimmen.
    // Achtung: Die Indizes `from` und `until` sind nicht zwingend gültig, können also ausserhalb von [0, samples.size()) liegen.
    vector<int> result(numBuckets);
    return result;
}

// Abspielfunktion für die Audiosamples. Diese Funktion ruft `summarize` auf und ist bereits fertig implementiert.
void play(const vector<vector<int16_t>> &samples, const int32_t sampleRate) {
    const size_t sampleCount = samples[0].size();
    const size_t channelCount = samples.size();
    const auto timeDelta = 200ms;
    const auto duration = 1000ms*sampleCount/sampleRate;
    const auto start = chrono::system_clock::now();
    
    for (auto time = 0ms; time < duration; time += timeDelta) {
        // Wait until it is time to show the next frame.
        const auto now = chrono::system_clock::now();
        const auto diff = now - start;
        const auto waitTime = (time > diff) ? time - diff : 0ms;

        this_thread::sleep_for(waitTime);
#ifdef _WIN64
        // Clear the screen to present the next frame.
        system("cls");
#else
        // "Clear" the screen by adding some empty lines.
        for (int i = 0; i < ConsoleNumRows; i++) {
            cout << endl;
        }
#endif
        const int64_t startSample = (time - 1000ms)*sampleRate/1000ms;
        const int64_t endSample = (time + 1000ms)*sampleRate/1000ms;
        cout << "Visualization from sample " << startSample << " (" << startSample/sampleRate << "s) to sample " << endSample  << " (" << endSample/sampleRate << "s)." << endl;
        const int numRows = int(ConsoleNumRows/2/channelCount);
        const int scalingFactor = 500;
        for(int channel = 0; channel < channelCount; channel++) {
            vector<int> waveform = summarize(samples[channel], (int)startSample, (int)endSample, ConsoleNumColumns);
            cout << endl;
            for(int y = numRows; y >= -numRows; y--) {
                for(int x = 0; x < ConsoleNumColumns; x++) {
                    if (abs(waveform[x])*numRows/scalingFactor >= abs(y)){
                        cout << "X";
                    } else {
                        cout << " ";
                    }
                }
                cout << endl;
            }
            cout << endl;
         }
    }
}

bool write(const string& filename, const vector<vector<int16_t>>& samples, Header header) {
    const size_t sampleCount = samples[0].size();
    const size_t channelCount = samples.size();

    ofstream ofs;

    ofs.open(filename, ios::binary);

    if (ofs) {
        // TODO: Schreiben Sie die Samples mit dem korrekten WAVE-Header.
        return true;
    } else {
        cout << "File " << filename << " could not be opened for writing." << endl;
        return false;
    }
}

vector<vector<int16_t>> addEcho(const vector<vector<int16_t>> &inputSamples, const int32_t sampleRate) {
    const size_t sampleCount = inputSamples[0].size();
    const size_t channelCount = inputSamples.size();

    vector<vector<int16_t>> outputSamples(channelCount);
    // TODO: Fügen Sie eine zeitverzögerte, abgeschwächte Kopie der `inputSamples` zu den `inputSamples` hinzu,
    // und geben Sie diese Version mit Echo zurück.
    return outputSamples;
}

// Langsame Vorlage zur Unterdrückung von Pausen in den `inputSamples`.
vector<vector<int16_t>> suppressPause(const vector<vector<int16_t>> &inputSamples, const int32_t sampleRate) {
    const size_t sampleCount = inputSamples[0].size();
    const size_t channelCount = inputSamples.size();

    vector<vector<int16_t>> outputSamples(channelCount);
    const auto averagingDuration = 100ms;
    const uint64_t averagingSamples = averagingDuration*sampleRate/1000ms;
    const uint64_t averagingSamplesD2 = averagingSamples/2;
    vector<uint16_t> averages;

    for (size_t sample = 0; sample < sampleCount; sample++) {
        const size_t minPos = (sample >= averagingSamplesD2) ? sample - averagingSamplesD2 : 0;
        const size_t maxPos = min((uint64_t) sampleCount, sample + averagingSamplesD2) - 1;
        uint32_t sum = 0;
        uint32_t count = 0;

        for (size_t pos = minPos; pos < maxPos; pos++) {
            for (int channel = 0; channel < channelCount; channel++) {
                sum += abs(inputSamples[channel][pos + 1] - inputSamples[channel][pos]);
                count++;
            }
        }
        averages.push_back(count ? sum/count : 0);
    }
    int sumOfAverages = 0;
    for (auto average : averages) {
        sumOfAverages += average;
    }
    const uint16_t averageOfAverages = (uint16_t)(sumOfAverages/averages.size());
    const uint16_t low = averageOfAverages/5;

    for (size_t sample = 0; sample < sampleCount; sample++) {
        if (averages[sample] >= low) {
            for (size_t channel = 0; channel < channelCount; channel++) {
                outputSamples[channel].push_back(inputSamples[channel][sample]);
            }
        }
    }
    return outputSamples;
}

vector<vector<int16_t>> suppressPausePrefixSum(const vector<vector<int16_t>> &inputSamples, const int32_t sampleRate) {
    const size_t sampleCount = inputSamples[0].size();
    const size_t channelCount = inputSamples.size();

    vector<vector<int16_t>> outputSamples(channelCount);
    const auto averagingDuration = 100ms;
    const uint64_t averagingSamples = averagingDuration*sampleRate/1000ms;
    const uint64_t averagingSamplesD2 = averagingSamples/2;

    // TODO: Beschleunigen Sie die Berechnung der durchschnittlichen Lautstärke im 100ms Zeitfenster rund um jedes Sample mit Hilfe
    // von vorberechnenten Präfix-Summen.
    return outputSamples;
}

using WaveProcessor = std::function<std::vector<std::vector<int16_t>>(const std::vector<std::vector<int16_t>>&, const int32_t)>;

// Hilfsfunktion zur Zeitmessung der einzelnen Audioverarbeitungsfunktionen.
vector<vector<int16_t>> timed(WaveProcessor processor, const string& name, const vector<vector<int16_t>>& inputSamples, const int32_t sampleRate) {
    auto start = chrono::system_clock::now();
    vector<vector<int16_t>> result = processor(inputSamples, sampleRate);
    auto end = chrono::system_clock::now();
    chrono::duration<double> diff = end - start;
    cout << "computing " << name << " took " << setw(9) << diff.count() << " seconds and resulted in " << result[0].size() << " samples" << endl;
    return result;
}

int main(int argc, const char*argv[]) {
    if (argc != 3) {
        cout << "Usage: audio command filename.wav" << endl;
        return -1;
    }
    string command = argv[1];
    string inputFilename = argv[2];
    Header header;
    int32_t sampleRate;
    int32_t sampleCount;
    int16_t channelCount;
    vector<vector<int16_t>> samples = read(inputFilename, header, sampleRate, sampleCount, channelCount);
    if (samples.empty()) {
        return -2;
    }

    if (command == "info") {
        cout << "FILE INFO" << endl;
        cout << "sample rate [Hz]: " << sampleRate << endl;
        cout << "sample count: " << sampleCount << endl;
        cout << "channel count: " << channelCount << endl;
        const double durationSeconds = 1.0*sampleCount/sampleRate;
        cout << "file lasts " << durationSeconds << " seconds" << endl;
        
        cout << "A few samples from five seconds into the file:" << endl;
        for(int channel = 0; channel < channelCount; channel++) {
            cout << "channel " << channel << ": ";
            const int start = 5*sampleRate;
            for(int i = start; i < start+10; i++) {
                cout << samples[channel][i] << " ";
            }
            cout << endl;
        }
    }
    if (command == "play") {
        play(samples, sampleRate);
    }
    if (command == "echo") {
        vector<vector<int16_t>> echoSamples = timed(&addEcho, "add echo", samples, sampleRate);
        write("echo_" + inputFilename, echoSamples, header);
    }
    if (command == "shorten") {
        vector<vector<int16_t>> pauseFreeSamples = timed(&suppressPause, "suppress pause", samples, sampleRate);
        write("short_slow_" + inputFilename, pauseFreeSamples, header);
        vector<vector<int16_t>> pauseFreeSamplesPrefixSum = timed(&suppressPausePrefixSum, "suppress pause with prefix sum", samples, sampleRate);
        write("short_fast_" + inputFilename, pauseFreeSamplesPrefixSum, header);
    }    
 }
