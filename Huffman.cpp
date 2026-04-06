#include <bits/stdc++.h>
using namespace std;

struct Node {
    int freq;
    char ch;
    Node* left;
    Node* right;
    Node(int f, char c = '\0', Node* l = nullptr, Node* r = nullptr)
        : freq(f), ch(c), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

class Codec {
    unordered_map<char, int> frequency;
    unordered_map<char, string> codes;
    unordered_map<string, char> reverse_codes;

    void buildFrequency(const string& data) {
        for (char ch : data) frequency[ch]++;
    }

    void generateCodes(Node* root, string code) {
        if (!root) return;
        if (!root->left && !root->right) {
            codes[root->ch] = code;
            reverse_codes[code] = root->ch;
        }
        generateCodes(root->left, code + "0");
        generateCodes(root->right, code + "1");
    }

    void serialize(Node* root, string& tree_string) {
        if (!root) return;
        if (!root->left && !root->right) {
            tree_string += "'";
            tree_string += root->ch;
            return;
        }
        tree_string += "0";
        serialize(root->left, tree_string);
        tree_string += "1";
        serialize(root->right, tree_string);
    }

    Node* deserialize(const string& tree_string, int& index) {
        if (tree_string[index] == '\'') {
            index++;
            char ch = tree_string[index++];
            return new Node(0, ch);
        }
        index++; // skip '0'
        Node* left = deserialize(tree_string, index);
        index++; // skip '1'
        Node* right = deserialize(tree_string, index);
        return new Node(0, '\0', left, right);
    }

public:
    pair<string, string> encode(string data) {
        frequency.clear();
        codes.clear();
        reverse_codes.clear();
        buildFrequency(data);

        if (frequency.empty()) {
            return { "zer#", "Compression complete.\nCompressed empty string." };
        }

        if (frequency.size() == 1) {
            char ch = frequency.begin()->first;
            int count = frequency.begin()->second;
            return { "one#" + string(1, ch) + "#" + to_string(count), "Compression complete.\nSingle character input." };
        }

        priority_queue<Node*, vector<Node*>, Compare> pq;
        for (auto it = frequency.begin(); it != frequency.end(); ++it) {
            pq.push(new Node(it->second, it->first));
        }

        while (pq.size() > 1) {
            Node* left = pq.top(); pq.pop();
            Node* right = pq.top(); pq.pop();
            pq.push(new Node(left->freq + right->freq, '\0', left, right));
        }

        Node* root = pq.top(); pq.pop();
        generateCodes(root, "");

        string binary_string = "";
        for (char ch : data) {
            binary_string += codes[ch];
        }

        int padding = (8 - (binary_string.length() % 8)) % 8;
        for (int i = 0; i < padding; ++i) binary_string += '0';

        string encoded_data = "";
        for (size_t i = 0; i < binary_string.length(); i += 8) {
            string byte = binary_string.substr(i, 8);
            bitset<8> b(byte);
            encoded_data += static_cast<char>(b.to_ulong());
        }

        string tree_string = "";
        serialize(root, tree_string);
        string final_data = to_string(tree_string.length()) + "#" + to_string(padding) + "#" + tree_string + encoded_data;
\
        string msg = "Compression complete";
        return { final_data, msg };
    }

    pair<string, string> decode(string data) {
        if (data.substr(0, 4) == "zer#") {
            return { "", "Decompression complete.\nEmpty input." };
        }

        if (data.substr(0, 4) == "one#") {
            char ch = data[4];
            size_t pos = data.find('#', 5);
            int count = stoi(data.substr(pos + 1));
            return { string(count, ch), "Decompression complete.\nSingle character input." };
        }

        size_t i = 0;
        string num = "";
        while (data[i] != '#') num += data[i++];
        int tree_len = stoi(num);
        i++;

        num = "";
        while (data[i] != '#') num += data[i++];
        int padding = stoi(num);
        i++;

        string tree_string = data.substr(i, tree_len);
        string encoded_data = data.substr(i + tree_len);

        int idx = 0;
        Node* root = deserialize(tree_string, idx);

        string binary_string = "";
        for (char ch : encoded_data) {
            bitset<8> b(ch);
            binary_string += b.to_string();
        }
        binary_string = binary_string.substr(0, binary_string.length() - padding);

        string decoded = "";
        Node* curr = root;
        for (char bit : binary_string) {
            if (bit == '0') curr = curr->left;
            else curr = curr->right;

            if (!curr->left && !curr->right) {
                decoded += curr->ch;
                curr = root;
            }
        }

        return { decoded, "Decompression complete." };
    }
};


int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: huffman <encode|decode> <input_file> <output_file>\n";
        return 1;
    }

    string mode = argv[1];
    string inputPath = argv[2];
    string outputPath = argv[3];

    ifstream input(inputPath, ios::binary);
    if (!input.is_open()) {
        cerr << "Failed to open input file.\n";
        return 1;
    }

    string content((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
    input.close();

    Codec codec;
    pair<string, string> result;

    if (mode == "encode") {
        result = codec.encode(content);
    } else if (mode == "decode") {
        result = codec.decode(content);
    } else {
        cerr << "Unknown mode: use 'encode' or 'decode'.\n";
        return 1;
    }

    ofstream output(outputPath, ios::binary);
    output << result.first;
    output.close();

    cout << result.second << endl;
    return 0;
}

