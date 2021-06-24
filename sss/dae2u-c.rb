require 'rexml/document'

$sourceMap = {}
$verticesMap = {}
$geometriesMap = {}

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

class Node
	attr_accessor :name, :transform, :geometry
	def initialize(element)
		@name = element.attributes['name']
		element.elements.each do |child|
			if child.name == 'matrix'
				if child.attributes['sid'] == 'transform'
					@transform = child.text.to_s.split.map{ |s| s.to_f }
				end
			elsif child.name == 'instance_geometry'
				@geometry = child.attributes['url']
			end
		end
	end
	def transform(v)
		w = v[0] * @transform[12] +v[1] * @transform[13] + v[2] * @transform[14] + @transform[15];
		v[0] = (v[0] * @transform[0] +v[1] * @transform[1] + v[2] * @transform[2] + @transform[3]) / w;
		v[1] = (v[0] * @transform[4] +v[1] * @transform[5] + v[2] * @transform[6] + @transform[7]) / w;
		v[2] = (v[0] * @transform[8] +v[1] * @transform[9] + v[2] * @transform[10] + @transform[11]) / w;
	end
end

class Collada
	attr_accessor :nodesMap
	def initialize(element)
		@nodesMap = {}
		element.elements.each do |child|
			if child.name == 'library_geometries'
				child.elements.each do |child|
					if child.name == 'geometry'
						$geometriesMap['#' + child.attributes['id']] = Geometry.new(child)
					end
				end
			elsif child.name == 'library_visual_scenes'
				child.elements.each do |child|
					if child.name == 'visual_scene'
						child.elements.each do |child|
							if child.name == 'node'
								@nodesMap[child.attributes['name']] = Node.new(child)
							end
						end
					end
				end
			end
		end
	end
end

src = ARGV.shift
doc = REXML::Document.new(File.new(src))
collada = Collada.new(doc.root)
ARGV.each do |name|
	node = collada.nodesMap[name]
	geometry = $geometriesMap[node.geometry]
	File.open(name + '.u-c.txt', 'w') do |w|
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
				w.write("u:#{textureSource.array[k * 2 + 0]}")
				w.write(" v:#{-textureSource.array[k * 2 + 1]}")
				k = geometry.triangles.p[stride * (i * 3 + j) + vertexInput.offset]
				v = vertexSource.array[k * 3, 3]
				node.transform(v)
				w.write(" x:#{v[0]}")
				w.write(" y:#{v[1]}")
				w.write(" z:#{-v[2]}")
				k = geometry.triangles.p[stride * (i * 3 + j) + normalInput.offset]
				w.write(" a:#{normalSource.array[k * 3 + 0]}")
				w.write(" b:#{normalSource.array[k * 3 + 1]}")
				w.write(" c:#{-normalSource.array[k * 3 + 2]}\n")
			end
			w.write("\n")
		end
	end
end
