#!/usr/bin/python3

import os, requests, tarfile

repos = [
	"",
	"_image",
]

def download():
	for repo in repos:
		print(f"Listing SDL2{repo} releases ...")
		responce = requests.get(f"https://api.github.com/repos/libsdl-org/SDL{repo}/releases")
		releases = responce.json()

		flag = False

		for release_index, release in enumerate(releases):
			version = releases[release_index]["name"]
			
			if version.startswith('2'):
				print(f"Latest SDL2{repo} version: {version}")
				
				for asset_index, asset in enumerate(releases[release_index]["assets"]):
					filename = releases[release_index]["assets"][asset_index]["name"]
					
					if filename == f"SDL2{repo}-devel-{version}-mingw.tar.gz":
						url = releases[release_index]["assets"][asset_index]["browser_download_url"]
						print(f"URL: {url}")
						flag = True
						break
			
			if flag:
				break
		
		print(f"Downloading {filename} ...")
		file = requests.get(url)

		with open(f"./lib/{filename}", "wb") as f:
			f.write(file.content)
		
		print("Done")

def extract():
	for file in os.listdir("./lib"):
		print(f"Extracting {file} ...")
		
		with tarfile.open(f"./lib/{file}") as tar:
			filename = file.removesuffix(".tar.gz")
			tar.extractall(f"./lib/{filename}")
			
		print("Done")

def package():
	print("Packaging ...")
	dirs = [dir for dir in os.listdir("./lib") if not dir.endswith(".tar.gz")]
	os.system("mkdir -p ./lib/bin ./lib/include/SDL2 ./lib/lib")
	
	for dir in dirs:
		if dir.startswith("SDL2-devel"):
			version = dir.removeprefix("SDL2-devel-").removesuffix("-mingw")
			os.system(f"cp -rT ./lib/SDL2-devel-{version}-mingw/SDL2-{version}/x86_64-w64-mingw32/bin/ ./lib/bin/")
			os.system(f"cp -rT ./lib/SDL2-devel-{version}-mingw/SDL2-{version}/x86_64-w64-mingw32/include/SDL2/ ./lib/include/SDL2/")
			os.system(f"cp -rT ./lib/SDL2-devel-{version}-mingw/SDL2-{version}/x86_64-w64-mingw32/lib/ ./lib/lib/")
		
		if dir.startswith("SDL2_image-devel"):
			version = dir.removeprefix("SDL2_image-devel-").removesuffix("-mingw")
			os.system(f"cp -rT ./lib/SDL2_image-devel-{version}-mingw/SDL2_image-{version}/x86_64-w64-mingw32/bin/ ./lib/bin/")
			os.system(f"cp -rT ./lib/SDL2_image-devel-{version}-mingw/SDL2_image-{version}/x86_64-w64-mingw32/include/SDL2/ ./lib/include/SDL2/")
			os.system(f"cp -rT ./lib/SDL2_image-devel-{version}-mingw/SDL2_image-{version}/x86_64-w64-mingw32/lib/ ./lib/lib/")
	
	with open("./lib/bin/sdl2-config", "r+") as config:
		content = config.read()
		config.seek(0)
		config.write(content.replace("/include/SDL2", "/include"))
		config.truncate()
	
	print("Done")

if __name__ == "__main__":
	if not "lib" in os.listdir('.'):
		os.mkdir("./lib", 0o755)
		download()
		extract()
		package()
