import youtube_dl
import sys

def download_video(url):	
	# youtube options
	ydl_opts = {
		'format': 'bestaudio/best',
		'outtmpl': '../audiofiles/%(title)s.%(ext)s', # <--- pay attention here
		'postprocessors': [{
		'key': 'FFmpegExtractAudio',
		'preferredcodec': 'mp3',
		'preferredquality': '192',
		}],
	}

	with youtube_dl.YoutubeDL(ydl_opts) as ydl:
		ydl.download([url])
		info_dict = ydl.extract_info(url, download=False)
		video_title = info_dict.get('title', None)
	

def read_from_file(filename: str):
	# read videos from text file
	f = open(filename, "r")
	for line in f:
		print(line)
		try:
			download_video(line)
		except Exception as e:
			print(e)

def main():
	if len(sys.argv) < 1:
		print("Missing command line arg: ")
	else:
		download_video(sys.argv[1])

	

if __name__ == "__main__":
	main()
		
