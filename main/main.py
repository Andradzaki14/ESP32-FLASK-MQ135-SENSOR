from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

data_list = []

@app.route('/sensor/data', methods=['POST'])
def receive_data():
    gas_level = request.form.get('gas_level')

    if gas_level is not None:
        try:
            gas_level = float(gas_level)
            data = {"gas_level": gas_level}
            data_list.append(data)
            return jsonify({"message": "Data received"}), 200
        except ValueError:
            return jsonify({"message": "Invalid data format"}), 400
    else:
        return jsonify({"message": "Missing parameters"}), 400

@app.route('/data', methods=['GET'])
def get_data():
    return jsonify(data_list), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
