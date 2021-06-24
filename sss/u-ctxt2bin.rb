
MAGIC = 299792
VERSION = 1.0

class Point
	attr_accessor :u
	attr_accessor :v
	attr_accessor :x
	attr_accessor :y
	attr_accessor :z
	def initialize(line)
		if /^\W*u:(.+)\W+v:(.+)\W+x:(.+)\W+y:(.+)\W+z:(.+)\W*$/ =~ line
			@u = $1.to_f
			@v = $2.to_f
			@x = $3.to_f
			@y = $4.to_f
			@z = $5.to_f
		end
	end
end

ARGV.each do |src|
	puts src
	dst = src.sub(/(.txt)?$/, '.bin')
	puts dst
	File.open(src, 'r') do |i|
		File.open(dst, 'w') do |o|
			o.write([MAGIC + VERSION].pack('f'));
			while ! i.eof?
				line = i.readline
				if /^\W*$/ =~ line
					#o.puts ""
				else
					p1 = Point.new(line)
					p2 = Point.new(i.readline)
					p3 = Point.new(i.readline)
					dx1 = p2.x - p1.x
					dx2 = p3.x - p1.x
					dy1 = p2.y - p1.y
					dy2 = p3.y - p1.y
					dz1 = p2.z - p1.z
					dz2 = p3.z - p1.z
					a = -(dy1 * dz2 - dz1 * dy2)
					b = -(dz1 * dx2 - dx1 * dz2)
					c = -(dx1 * dy2 - dy1 * dx2)

					o.write([p1.u.to_f].pack('f'));
					o.write([p1.v.to_f].pack('f'));
					o.write([p1.x.to_f].pack('f'));
					o.write([p1.y.to_f].pack('f'));
					o.write([p1.z.to_f].pack('f'));
					o.write([a.to_f].pack('f'));
					o.write([b.to_f].pack('f'));
					o.write([c.to_f].pack('f'));

					o.write([p2.u.to_f].pack('f'));
					o.write([p2.v.to_f].pack('f'));
					o.write([p2.x.to_f].pack('f'));
					o.write([p2.y.to_f].pack('f'));
					o.write([p2.z.to_f].pack('f'));
					o.write([a.to_f].pack('f'));
					o.write([b.to_f].pack('f'));
					o.write([c.to_f].pack('f'));

					o.write([p3.u.to_f].pack('f'));
					o.write([p3.v.to_f].pack('f'));
					o.write([p3.x.to_f].pack('f'));
					o.write([p3.y.to_f].pack('f'));
					o.write([p3.z.to_f].pack('f'));
					o.write([a.to_f].pack('f'));
					o.write([b.to_f].pack('f'));
					o.write([c.to_f].pack('f'));
				end
			end
		end
	end
end


