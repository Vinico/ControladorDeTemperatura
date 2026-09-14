from flask import Flask, request, jsonify
from datetime import datetime

app = Flask(__name__)


@app.route('/temperatura', methods=['POST'])
def receber_temperatura():
    dados = request.get_json()

    # Valida se o JSON possui a chave 'temperatura'
    if not dados or 'temperatura' not in dados:
        return jsonify({"status": "erro", "mensagem": "Dados inválidos"}), 400

    valor_temp = dados['temperatura']
    horario = datetime.now().strftime("%H:%M:%S")

    print(f"[{horario}] Temperatura recebida: {valor_temp}°C")



    return jsonify({"status": "sucesso", "temperatura_recebida": valor_temp}), 200


if __name__ == '__main__':
    # host='0.0.0.0' loopback
    app.run(host='0.0.0.0', port=5000)