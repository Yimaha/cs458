import requests
import utils
import nacl.secret
import nacl.utils


print("preshared key, Q2")

secret_shared_key = bytes.fromhex("f9ff70cc63fbd898cc8235d40330b6d30b041c779c71fb892190a2f133b2975c")
print(len(secret_shared_key))
box = nacl.secret.SecretBox(secret_shared_key)
# encrypted = box.encrypt(b"some message")

url = "https://hash-browns.cs.uwaterloo.ca/api/psk/inbox"
headers = {"Accept": "application/json", "Content-Type" : "application/json"}
body = {
    "api_token": "df14488ca42b4cdd09e8ceed78a0cfcdbbe1b8b6a3417841b8e6e60460f38e37",
}
x = requests.post(url=url, headers=headers, json=body)

print("the status code is:", x.status_code)
print(x.json()[0])
rawByte = utils.from_b64_raw(x.json()[0]["msg"])

plaintext = box.decrypt(rawByte)
print(plaintext.decode('utf-8'))
