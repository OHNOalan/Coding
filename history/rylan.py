import requests

from datetime import datetime, timezone, timedelta

# 生成当前时间（带时区）
tz = timezone(timedelta(hours=8))  # +0800时区
local_time = datetime.now(tz).strftime("%Y-%m-%dT%H:%M:%S.%f")[:-3] + "+0800"
print(local_time)  # 输出示例：2025-05-27T23:22:06.386+0800

url = "https://api.xiaoyuzhoufm.com/v1/category/podcast/list-by-tab"

headers = {
    "host": "api.xiaoyuzhoufm.com",
    "os": "android",
    "os-version": "29",
    "manufacturer": "Xiaomi",
    "model": "MI 8",
    "resolution": "1080x2115",
    "market": "xiaomi",
    "applicationid": "app.podcast.cosmos",
    "app-version": "2.85.0",
    "app-buildno": "1221",
    "x-jike-device-id": "6e953171-04c0-4755-87e1-ee8c02ef1fa0",
    "webviewversion": "83.0.4103.101",
    "app-permissions": "100100",
    "abtest-info": "{}",
    "wificonnected": "true",
    "timezone": "Asia/Shanghai",
    "local-time": local_time,
    "x-jike-access-token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkYXRhIjoiRXpmdWpHUGRocHVZS1BJVXRkTU92eXNNUzZudmJNRWdmcG9XVHFWckp3cWZWdWM0bnIwbnEzaUxHbFBzbG9cL0ZsdldCS2ZiT1Zybk1jd01YXC8zYUFFN3FhcTRDRWoxZXhVUmd3QURieGdrS0lSd0xQK0kyNWJBNTBoeDRYbGlacXZDU0R4aHVodEpoeVhJTWxSM2tNVVwva3NuQnZZa3pKbW5JRjFjc2xpWk9va3I2Sk1OblpVYjM1cnE0c25WVVFnUmJUZXZDU3NlUHhONTlPaU80UThpcklvWXd2U3BaNHd3RjYranpMd2FsYzZuNUJzeW1VSHhHdFJ1UVRzQmRUdkF4TFdXZTc3TkcrOGVpcE9WUVVseEZRbUpIcXVqM201Y3VlaGZwWStpZEVZcGdFOVlPa211aGZ5U0RVUFhlUm9nbWlMN1psS2dYeGxKQkhiVW9pazQrTitUZHRxNEhOVUpIXC9WV2NDN1wvUUtSVjdoc2FBTWt3aEpIbngreGtUT0hweGFJcitWZzQyd1MwV1l2RGczUFZOQjROZXpyeGdFQ1BaOGFWRG5hQ0NRPSIsInYiOjMsIml2IjoicFwvR1l3K2pmck1sbCtwNTMyNUEyWnc9PSIsImlhdCI6MTc0ODM2MDYyMi40MzV9.8tU-e6OYURbbz9-3BCSS3w0XBOwojij170eUJWTlp_w",
    "x-jike-refresh-token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJkYXRhIjoiOFl3dXFYeUZOTDZJbTNOXC9maXFZOVdFS1B3T05Wc2NJa09peGpJQUcrZldJTEF4eEdYcGRsc2h1ejR4eEV0S1wvSjRVRk8zSUt4Wnd5a0hsdmtwbnRwMmN3aThXUUFXbFpaOGZOdXRzSjZ5VjFhWTBuaThLaTB5Y25jUVpibFEyTGRVQU1vbTMrRlNHMjNQNkhPS0l0M3c0SGRBN2lqZFVCNXhcLzMwTTNnTWx3YTc1UHVCWExhbmRxem1WcHpcLzZmS0ZYZEZYcFI2aExFTURnMXBOOHpyU1JNUVlWSTRUQXFPM1RlYXhDQ29tcHp0V0xFTnF6Tm5scjVmTU0yRU1seVUiLCJ2IjozLCJpdiI6IkJ0MGxjb1wvdVVsc0RLaFBqWmhIdHp3PT0iLCJpYXQiOjE3NDgzNjA2MjIuNDM1fQ.4tHWGJ4ohh0ZPwINmW5qOcJrvWUnzk8j65cK_RIlSyU",
    "content-type": "application/json;charset=utf-8",
    "content-length": "76",
    "accept-encoding": "gzip",
    "user-agent": "okhttp/4.12.0",
    "x-jike-device-properties": "{\"uuid\":\"06a9a51d-fa05-46e5-a1ea-10b39db929ca\",\"android_id\":\"502f1a590635b94c\",\"oaid\":\"0c2f9397a1723cc5\",\"vaid\":\"390bd08651bb5ec9\",\"aaid\":\"9873f310-dce7-4b89-a465-ff872b11df1c\"}",
    "sentry-trace": "00000000000000000000000000000000-0000000000000000-0"
}

data = {
    "categoryId": "63c76a8924b1622727bd321c",
    "tab": "ALL",
    "omitSubscribed": False
}

response = requests.post(url, headers=headers, json=data)
print(response.status_code)
print(response.json())