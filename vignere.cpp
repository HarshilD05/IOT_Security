#include<iostream>
#include<stdint.h>
#include<vector>

using namespace std;

class VignereCipher{
    vector<uint8_t> _key;
public : 

    VignereCipher(string key) {
        set_key(key);
    }

    void set_key(string key) {
        _key.clear();
        for (char c : key) {
            if (isalpha(c) ) {
                c = tolower(c);
                _key.push_back(c-'a');
            }
            else if (isdigit(c) ) _key.push_back(c-'0');
        }
    }

    string encrypt(const string& str) {
        string crypt;
        size_t i = 0;

        for (char c : str) {
            if (isalpha(c) ) {
                if (islower(c) ) crypt.push_back('a'+(c-'a'+_key[i])%26);
                else crypt.push_back('A'+(c-'A'+_key[i])%26);
            }
            else if (isdigit(c) ) crypt.push_back('0'+(c-'0'+_key[i])%10);
            else crypt.push_back(c);

            i++;
            i %= _key.size();
        }

        return crypt;
    }

    string decrypt(const string& crypt_str) {
        string txt;
        int char_d_shift;
        int digit_d_shift;
        int i = 0;

        for (char c : crypt_str) {
            char val;
            if (isalpha(c) ) {
                char_d_shift = 26 - _key[i];
                if (islower(c) ) txt.push_back('a'+(c-'a'+char_d_shift)%26);
                else txt.push_back('A'+(c-'A'+char_d_shift)%26);
            }
            else if (isdigit(c) ) {
                digit_d_shift = 10 - _key[i]%10;
                txt.push_back('0'+(c-'0'+digit_d_shift)%10);
            }
            else txt.push_back(c);

            i++;
            i %= _key.size();
        }

        return txt;
    }

};

int main () {
    string og_text = "hello World! I am 20 yrs old...";
    string key = "Crypt123";
    VignereCipher cryptor(key);

    string crypt_txt = cryptor.encrypt(og_text);
    string decrypt_txt = cryptor.decrypt(crypt_txt);

    cout<<"OG text : "<< og_text <<endl;
    cout<<"Encrypted text : "<< crypt_txt <<endl;
    cout<<"Decrypted text : "<< decrypt_txt <<endl;

    return 0;
}