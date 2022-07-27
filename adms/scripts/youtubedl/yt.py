import youtube_dl
import json
import sys

def download_video(url, path):	
	
	print(f"PATH: {path}")
	# youtube options
	ydl_opts = {
		'format': 'bestaudio/best',
		'outtmpl': f'{path}/%(title)s.%(ext)s', # <--- pay attention here
		'postprocessors': [{
		'key': 'FFmpegExtractAudio',
		'preferredcodec': 'wav',
		'preferredquality': '192',
		}],
	}

	with youtube_dl.YoutubeDL(ydl_opts) as ydl:
		ydl.download([url])
		info_dict = ydl.extract_info(url, download=False)
		video_title = info_dict.get('title', None)
	

def read_from_file(filename: str):
	# read videos from text file
	with open(filename) as json_file:
		data = json.load(json_file)
		get_collection = data["collections"]["collection_testing"]
		for video in get_collection["videos"]:
			download_video(video, get_collection["path"])

def main():

	read_from_file("data.json")

	

if __name__ == "__main__":
	main()
		
