#!/usr/bin/env python

import math

variant_names = ['boost_flat_map', 'std_map', 'vector', 'vector_custom_pair']
variant_colors = {'boost_flat_map': 'blue', 'std_map': 'red', 'vector': 'green', 'vector_custom_pair': 'black'}

pretty_variant_names = {'boost_flat_map': 'Boost.FlatMap', 'std_map': 'std::map', 'vector': 'std::vector', 'vector_custom_pair': 'std::vector (custom pair)'}

variant_data = {}
for v in variant_names:
    execfile('{}.py'.format(v))
    variant_data[v] = {'int': int_timings, 'struct': struct_timings}

#print variant_data
#exit(0)

element_type_marks = {'int': 'square', 'struct': 'triangle'}
operation_colors = {'insert': 'red', 'iterate': 'green', 'erase': 'blue'}
operation_marks = {'insert': '+', 'iterate': 'o', 'erase': '-'}


def operation_chart(operation, element_type):
    retval = ''

    plots = []
    legends = []
    xmax = 0
    ymax = 0

    for k,v in variant_data.items():
        points = ''
        for element in v[element_type]:
            x = element['size']
            y = element[operation]
            xmax = max(xmax, x)
            ymax = max(ymax, y)
            points += '({x},{y})'.format(**locals())
        plots.append({
            'color': variant_colors[k],
            'mark': operation_marks[operation],
            'points': points,
        })
        legends.append(pretty_variant_names[k])

    legends = ','.join(legends)

    xmax = math.pow(2, math.floor(math.log(xmax, 2) - 0.5) + 1)
    ymax = math.pow(10, math.floor(math.log10(ymax) - 0.5) + 1)

    xtick = ''
    num_x_ticks = 10
    for i in range(0, num_x_ticks):
        if i != 0:
            xtick += ','
        xtick += str(xmax / num_x_ticks * i)

    ytick = ''
    num_y_ticks = 10
    for i in range(0, num_y_ticks):
        if i != 0:
            ytick += ','
        ytick += str(ymax / num_y_ticks * i)

    title = '{} Operations on {} Elements'.format(operation.title(), element_type)

    retval = '''\\begin{{tikzpicture}}
    \\begin{{axis}}[
        title={{{title}}},
        xlabel={{N}},
        ylabel={{Time [milliseconds]}},
        xmin=0, xmax={xmax},
        ymin=0, ymax={ymax},
        xtick={{{xtick}}},
        ytick={{{ytick}}},
        legend pos=north west,
        ymajorgrids=true,
        grid style=dashed,
        legend entries={{{legends}}}
        ]

'''.format(**locals())

    for plot in plots:
        color = plot['color']
        mark = plot['mark']
        points = plot['points']
        retval += '''    \\addplot[color={color},mark={mark},]
        coordinates {{{points}}};

'''.format(**locals())

    retval += '''    \\end{{axis}}
\\end{{tikzpicture}}
'''.format()

    return retval

contents = open('../../paper/paper.in.tex', 'r').read()

contents = contents.replace('%%% insert / int %%%', operation_chart('insert', 'int'))
contents = contents.replace('%%% iterate / int %%%', operation_chart('iterate', 'int'))
contents = contents.replace('%%% erase / int %%%', operation_chart('erase', 'int'))

contents = contents.replace('%%% insert / struct %%%', operation_chart('insert', 'struct'))
contents = contents.replace('%%% iterate / struct %%%', operation_chart('iterate', 'struct'))
contents = contents.replace('%%% erase / struct %%%', operation_chart('erase', 'struct'))

open('../../paper/paper.tex', 'w').write(contents)
