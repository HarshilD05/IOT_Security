#include<iostream>

using namespace std;

class CeasarCipher {
    int _shift;
public : 

    CeasarCipher(int shift = 3): _shift(shift % 26) {}

    void set_shift(int shift) {
        _shift = shift % 26;
    }

    string encrypt(const string& str) {
        string crypt;
        for (char c : str) {
            if (isalpha(c) ) {
                if (islower(c) ) crypt.push_back('a'+(c-'a'+_shift)%26);
                else crypt.push_back('A'+(c-'A'+_shift)%26);
            }
            else if (isdigit(c) ) crypt.push_back('0'+(c-'0'+_shift)%10);
            else crypt.push_back(c); 
        }

        return crypt;
    }

    string decrypt(const string& crypt_str) {
        string txt;
        int char_d_shift = 26-_shift;
        int digit_d_shift = 10-(_shift%10);

        for (char c : crypt_str) {
            char val;
            if (isalpha(c) ) {
                if (islower(c) ) txt.push_back('a'+(c-'a'+char_d_shift)%26);
                else txt.push_back('A'+(c-'A'+char_d_shift)%26);
            }
            else if (isdigit(c) ) txt.push_back('0'+(c-'0'+digit_d_shift)%10);
            else txt.push_back(c);
        }

        return txt;
    }

};

int main () {
    string og_text = "hello World! I am 20 yrs old...";
    CeasarCipher cryptor(25);

    string crypt_txt = cryptor.encrypt(og_text);
    string decrypt_txt = cryptor.decrypt(crypt_txt);

    cout<<"OG text : "<< og_text <<endl;
    cout<<"Encrypted text : "<< crypt_txt <<endl;
    cout<<"Decrypted text : "<< decrypt_txt <<endl;

    return 0;
}