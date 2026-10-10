"""Fetch a URL with browser-like headers, retrying on Cloudflare 403s.

devkitPro's package server intermittently answers 403 to scripted
requests; the same URL succeeds with a browser header and a little
patience.  Usage: python tools/_fetch.py <url> <out>
"""

import sys
import time
import urllib.request

HEADERS = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36",
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
    "Accept-Language": "en-US,en;q=0.9",
}


def fetch(url: str, out: str, tries: int = 5) -> bool:
    for attempt in range(tries):
        try:
            req = urllib.request.Request(url, headers=HEADERS)
            data = urllib.request.urlopen(req, timeout=120).read()
            with open(out, "wb") as f:
                f.write(data)
            print(f"OK  {url} -> {out} ({len(data)} bytes)")
            return True
        except Exception as e:
            wait = 2 ** attempt
            print(f"try {attempt + 1}/{tries}: {e} (waiting {wait}s)")
            time.sleep(wait)
    return False


if __name__ == "__main__":
    sys.exit(0 if fetch(sys.argv[1], sys.argv[2]) else 1)
