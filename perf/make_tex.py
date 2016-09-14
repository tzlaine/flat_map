#!/usr/bin/env python

import math

variant_names = ['boost_flat_map', 'std_map', 'unordered_map', 'vector', 'vector_custom_pair']
variant_colors = {'boost_flat_map': 'blue', 'std_map': 'red', 'unordered_map': 'brown', 'vector': 'green', 'vector_custom_pair': 'black'}

pretty_variant_names = {'boost_flat_map': 'Boost.FlatMap', 'std_map': 'std::map', 'unordered_map': 'std::unordered\\_map', 'vector': 'std::vector', 'vector_custom_pair': 'std::vector (custom pair)'}

variant_data = {}
for v in variant_names:
    execfile('{}.py'.format(v))
    variant_data[v] = {'int': int_timings, 'string': string_timings}

element_type_marks = {'int': 'square', 'string': 'triangle'}
operation_colors = {'insert': 'red', 'iterate': 'green', 'erase': 'blue'}
#operation_marks = {'insert': '+', 'iterate': 'o', 'find': '|', 'erase': '-'}
operation_marks = {'insert': '|', 'iterate': '|', 'find': '|', 'erase': '|'}

def operation_chart(operation, element_type):
    retval = ''

    plots = []
    legends = []
    xmax = 0
    ymax = 0

    operation_list = operation.split('/')

    for variant_name in variant_names:
        data = variant_data[variant_name]
        points = ''
        for element in data[element_type]:
            x = element['size']
            if len(operation_list) == 2:
                y = element[operation_list[0]]
                divisor = element[operation_list[1]]
                if divisor == 0:
                    divisor = 0.00001
                y /= divisor
            else:
                y = element[operation]
            xmax = max(xmax, x)
            ymax = max(ymax, y)
            points += '({x},{y})'.format(**locals())
        plots.append({
            'color': variant_colors[variant_name],
            'mark': operation in operation_marks and operation_marks[operation] or '|',
            'points': points,
        })
        legends.append(pretty_variant_names[variant_name])

    legends = ','.join(legends)

    y_step = math.pow(10, math.trunc(math.log10(ymax)))
    ymax = (math.trunc(ymax / y_step) + 1) * y_step

    xtick_int = 8
    xtick = str(xtick_int)
    xtick_labels = xtick
    while xtick_int <= xmax:
        xtick += ','
        xtick_labels += ','

        xtick_int <<= 1

        xtick += str(xtick_int)
        if 64 * 1024 < xtick_int:
            if 1024 * 1024 <= xtick_int:
                xtick_labels += '{}M'.format(xtick_int >> 20)
            elif 1024 <= xtick_int:
                xtick_labels += '{}k'.format(xtick_int >> 10)

    ytick_float = 0.0
    ytick = str(ytick_float)
    while ytick_float <= ymax:
        ytick += ','
        ytick_float += y_step
        ytick += str(ytick_float)

    title = '{0} Operations, <{1}, {1}> Elements'.format(operation.title(), element_type)
    ylabel = 'Time [milliseconds]'
    if len(operation_list) == 2:
        title = '{0} Cost Ratio, <{1}, {1}> Elements'.format(operation.title(), element_type)
        ylabel = 'Ratio'

    retval = '''\\begin{{center}}
    \\begin{{tikzpicture}}
    \\begin{{axis}}[
        width=3.5in,
        title={{{title}}},
        xlabel={{N}},
        ylabel={{{ylabel}}},
        xmin=0, xmax={xmax},
        ymin=0, ymax={ymax},
        xtick={{{xtick}}},
        xticklabels={{{xtick_labels}}},
        ytick={{{ytick}}},
        legend pos=north west,
        ymajorgrids=true,
        grid style=dashed,
        scaled x ticks=false,
        scaled y ticks=true,
        legend entries={{{legends}}},
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
\\end{{center}}
'''.format()

    return retval

contents = open('../../paper/motivation_and_scope.in.tex', 'r').read()

contents = contents.replace('%%% insert, int %%%', operation_chart('insert', 'int'))
contents = contents.replace('%%% insert, string %%%', operation_chart('insert', 'string'))
contents = contents.replace('%%% iterate, int %%%', operation_chart('iterate', 'int'))
contents = contents.replace('%%% iterate, string %%%', operation_chart('iterate', 'string'))
contents = contents.replace('%%% find, int %%%', operation_chart('find', 'int'))
contents = contents.replace('%%% find, string %%%', operation_chart('find', 'string'))
contents = contents.replace('%%% erase, int %%%', operation_chart('erase', 'int'))
contents = contents.replace('%%% erase, string %%%', operation_chart('erase', 'string'))

contents = contents.replace('%%% insert/iterate, int %%%', operation_chart('insert/iterate', 'int'))
contents = contents.replace('%%% insert/iterate, string %%%', operation_chart('insert/iterate', 'string'))
contents = contents.replace('%%% insert/find, int %%%', operation_chart('insert/find', 'int'))
contents = contents.replace('%%% insert/find, string %%%', operation_chart('insert/find', 'string'))

open('../../paper/motivation_and_scope.tex', 'w').write(contents)
