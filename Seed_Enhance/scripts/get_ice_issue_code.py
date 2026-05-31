import os
import requests
import json
from typing import TypedDict
from tqdm import tqdm


class IssueWithCode(TypedDict):
    id: int
    t: str
    code: str


def get_rust_issue_list(page=1, link=None, token=None):
    if link is None:
        url = f"https://api.github.com/repositories/724712/issues?state=open&labels=I-ICE&per_page=100&page={page}"
        query = None
    else:
        url = link
        query = None

    headers = {
        "Accept": "application/vnd.github.v3+json",
        "X-GitHub-Api-Version": "2022-11-28"
    }
    if token:
        headers["Authorization"] = f"Bearer {token}"

    res = requests.get(url, headers=headers, params=query, timeout=10)
    res.raise_for_status()
    data = res.json()
    h = res.headers
    return data, h


def map_issue_to_body(issue):
    return issue["body"]


def map_issue_to_id(issue):
    if "number" in issue:
        return issue["number"]
    return issue["id"]


def map_issue_body_to_code(issue_body):
    if not issue_body:
        return ""
    CODE_BEGINS = [
        "```Rust",
        "```rust",
        "```rs",
    ]
    CODE_END = "```"
    for CODE_BEGIN in CODE_BEGINS:
        s1 = issue_body.split(CODE_BEGIN)
        if len(s1) < 2:
            continue
        s2 = s1[1].split(CODE_END)
        if len(s2) < 1:
            continue
        code = s2[0]
        return code
    return ""


def map_issue_to_store(issue):
    code = map_issue_body_to_code(map_issue_to_body(issue))
    if not code:
        return None
    return IssueWithCode(
        id=map_issue_to_id(issue),
        code=code,
        t=''
    )


def get_rust_issue_timeline(issue_id, token=None):
    url = f"https://api.github.com/repositories/724712/issues/{issue_id}/timeline"
    headers = {
        "Accept": "application/vnd.github.v3+json",
        "X-GitHub-Api-Version": "2022-11-28"
    }
    if token:
        headers["Authorization"] = f"Bearer {token}"

    res = requests.get(url, headers=headers, timeout=10)
    res.raise_for_status()
    data = res.json()
    return data


def timeline_to_code(timeline, issue_id):
    res = []
    for i in timeline:
        if i.get("body") is None:
            continue
        code = map_issue_body_to_code(i["body"])
        if code:
            res.append(IssueWithCode(
                id=i["id"],
                code=code,
                t=f"{issue_id}_ti"
            ))
    return res

def main(args):
    issues_with_code = set()
    issue_no_code = set()
    page = args.page
    link = None
    t = tqdm(total=args.count)
    if args.dir:
        os.makedirs(args.dir, exist_ok=True)

    while len(issues_with_code) < args.count:
        issues, headers = get_rust_issue_list(page, link, args.token)
        if not issues:
            break

        for i in issues:
            if map_issue_to_id(i) in issues_with_code:
                continue
            if map_issue_to_id(i) in issue_no_code:
                continue
            
            timeline = get_rust_issue_timeline(
                map_issue_to_id(i), args.token)
            t_code = timeline_to_code(timeline, map_issue_to_id(i))
            t.update(len(t_code))
            if args.dir:
                for issue in t_code:
                    with open(f"{args.dir}/issue_{issue["t"]}{issue["id"]}.rs", "w", encoding="utf-8") as f:
                        f.write(issue["code"])

            m = map_issue_to_store(i)
            if m is None:
                issue_no_code.add(map_issue_to_id(i))
                continue
            issues_with_code.add(m["id"])
            t.update(1)

            if args.dir:
                with open(f"{args.dir}/issue_{m["t"]}{m["id"]}.rs", "w", encoding="utf-8") as f:
                    f.write(m["code"])
                    # f.write(f"\n\n//[Issue: {issue["t"]}{issue['id']}]\n\n")

        page += 1

    print("Total issues with code:", len(issues_with_code))
    print("To page:", page)

    if args.dir:
        with open(f"{args.dir}/issue_no_code.json", "w", encoding="utf-8") as f:
            json.dump(list(issue_no_code), f)
    if args.dir:
        with open(f"{args.dir}/issue_with_code.json", "w", encoding="utf-8") as f:
            json.dump(list(issues_with_code), f)


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("-t", "--token", type=str,
                        help="Github token", default=None)
    parser.add_argument("-o", "--output", type=str, help="Output file")
    parser.add_argument("-p", "--page", type=int,
                        help="From page", default=1, )
    parser.add_argument("-c", "--count", type=int,
                        help="Count of issues", default=60)
    parser.add_argument("-d", "--dir", type=str,
                        help="Output rust source code directory")
    args = parser.parse_args()
    main(args)
