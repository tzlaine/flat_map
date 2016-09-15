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

x_is_power_of_2 = False
power_of_2_initial = 8
non_power_of_2_initial = 10

def operation_chart(operation, element_type, show_legend = False):
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
            'dashed': False # TODO: Perhaps use this to show multiple platforms on one chart.
        })
        legends.append(pretty_variant_names[variant_name])

    legends = ','.join(legends)

    y_step = math.pow(10, math.trunc(math.log10(ymax)))
    ymax = (math.trunc(ymax / y_step) + 1) * y_step

    if x_is_power_of_2:
        xtick_int = power_of_2_initial
    else:
        xtick_int = non_power_of_2_initial
    xtick = str(xtick_int)
    xtick_labels = xtick
    steps = 2
    while xtick_int <= xmax:
        xtick += ','
        xtick_labels += ','

        if x_is_power_of_2:
            xtick_int <<= 1
        else:
            xtick_int *= non_power_of_2_initial

        xtick += str(xtick_int)
        if x_is_power_of_2:
            if 64 * 1024 < xtick_int:
                if 1024 * 1024 <= xtick_int:
                    xtick_labels += '{}M'.format(xtick_int >> 20)
                elif 1024 <= xtick_int:
                    xtick_labels += '{}k'.format(xtick_int >> 10)
        else:
            if non_power_of_2_initial ** 2 < xtick_int:
                xtick_labels += '${}^{{{}}}$'.format(non_power_of_2_initial, steps)

        steps += 1

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

    legend_pos = ''
    legend_entries = ''
    if show_legend:
        legend_pos = 'legend pos=outer north east,'
        legend_entries = 'legend entries={{{legends}}},'.format(**locals())

    #TODO: Use \groupplot ?

    retval = '''\\begin{{tikzpicture}}[baseline]
    \\begin{{axis}}[
        small,{legend_pos}{legend_entries}
        %width=3.75in,
        title={{{title}}},
        xlabel={{N}},
        ylabel={{{ylabel}}},%\empty,
        xmin=0, xmax={xmax},
        ymin=0, ymax={ymax},
        xtick={{{xtick}}},
        xticklabels={{{xtick_labels}}},
        ytick={{{ytick}}},
        ymajorgrids=true,
        grid style=dashed,
        scaled x ticks=false,
        scaled y ticks=true,
        ]

'''.format(**locals())

    for plot in plots:
        color = plot['color']
        mark = plot['mark']
        points = plot['points']
        dashed = plot['dashed'] and 'dashed' or ''
        retval += '''    \\addplot[color={color},mark={mark},no markers,{dashed}]
        coordinates {{{points}}};

'''.format(**locals())

    retval += '''    \\end{{axis}}%
\\end{{tikzpicture}}%
~%
%'''.format()

    return retval

contents = open('../../paper/motivation_and_scope.in.tex', 'r').read()

contents = contents.replace('%%% insert, int %%%', operation_chart('insert', 'int', True))
contents = contents.replace('%%% insert, string %%%', operation_chart('insert', 'string'))
contents = contents.replace('%%% iterate, int %%%', operation_chart('iterate', 'int', True))
contents = contents.replace('%%% iterate, string %%%', operation_chart('iterate', 'string'))
contents = contents.replace('%%% find, int %%%', operation_chart('find', 'int', True))
contents = contents.replace('%%% find, string %%%', operation_chart('find', 'string'))
contents = contents.replace('%%% erase, int %%%', operation_chart('erase', 'int', True))
contents = contents.replace('%%% erase, string %%%', operation_chart('erase', 'string'))

contents = contents.replace('%%% insert/iterate, int %%%', operation_chart('insert/iterate', 'int', True))
contents = contents.replace('%%% insert/iterate, string %%%', operation_chart('insert/iterate', 'string'))
contents = contents.replace('%%% insert/find, int %%%', operation_chart('insert/find', 'int', True))
contents = contents.replace('%%% insert/find, string %%%', operation_chart('insert/find', 'string'))

open('../../paper/motivation_and_scope.tex', 'w').write(contents)
