require 'rexml/document'

$sourceMap = {}
$verticesMap = {}

class Source
	attr_accessor :array
		def initialize(element)
		element.elements.each do |child|
			if child.name == 'float_array'
				@array = child.text.split(' ').map{ |s| s.to_f }
			end
		end
	end
end

class Input
	attr_accessor :source, :offset
	def initialize(element)
		@semantic = element.attributes['semantic']
		@source = element.attributes['source']
		@offset = element.attributes['offset'].to_i if element.attributes.has_key?('offset')
	end
end

class Vertices
	attr_accessor :inputs
	def initialize(element)
		@inputs = {}
		element.elements.each do |child|
			if child.name == 'input'
				@inputs[child.attributes['semantic']] = Input.new(child)
			end
		end
	end
end

class Triangles
	attr_accessor :inputs, :count, :p
	def initialize(element)
		@inputs = {}
		@count = element.attributes['count'].to_i
		element.elements.each do |child|
			if child.name == 'input'
				@inputs[child.attributes['semantic']] = Input.new(child)
			elsif child.name == 'p'
				@p = child.text.split(' ').map{ |s| s.to_i }
			end
		end
	end
end

class Geometry
	attr_accessor :name, :triangles
	def initialize(element)
		@name = element.attributes['name']
		element.elements.each do |child|
			if child.name == 'mesh'
				child.elements.each do |child|
					if child.name == 'source'
						$sourceMap['#' + child.attributes['id']] = Source.new(child)
					elsif child.name == 'vertices'
						$verticesMap['#' + child.attributes['id']] = Vertices.new(child)
					elsif child.name == 'triangles'
						@triangles = Triangles.new(child)
					end
				end
			end
		end
	end
end

class Collada
	attr_accessor :geometries
	def initialize(element)
		@geometries = []
		element.elements.each do |child|
			if child.name == 'library_geometries'
				child.elements.each do |child|
					if child.name == 'geometry'
						@geometries.push(Geometry.new(child))
					end
				end
			end
		end
	end
end

ARGV.each do |src|
	doc = REXML::Document.new(File.new(src))
	collada = Collada.new(doc.root)
	collada.geometries.each do |geometry|
		name = File.join(File.dirname(src), geometry.name + '.u-c.txt')
		File.open(name, 'w') do |o|
			textureInput = geometry.triangles.inputs['TEXCOORD']
			normalInput = geometry.triangles.inputs['NORMAL']
			vertexInput = geometry.triangles.inputs['VERTEX']
			textureSource = $sourceMap[textureInput.source]
			normalSource = $sourceMap[normalInput.source]
			vertexSource = $sourceMap[$verticesMap[vertexInput.source].inputs['POSITION'].source]
			stride = geometry.triangles.p.length / (3 * geometry.triangles.count)
			(0 ... geometry.triangles.count).each do |i|
				(0 ... 3).reverse_each do |j|
					k = geometry.triangles.p[stride * (i * 3 + j) + textureInput.offset]
					o.write("u:#{textureSource.array[k * 2 + 0]}")
					o.write(" v:#{textureSource.array[k * 2 + 1]}")
					k = geometry.triangles.p[stride * (i * 3 + j) + vertexInput.offset]
					o.write(" x:#{vertexSource.array[k * 3 + 0]}")
					o.write(" y:#{vertexSource.array[k * 3 + 1]}")
					o.write(" z:#{-vertexSource.array[k * 3 + 2]}")
					k = geometry.triangles.p[stride * (i * 3 + j) + normalInput.offset]
					o.write(" a:#{normalSource.array[k * 3 + 0]}")
					o.write(" b:#{normalSource.array[k * 3 + 1]}")
					o.write(" c:#{-normalSource.array[k * 3 + 2]}\n")
				end
				o.write("\n")
			end
		end
	end
end
