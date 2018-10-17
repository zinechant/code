import re

# read the string filename
filename = input()

ans = set()
with open(filename, "r") as fi:
    for line in fi:
        matches = re.match("\S+\s+-\s+-\s+\[.*\]\s+\"GET\s+(\S+\.gif)\s+\S+\"\s+200\s+\d+", line)
        if matches:
            gif = matches.groups()[0]
            gif = gif.split("/")[-1]
            ans.add(gif)

with open("gifs_%s" % filename, "w") as fo:
    for gif in sorted(ans):
        fo.write("%s\n" % gif)