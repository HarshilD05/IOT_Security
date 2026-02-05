#include<iostream>
#include<vector>
#include<unordered_map>

using namespace std;

class PlayFairCipher {
    vector<vector<char>> _mat;
    unordered_map<char, pair<int, int>> _mat_pos;
    string _key;
    char _filler;

    void _generate_matrix() {
        _mat = vector<vector<char>>(6, vector<char>(6));
        vector<bool> char_occured(26);
        vector<bool> digit_occured(10);

        int row = 0, col = 0;
        // Unique key Chars
        for (char c : _key) {
            // Only process Alpha Numeric Characters in the Key
            if (isalnum(c) ) {
                // Numbers
                if (isdigit(c) ) {
                    int idx = c-'0';
                    // New Unique Digit
                    if (!digit_occured[idx]) {
                        digit_occured[idx] = true;
                    }

                    _mat[row][col] = c;
                    _mat_pos.insert({c, {row, col}});
    
                    // Update Insert index
                    col++;
                    if (col == 6) {
                        row++;
                        col = 0;
                    }
                }
                // Characters
                else {
                    c = tolower(c);
                    int idx = c-'a';

                    // If New unique char
                    if (!char_occured[idx]) {
                        char_occured[idx] = true;
                        _mat[row][col] = c;
                        _mat_pos.insert({c, {row, col}});
        
                        // Update Insert index
                        col++;
                        if (col == 6) {
                            row++;
                            col = 0;
                        }
                    }
                }
            }
        }

        // Remaining Chars
        for (int i = 0; i < 26; i++) {
            if (!char_occured[i]) {
                _mat[row][col] = 'a'+i;
                _mat_pos.insert({'a'+i, {row, col} });

                // Update index
                col++;
                if (col == 6) {
                    col = 0;
                    row++;
                }
            }

        }

        // Remaining Digits
        for (int i = 0; i < 10; i++) {
            if (!digit_occured[i]) {
                _mat[row][col] = '0'+i;
                _mat_pos.insert({'0'+i, {row, col} });
                
                // Update index
                col++;
                if (col == 6) {
                    col = 0;
                    row++;
                }
            }
        }
    }

    pair<char, char> _get_encrypted(char c1, char c2) {
        pair<int,int> p1 = _mat_pos[c1];
        pair<int,int> p2 = _mat_pos[c2];

        // Same Row
        if (p1.first == p2.first) {
            int col_c1 = (p1.second+1) % 6;
            int col_c2 = (p2.second+1) % 6;
            return {_mat[p1.first][col_c1], _mat[p2.first][col_c2]};
        }
        // Same Col
        else if (p1.second == p2.second) {
            int r_c1 = (p1.first+1) % 6;
            int r_c2 = (p2.first+1) % 6;
            return { _mat[r_c1][p1.second], _mat[r_c2][p2.second] };
        }
        // Diagonal
        else {
            return { _mat[p1.first][p2.second], _mat[p2.first][p1.second] };
        }
    }

    pair<char, char> _get_decrypted(char c1, char c2) {
        pair<int,int> p1 = _mat_pos[c1];
        pair<int,int> p2 = _mat_pos[c2];

        // Same Row
        if (p1.first == p2.first) {
            int col_c1 = p1.second-1;
            if (col_c1 < 0) col_c1 = 5;

            int col_c2 = p2.second-1;
            if (col_c2 < 0) col_c2 = 5;

            return {_mat[p1.first][col_c1], _mat[p2.first][col_c2]};
        }
        // Same Col
        else if (p1.second == p2.second) {
            int r_c1 = p1.first-1;
            if (r_c1 < 0) r_c1 = 5;

            int r_c2 = p2.first-1;
            if (r_c2 < 0) r_c2 = 5;

            return { _mat[r_c1][p1.second], _mat[r_c2][p2.second] };
        }
        // Diagonal
        else {
            return { _mat[p1.first][p2.second], _mat[p2.first][p1.second] };
        }
    }

public : 

    PlayFairCipher(string key, char filler = 'q'): _key(key), _filler(tolower(filler) ) {
        _generate_matrix();
    }

    void set_key(string key) {
        _key = key;
        _generate_matrix();
    }

    void set_filler(char c) {
        _filler = c;
    }

    string encrypt(const string& str) {
        string crypt;

        // Two pointers to avoid whitespace
        int l = 0, r = 1;
        // Start with alnum char
        while (l < str.length() && !isalnum(str[l]) ) {
            crypt.push_back(str[l]);
            l++;
        }

        pair<char, char> crypt_pair;

        // Begin normal Cipher 
        while (l < str.length() ) {
            // Append l non alnum and move forward
            while (l < str.length() && !isalnum(str[l]) ) {
                crypt.push_back(str[l]);
                l++;
            }
            r = l+1;

            // Safe Guard
            if (l == str.length() ) break;

            // Ignore r pos non alnum and move forward
            while (r < str.length() && !isalnum(str[r]) ) {
                r++;
            }

            // If End of str reached or duplicate character pair
            // Use filler char with l pos char
            if (r == str.length() || str[r] == str[l]) {
                crypt_pair = _get_encrypted(tolower(str[l]), _filler );
            }
            else {
                crypt_pair = _get_encrypted(tolower(str[l]), tolower(str[r]) );
            }

            // Append Encrypted chars
            crypt.push_back(crypt_pair.first);
            // Add the in between whitespace
            for (int i = l+1; i < r; i++) {
                crypt.push_back(str[i]);
            }
            crypt.push_back(crypt_pair.second);

            // update next pair start
            l = (str[r] == str[l])? r : r+1 ;
            // update next pair second pos
            r = l+1;
        } 

        return crypt;
    }

    string decrypt(const string& crypt_str) {
        string txt;

        // Two pointers to avoid whitespace
        int l = 0, r = 1;
        // Start with alnum char
        while (l < crypt_str.length() && !isalnum(crypt_str[l]) ) {
            txt.push_back(crypt_str[l]);
            l++;
        }

        pair<char, char> decrypt_pair;

        // Begin normal Cipher 
        while (l < crypt_str.length() ) {
            // Append l non alnum and move forward
            while (l < crypt_str.length() && !isalnum(crypt_str[l]) ) {
                txt.push_back(crypt_str[l]);
                l++;
            }
            r = l+1;

            // Safe Guard
            if (l == crypt_str.length() ) break;

            // Ignore r pos non alnum and move forward
            while (r < crypt_str.size() && !isalnum(crypt_str[r]) ) {
                r++;
            }
            
            decrypt_pair = _get_decrypted(crypt_str[l], crypt_str[r] );
            
            // Append Encrypted chars
            txt.push_back(decrypt_pair.first);
            // Add the in between non alnum
            for (int i = l+1; i < r; i++) {
                txt.push_back(crypt_str[i]);
            }
            txt.push_back(decrypt_pair.second);

            // Update next pair start
            l = r+1;
            // update next pair second pos
            r = l+1;
        }

        return txt;
    }

};

int main () {
    string og_text = "  !@#  hello World! I am 20 yrs old...";
    string key = "HelpMe";

    cout<<"OG text : "<< og_text <<endl;

    PlayFairCipher cryptor(key);

    string crypt_txt = cryptor.encrypt(og_text);
    cout<<"Encrypted text : "<< crypt_txt <<endl;
    
    string decrypt_txt = cryptor.decrypt(crypt_txt);
    cout<<"Decrypted text : "<< decrypt_txt <<endl;

    return 0;
}