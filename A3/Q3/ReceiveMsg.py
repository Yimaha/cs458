import requests
import utils
from nacl import pwhash, secret
import nacl


print("hashing password, Q3")


preshared_psward = b"identical prospect"

salt = bytes.fromhex("5a5bf7c9ae4ba859a96074ab5519c5ce")

operation_lim = 2
mem_lim = 67108864

encrypted = pwhash.argon2id.kdf(size=secret.SecretBox.KEY_SIZE, password=preshared_psward, salt=salt, opslimit=operation_lim, memlimit=mem_lim)
box = secret.SecretBox(encrypted)

# msg = utils.to_b64_raw(box.encrypt(msg, nonce))

# encrypted = box.encrypt(b"some message")
url = "https://hash-browns.cs.uwaterloo.ca/api/psp/inbox"
headers = {"Accept": "application/json", "Content-Type" : "application/json"}
body = {
    "api_token": "df14488ca42b4cdd09e8ceed78a0cfcdbbe1b8b6a3417841b8e6e60460f38e37",
}
x = requests.post(url=url, headers=headers, json=body)

rawByte = utils.from_b64_raw(x.json()[0]["msg"])

plaintext = box.decrypt(rawByte)
print(plaintext.decode('utf-8'))
