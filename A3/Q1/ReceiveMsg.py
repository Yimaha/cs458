import requests
import utils


url = "https://hash-browns.cs.uwaterloo.ca/api/plain/inbox"
headers = {"Accept": "application/json", "Content-Type" : "application/json"}
body = {
    "api_token": "df14488ca42b4cdd09e8ceed78a0cfcdbbe1b8b6a3417841b8e6e60460f38e37",
    # "recipient": "Batman", 
    # "msg": utils.to_b64("damn son where did you find this")
}
print(body)
x = requests.post(url=url, headers=headers, json=body)

print("the status code is:", x.status_code)
print(utils.from_b64(x.json()[0]["msg"]))
