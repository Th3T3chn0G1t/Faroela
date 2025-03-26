# SPDX-License-Identifier: MIT

import argparse
import json

parser = argparse.ArgumentParser(
	prog='generate',
	description='Generate Faroela interfaces for supported languages')

parser.add_argument('-i', '--api-source', default='api.json', help='The JSON API description to generate from')
parser.add_argument('-o', '--output', help='The interface file to output to', required=True)
parser.add_argument('-t', '--language', default='cxx', help='The language to generate interfaces for')

parsed = parser.parse_args()

api_source = ''
with open(parsed.api_source) as f:
	for line in f:
		line = line.strip()
		if line[0:2] == '//':
			continue

		api_source += line

api = json.loads(api_source)

def snake_case(s):
	return s

def pascal_case(s):
	out = ''

	i = 0
	do_upper = True

	while i < len(s):
		c = s[i]

		if do_upper:
			c = c.upper()
			do_upper = False

		if c == '_':
			do_upper = True
		else:
			out += c

		i += 1

	return out

def generate_cxx(f, api):
	f.write('// SPDX-License-Identifier: MIT')
	f.write('\n')
	f.write('#pragma once\n')
	f.write('\n')
	f.write('extern "C" {\n')
	f.write('\tnamespace faroela::api {\n')

	for id, values in api['enumerations'].items():
		f.write(f'\t\tenum class {snake_case(id)} : int {{\n')

		for e, v in values.items():
			f.write(f'\t\t\t{snake_case(e)} = {snake_case(str(v))},\n')

		f.write('\t\t};\n')
		f.write('\n')

	for id, values in api['functions'].items():
		f.write(f'\t\tFAROELA_COMMON_EXPORT link_bool faroela_{snake_case(id)}(')

		for i in range(len(values)):
			parameter = values[i]

			split = parameter.split('*', 1)
			parameter = split[0]
			if len(split) > 1:
				indirection = split[1] + '*'
			else:
				indirection = ''

			if parameter == 'pointer':
				parameter = 'void*'
			elif parameter == 'const_pointer':
				parameter = 'void*'
			elif parameter == 'context':
				parameter = 'context*'
			elif parameter == 'bool':
				parameter = 'link_bool'
			elif parameter == 'string':
				parameter = 'char*'
			elif parameter == 'const_string':
				parameter = 'const char*'

			f.write(f'{parameter}{indirection}')
			if i < len(values) - 1:
				f.write(', ')

		f.write(');\n')

	f.write('\t}\n')
	f.write('}\n')

def generate_cs(f, api):
	f.write('// SPDX-License-Identifier: MIT')
	f.write('\n')
	f.write('using System;\n')
	f.write('using System.Runtime.InteropServices;\n')
	f.write('\n')
	f.write('namespace Pharaoh;\n')
	f.write('\n')
	f.write('public unsafe partial class Faroela\n')
	f.write('{\n')

	for id, values in api['enumerations'].items():
		f.write(f'\tpublic enum {pascal_case(id)} {{\n')

		for e, v in values.items():
			f.write(f'\t\t{pascal_case(e)} = {pascal_case(str(v))},\n')

		f.write('\t}\n')
		f.write('\n')

	for id, values in api['functions'].items():
		f.write(f'\t[LibraryImport("faroela", EntryPoint = "faroela_{id}", StringMarshalling = StringMarshalling.Utf8)]\n')
		f.write('\t[return: MarshalAs(UnmanagedType.U4)]\n')
		f.write(f'\tpublic static partial bool {pascal_case(id)}(')

		for i in range(len(values)):
			parameter = values[i]

			split = parameter.split('*', 1)
			parameter = split[0]
			if len(split) > 1:
				indirection = split[1] + '*'
			else:
				indirection = ''

			if parameter == 'pointer':
				parameter = 'IntPtr'
			elif parameter == 'const_pointer':
				parameter = 'IntPtr'
			elif parameter == 'context':
				parameter = 'IntPtr'
			elif parameter == 'bool':
				parameter = '[MarshalAs(UnmanagedType.U4)] bool'
			elif parameter == 'const_string':
				parameter = 'string'

			intrins = [
				'bool', 'char', 'float', 'double',
				'sbyte', 'byte', 'short', 'ushort', 'int', 'uint', 'long', 'ulong'
			]

			if parameter not in intrins:
				parameter = pascal_case(parameter)

			f.write(f'{parameter}{indirection} p{i}')
			if i < len(values) - 1:
				f.write(', ')

		f.write(');\n\n')

	f.write('}\n')

generators = {
	'cxx': generate_cxx,
	'cs': generate_cs
}

def splice(f, api, position, language):
	try:
		with open(f'extra/{language}/{position}') as h:
			f.write(h.read())
	except:
		pass

with open(parsed.output, 'w') as f:
	splice(f, api, 'head', parsed.language)

	generators[parsed.language](f, api)

	splice(f, api, 'tail', parsed.language)
