#include<iostream>
#include<stdint.h>
#include<vector>
#include<exception>

using namespace std;

class RailRoadCipher{
    unsigned int _cols;

    vector<size_t> _calculate_char_distribution(size_t len) {
        // 1 Zig Zag cycle from 123432 -> n + n - 1
        size_t cycle_len = (_cols<<1)-2;
        // Calculate the number of chars in each rail
        size_t complete_cycles = len / cycle_len;

        vector<size_t> rail_lens(_cols);
        rail_lens[0] = complete_cycles;
        rail_lens.back() = complete_cycles;

        for (uint32_t i = 1; i < _cols-1; i++) {
            rail_lens[i] = (complete_cycles << 1);    
        }

        // Add Residual Chars in last incomplete Cycle
        size_t rem = len % cycle_len;
        if (rem) {
            for (uint32_t i = 0; i < rem && i < _cols; i++) {
                rail_lens[i]++;
            }

            if (rem > _cols) {
                rem -= _cols;
                for (uint32_t i = _cols-1; i >= 0 && rem > 0; i--, rem--) {
                    rail_lens[i]++;
                }
            }
        }

        return rail_lens;
    }
public : 

    RailRoadCipher(unsigned int cols = 2) {
        set_cols(cols);
    }

    void set_cols(unsigned int cols) {
        if (cols < 2) throw invalid_argument("Number of Columns for RailRoad Cipher must be >= 2.");
        _cols = cols;
    }

    string encrypt(const string& str) {
        if (str.length() == 0) return "";

        vector<string> rails(_cols);
        bool up = true;
        uint32_t i = 0;

        for (char c : str) {
            rails[i].push_back(c);

            // Next rail Select Zig Zag pattern
            if (up) {
                i++;
                if (i == _cols-1) up = false;
            }
            else {
                i--;
                if (i == 0) up = true;
            }
        }

        string crypt;
        for (string s : rails) {
            crypt.append(s);
        } 

        return crypt;
    }

    string decrypt(const string& crypt_str) {
        if (crypt_str.length() == 0) return "";
        
        vector<size_t> rail_lens = _calculate_char_distribution(crypt_str.length() );
        vector<string> rails(_cols);
        size_t s_idx = 0;
        // Append Substrings of Length l to the rails
        for (unsigned int i = 0; i < _cols; i++) {
            rails[i] = crypt_str.substr(s_idx, rail_lens[i]);
            s_idx += rail_lens[i];
        }

        // Decrypt ZigZag Rails
        string txt;
        vector<size_t> idxs(_cols);
        uint32_t r = 0;
        bool up = true;

        for (size_t i = 0; i < crypt_str.length(); i++) {
            // Get the index for the  
            size_t c = idxs[r];
            
            txt.push_back(rails[r][c]);
            
            idxs[r]++;
            
            
            // Next rail Select Zig Zag pattern
            if (up) {
                r++;
                if (r == _cols-1) {
                    up = false;
                }
            }
            else {
                r--;
                if (r == 0) {
                    up = true;
                }
            }
        }

        return txt;
    }

};

int main () {
    string og_text = "hello World! I am 20 yrs old...";
    unsigned int rails = 7;
    
    cout<<"OG text : "<< og_text <<endl;
    
    RailRoadCipher cryptor(rails);

    string crypt_txt = cryptor.encrypt(og_text);
    cout<<"Encrypted text : "<< crypt_txt <<endl;

    string decrypt_txt = cryptor.decrypt(crypt_txt);
    cout<<"Decrypted text : "<< decrypt_txt <<endl;

    return 0;
}