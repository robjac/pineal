from imports import *
from pyo import *

class Analyzer(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)

		self.s = Server(
			audio="jack",
			jackname=TITLE,
			nchnls = 4
		).boot()

		self._stop = False

	def run(self):
		self.s.start()

		src = Input(chnl=AUDIO_PORTS)

		self.amp = Follower(src)
		self.bass = Follower(Biquad(src, 110, type=0))
		self.high = Follower(Biquad(src, 1000, type=1))

		self.pitch = Yin(src)
		self.note = 0

		self.band = list()
		self.norm_band = [0]*9
		c = 4186.01*2
		for i in xrange(9):
			lowpassed = Follower(Biquad(src, c, type=0))
			highpassed = Follower(Biquad(lowpassed, c/2, type=1))
			band = Follower(highpassed)/(2**(i-1))
			self.band.append(band)
			c /= 2
		self.band.reverse()

		#p = Pattern(self.update, 1.0/30)
		#p.play()

		while not self._stop:
			self.update()
			time.sleep(0.1)

	def stop(self):
		self._stop = True
		self.s.stop()

	def update(self):
		for v in visuals.get():
			if self.pitch.get()>1:
				self.note = math.log(self.pitch.get()/16.35,2)%1.0


		for i in xrange(9):
			self.norm_band[i] = self.band[i].get()

		m = max(self.norm_band)
		if m>0:
			for i in xrange(9):
				self.norm_band[i] = self.amp.get()*self.band[i].get()/m

def init():
	global analyzer
	analyzer = Analyzer()

def band(a, b=None):
	if b:
		return sum(analyzer.norm_band[a:b])
	else:
		return analyzer.norm_band[a]

def start():
	analyzer.start()

def stop():
	analyzer.stop()

def update():
	analyzer.update()

def amp():
	return analyzer.amp.get()

def note():
	return analyzer.note
