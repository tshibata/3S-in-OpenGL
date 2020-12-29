
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

File.open(ARGV[0], 'r') do |i|
	File.open(ARGV[1], 'w') do |o|
		while ! i.eof?
			line = i.readline
			if /^\W*$/ =~ line
				o.puts ""
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
				o.puts "u:#{p1.u} v:#{p1.v} x:#{p1.x} y:#{p1.y} z:#{p1.z} a:#{a} b:#{b} c:#{c}"
				o.puts "u:#{p2.u} v:#{p2.v} x:#{p2.x} y:#{p2.y} z:#{p2.z} a:#{a} b:#{b} c:#{c}"
				o.puts "u:#{p3.u} v:#{p3.v} x:#{p3.x} y:#{p3.y} z:#{p3.z} a:#{a} b:#{b} c:#{c}"
			end
		end
	end
end

