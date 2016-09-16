#!/usr/bin/env python

import math
import os

compiler_names = ['msvc', 'clang', 'gcc']
compiler_data_paths = {
    'msvc': os.path.join('..', 'windows_msvc_data'),
    'clang': os.path.join('..', 'linux_clang_data'),
    'gcc': os.path.join('..', 'linux_gcc_data')
}
pretty_compiler_names = {'msvc': 'Windows/MSVC 2015', 'clang': 'Linux/Clang 3.8', 'gcc': 'Linux/GCC 4.8.4'}

variant_names = ['boost_flat_map', 'std_map', 'unordered_map', 'vector', 'vector_custom_pair']
variant_colors = {'boost_flat_map': 'blue', 'std_map': 'red', 'unordered_map': 'brown', 'vector': 'green', 'vector_custom_pair': 'black'}

pretty_variant_names = {'boost_flat_map': 'Boost.FlatMap', 'std_map': 'std::map', 'unordered_map': 'std::unordered\\_map', 'vector': 'std::vector', 'vector_custom_pair': 'std::vector (custom pair)'}

compiler_data = {}
for c in compiler_names:
    compiler_data[c] = {}
    variant_data = compiler_data[c]
    for v in variant_names:
        execfile(os.path.join(compiler_data_paths[c], '{}.py'.format(v)))
        variant_data[v] = {'int': int_timings, 'string': string_timings}

element_type_marks = {'int': 'square', 'string': 'triangle'}
operation_colors = {'insert': 'red', 'iterate': 'green', 'erase': 'blue'}
#operation_marks = {'insert': '+', 'iterate': 'o', 'find': '|', 'erase': '-'}
operation_marks = {'insert': '|', 'iterate': '|', 'find': '|', 'erase': '|'}

x_is_power_of_2 = False
power_of_2_initial = 8
non_power_of_2_initial = 10

class plot_t:
    def __init__(
        self,
        color,
        mark,
        points,
        dashed
    ):
        self.color = color
        self.mark = mark
        self.points = points
        self.dashed = dashed

class graph_t:
    def __init__(
        self,
        title,
        plots,
        xmax,
        ymax,
        xtick,
        xtick_labels,
        ytick
    ):
        self.title = title
        self.plots = plots
        self.xmax = xmax
        self.ymax = ymax
        self.xtick = xtick
        self.xtick_labels = xtick_labels
        self.ytick = ytick

def operation_graphs(operation, *element_types):
    retval = ''

    graphs = []

    ylabel = 'Time [milliseconds]'
    operation_list = operation.split('/')
    if len(operation_list) == 2:
        ylabel = 'Ratio'

    for compiler_name in compiler_names:
        for element_type in element_types:
            plots = []
            xmax = 0
            ymax = 0

            for variant_name in variant_names:
                data = compiler_data[compiler_name][variant_name]
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
                plots.append(
                    plot_t(
                        variant_colors[variant_name],
                        operation in operation_marks and operation_marks[operation] or '|',
                        points,
                        False # TODO: Perhaps use this to show multiple platforms on one chart.
                    )
                )

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

            title = '<{0}, {0}> Elements'.format(element_type)
            if len(operation_list) == 2:
                title = '<{0}, {0}> Elements'.format(element_type)

            graphs.append(
                graph_t(
                    title,
                    plots,
                    xmax,
                    ymax,
                    xtick,
                    xtick_labels,
                    ytick
                )
            )

    num_element_types = len(element_types)
    num_compilers = len(compiler_names)
    retval = '''\\begin{{tikzpicture}}
    \\begin{{groupplot}}[group style={{group size={} by {}, group name=myplot}}, width={}in]

'''.format(num_element_types, num_compilers, 6.5 / num_element_types)

    for i in range(len(graphs)):
        graph = graphs[i]

        title = '\\empty'
        if i < num_element_types:
            title = '{{{graph.title}}}'.format(**locals())

        xlabel = '\\empty'
        if i / num_element_types == num_compilers - 1:
            xlabel = '{N}'

        local_ylabel = '\\empty'
        if i % num_element_types == 0:
            local_ylabel = '{{{}}}'.format(pretty_compiler_names[compiler_names[i / num_element_types]])

        retval += '''
    \\nextgroupplot[
        title={title},
        xlabel={xlabel},
        ylabel={local_ylabel},
        xmin=0, xmax={graph.xmax},
        ymin=0, ymax={graph.ymax},
        xtick={{{graph.xtick}}},
        xticklabels={{{graph.xtick_labels}}},
        ytick={{{graph.ytick}}},
        ymajorgrids=true,
        grid style=dashed,
        scaled x ticks=false,
        scaled y ticks=true,
        ]

'''.format(**locals())

        for j in range(len(graph.plots)):
            plot = graph.plots[j]
            dashed = plot.dashed and 'dashed' or ''
            retval += '''    \\addplot[color={plot.color},mark={plot.mark},no markers,{dashed}]
        coordinates {{{plot.points}}};
        \\label{{plots:plot{j}}}

'''.format(**locals())

        if i == 0:
            retval += '''    \coordinate (top) at (rel axis cs:0,1);% coordinate at top of the first plot

'''
        if i == len(graphs) - 1:
            retval += '''    \coordinate (bot) at (rel axis cs:1,0);% coordinate at bottom of the last plot

'''

    retval += '''    \\end{{groupplot}}

    \\path (myplot c1r1.outer north west)% plot in column 1 row 1
          -- node[anchor=south,rotate=90] {{{ylabel}}}% label midway
          (myplot c1r{num_compilers}.outer south west);% plot in column 1 row {num_compilers}

    % legend
    \path (myplot c1r{num_compilers}.south west|-current bounding box.south)--
      coordinate(legendpos)
      (myplot c{num_element_types}r{num_compilers}.south east|-current bounding box.south);
    \matrix[
        matrix of nodes,
        anchor=south,
        draw,
        inner sep=0.2em,
        draw
      ]at([yshift=-5ex]legendpos)
      {{'''.format(**locals())

    legends = map(lambda x: pretty_variant_names[x], variant_names)
    for i in range(len(legends)):
        retval += '''
        \\ref{{plots:plot{}}}& {}&[5pt]'''.format(i, legends[i])

    retval += '''\\\\
      };

\\end{tikzpicture}
\\\\
\\\\
'''

    return retval

contents = open('../../paper/motivation_and_scope.in.tex', 'r').read()

contents = contents.replace('%%% insert, int, string %%%', operation_graphs('insert', 'int', 'string'))
contents = contents.replace('%%% iterate, int, string %%%', operation_graphs('iterate', 'int', 'string'))
contents = contents.replace('%%% find, int, string %%%', operation_graphs('find', 'int', 'string'))
contents = contents.replace('%%% erase, int, string %%%', operation_graphs('erase', 'int', 'string'))

contents = contents.replace('%%% insert/iterate, int, string %%%', operation_graphs('insert/iterate', 'int', 'string'))
contents = contents.replace('%%% insert/find, int, string %%%', operation_graphs('insert/find', 'int', 'string'))

open('../../paper/motivation_and_scope.tex', 'w').write(contents)
