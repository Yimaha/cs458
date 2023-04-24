
import base64 as b64

def to_b64(s: str):
    return b64.b64encode(s.encode('ascii')).decode('ascii')

def to_b64_raw(bytes):
    return b64.b64encode(bytes).decode('ascii')


def from_b64(code: str):
    return b64.b64decode(code.encode('ascii')).decode('ascii')

def from_b64_raw(bytes):
    return b64.b64decode(bytes)