#include <bits/stdc++.h>
using namespace std;

long long convertBase(const string &value, int base) {
    long long num = 0;
    for (char c : value) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = tolower(c) - 'a' + 10;
        else continue;
        if (digit >= base) {
            cerr << "Invalid digit '" << c << "' for base " << base << "\n";
            return 0;
        }
        num = num * base + digit;
    }
    return num;
}

long double lagrangeInterpolation(const vector<pair<long long,long long>> &points) {
    long double secret = 0.0;
    int k = points.size();
    for (int i = 0; i < k; i++) {
        long double term = points[i].second;
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0 - points[j].first);
                term /= (points[i].first - points[j].first);
            }
        }
        secret += term;
    }
    return secret;
}

long long processJSON(const string &json) {
    int n = 0, k = 0;
    vector<pair<long long,long long>> points;

    smatch match;
    regex_search(json, match, regex("\"n\"\\s*:\\s*(\\d+)"));
    if (!match.empty()) n = stoi(match[1]);

    regex_search(json, match, regex("\"k\"\\s*:\\s*(\\d+)"));
    if (!match.empty()) k = stoi(match[1]);

    regex keyRegex("\"(\\d+)\"\\s*:\\s*\\{[^}]?\"base\"\\s:\\s*\"(\\d+)\"[^}]?\"value\"\\s:\\s*\"([^\"]+)\"");
    auto keysBegin = sregex_iterator(json.begin(), json.end(), keyRegex);
    auto keysEnd = sregex_iterator();

    for (auto it = keysBegin; it != keysEnd; ++it) {
        int x = stoi((*it)[1]);
        int base = stoi((*it)[2]);
        string value = (*it)[3];
        long long y = convertBase(value, base);
        points.push_back({x, y});
    }

    sort(points.begin(), points.end());
    if (points.size() < k) {
        cerr << "Not enough points extracted from JSON.\n";
        return 0;
    }

    vector<pair<long long,long long>> subset(points.begin(), points.begin()+k);
    long double secret = lagrangeInterpolation(subset);
    return (long long)llround(secret);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream file("input.json");
    string jsonContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    size_t splitPos = jsonContent.find("}{");
    string json1 = jsonContent.substr(0, splitPos+1);
    string json2 = jsonContent.substr(splitPos+1);

    cout << processJSON(json1) << "\n";
    cout << processJSON(json2) << "\n";

    return 0;
}
