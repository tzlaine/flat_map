#!/usr/bin/env python

import math

variant_names = ['boost_flat_map', 'std_map', 'vector', 'vector_custom_pair']
variant_colors = {'boost_flat_map': 'blue', 'std_map': 'red', 'vector': 'green', 'vector_custom_pair': 'black'}

pretty_variant_names = {'boost_flat_map': 'Boost.FlatMap', 'std_map': 'std::map', 'vector': 'std::vector', 'vector_custom_pair': 'std::vector (custom pair)'}

variant_data = {}
for v in variant_names:
    execfile('{}.py'.format(v))
    variant_data[v] = {'int': int_timings, 'struct': struct_timings}

element_type_marks = {'int': 'square', 'struct': 'triangle'}
operation_colors = {'insert': 'red', 'iterate': 'green', 'erase': 'blue'}
#operation_marks = {'insert': '+', 'iterate': 'o', 'lower bound': '|', 'erase': '-'}
operation_marks = {'insert': '|', 'iterate': '|', 'lower bound': '|', 'erase': '|'}

def operation_chart(operation, element_type):
    retval = ''

    plots = []
    legends = []
    xmax = 0
    ymax = 0

    operation_list = operation.split('/')

    for k,v in variant_data.items():
        points = ''
        for element in v[element_type]:
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
            'color': variant_colors[k],
            'mark': operation in operation_marks and operation_marks[operation] or '|',
            'points': points,
        })
        legends.append(pretty_variant_names[k])

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

    title = '{} Operations, {}-Valued Elements'.format(operation.title(), element_type)
    ylabel = 'Time [milliseconds]'
    if len(operation_list) == 2:
        title = '{} Cost Ratio, {}-Valued Elements'.format(operation.title(), element_type)
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
contents = contents.replace('%%% iterate, int %%%', operation_chart('iterate', 'int'))
contents = contents.replace('%%% lower bound, int %%%', operation_chart('lower bound', 'int'))
contents = contents.replace('%%% erase, int %%%', operation_chart('erase', 'int'))

contents = contents.replace('%%% insert/iterate, int %%%', operation_chart('insert/iterate', 'int'))
contents = contents.replace('%%% insert/lower bound, int %%%', operation_chart('insert/lower bound', 'int'))

#contents = contents.replace('%%% insert, struct %%%', operation_chart('insert', 'struct'))
#contents = contents.replace('%%% iterate, struct %%%', operation_chart('iterate', 'struct'))
#contents = contents.replace('%%% lower bound, struct %%%', operation_chart('lower bound', 'struct'))
#contents = contents.replace('%%% erase, struct %%%', operation_chart('erase', 'struct'))

open('../../paper/motivation_and_scope.tex', 'w').write(contents)
