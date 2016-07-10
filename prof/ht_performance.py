import numpy as np
import matplotlib.pyplot as plt

class Performance:
    def __init__(self):
        "docstring"

    def setarea(self, plot, fg_color, bg_color):
        "Sets foreground/background, labels."
        plot.set_axis_bgcolor(bg_color)
        plot.yaxis.label.set_color(fg_color)
        plot.xaxis.label.set_color(fg_color)
        plot.spines['bottom'].set_color(fg_color)
        plot.spines['top'].set_color(fg_color) 
        plot.spines['right'].set_color(fg_color)
        plot.spines['left'].set_color(fg_color)
        plot.tick_params(axis='x', colors=fg_color)
        plot.tick_params(axis='y', colors=fg_color)
    

class PerfComparison(Performance):
    def __init__(self, filename):
        "Reads Hashtable's performance estimations file"
        data = np.loadtxt(filename, dtype='double', delimiter=' ')
        self.n = []
        self.ht_insertions = []
        self.um_insertions = []
        self.max_insertion = 0
        self.ht_retrieval = []
        self.um_retrieval = []
        self.max_retrieval = 0
        for datarow in data:
            self.n.append(datarow[0])

            self.ht_insertions.append(datarow[1]);
            if datarow[1] > self.max_insertion:
                self.max_insertion = datarow[1]

            self.um_insertions.append(datarow[2]);
            if datarow[2] > self.max_insertion:
                self.max_insertion = datarow[2]

            self.ht_retrieval.append(datarow[3]);
            if datarow[3] > self.max_retrieval:
                self.max_retrieval = datarow[3]

            self.um_retrieval.append(datarow[4]);
            if datarow[4] > self.max_retrieval:
                self.max_retrieval = datarow[4]

    def plot_insertion(self, plot):
        'Plots an insertion estimations.'
        self.setarea(plot, '#cccccc', '#333333')
        plot.set_xscale('log')
        ht_dots, = plot.plot(self.n, self.ht_insertions, 'o', label = 'esr::Hashtable')
        plt.setp(ht_dots, color='#00ff00')
        um_dots, = plot.plot(self.n, self.um_insertions, 'x', label = 'std::unordered_map')
        plt.setp(um_dots, color='#ffff00')
        legend = plot.legend(handles=[ht_dots, um_dots])
        legend.get_frame().set_color('#333333') 
        legend.get_frame().set_edgecolor('#cccccc') 
        for text in legend.get_texts():
            plt.setp(text, color = '#cccccc')
        plot.grid(True)
 
    def plot_retrieval(self, plot):
        'Plots an retrieval estimations.'
        self.setarea(plot, '#cccccc', '#333333')
        plot.set_xscale('log')
        ht_dots, = plot.plot(self.n, self.ht_retrieval, 'o', label = 'esr::Hashtable')
        plt.setp(ht_dots, color='#00ff00')
        um_dots, = plot.plot(self.n, self.um_retrieval, 'x', label = 'std::unordered_map')
        plt.setp(um_dots, color='#ffff00')
        legend = plot.legend(handles=[ht_dots, um_dots])
        legend.get_frame().set_color('#333333') 
        legend.get_frame().set_edgecolor('#cccccc') 
        for text in legend.get_texts():
            plt.setp(text, color = '#cccccc')
        plot.grid(True)
    def mean_estimations(self) :
        'Prints Means of estimations and their differences.'

        m_ht_insertion = sum(self.ht_insertions)/float(len(self.ht_insertions))
        m_um_insertion = sum(self.um_insertions)/float(len(self.um_insertions))
        insertion = (m_ht_insertion, m_um_insertion, abs(m_ht_insertion - m_um_insertion))

        m_ht_retrieval = sum(self.ht_retrieval)/float(len(self.ht_retrieval))
        m_um_retrieval = sum(self.um_retrieval)/float(len(self.um_retrieval))
        retrieval = (m_ht_retrieval, m_um_retrieval, abs(m_ht_retrieval - m_um_retrieval))

        print 'Isertion'
        print insertion
        print 'Retrieval'
        print retrieval 



# Integers
perfComparisonIntegers = PerfComparison('input/int_univ.csv')

figIntegers = plt.figure(1)
figIntegers.set_figheight(20)
figIntegers.set_figwidth(8.5)
figIntegers.canvas.set_window_title('Hashtable and std::unordered_map performance')

rect = figIntegers.patch
rect.set_facecolor('#333333') 

axInsertionIntegers = figIntegers.add_subplot(2,1,1)
axRetrievalsIntegers = figIntegers.add_subplot(2,1,2)

axInsertionIntegers.set_title('Isertion of Integers', color='#cccccc')
perfComparisonIntegers.plot_insertion(axInsertionIntegers);

axRetrievalsIntegers.set_title('Retrieval of Integers', color='#cccccc')
perfComparisonIntegers.plot_retrieval(axRetrievalsIntegers);

figIntegers.set_size_inches(16.0, 10.5)
int_perf = '/home/fima/texmf/kmbeamer-master/examples/int_univ_perf.png'
plt.savefig(int_perf, dpi=100, facecolor=figIntegers.get_facecolor(), edgecolor='none')
perfComparisonIntegers.mean_estimations()

# Fixed length strings
perfComparisonFixedStrings = PerfComparison('input/fixed_string.csv')

figFixedStrings = plt.figure(2)
figFixedStrings.set_figheight(20)
figFixedStrings.set_figwidth(8.5)
figFixedStrings.canvas.set_window_title('Hashtable and std::unordered_map performance')

rect = figFixedStrings.patch
rect.set_facecolor('#333333') 

axInsertionFixedStrings = figFixedStrings.add_subplot(2,1,1)
axRetrievalsFixedStrings = figFixedStrings.add_subplot(2,1,2)

axInsertionFixedStrings.set_title('Isertion of Fixed Length Strings', color='#cccccc')
perfComparisonFixedStrings.plot_insertion(axInsertionFixedStrings);

axRetrievalsFixedStrings.set_title('Retrieval of Fixed Length Strings', color='#cccccc')
perfComparisonFixedStrings.plot_retrieval(axRetrievalsFixedStrings);

figFixedStrings.set_size_inches(16.0, 10.5)
fstr_perf = '/home/fima/texmf/kmbeamer-master/examples/fstr_perf.png'
plt.savefig(fstr_perf, dpi=100, facecolor=figFixedStrings.get_facecolor(), edgecolor='none')

# Variable length strings
perfComparisonVariableStrings = PerfComparison('input/var_string.csv')

figVariableStrings = plt.figure(3)
figVariableStrings.set_figheight(20)
figVariableStrings.set_figwidth(8.5)
figVariableStrings.canvas.set_window_title('Hashtable and std::unordered_map performance')

rect = figVariableStrings.patch
rect.set_facecolor('#333333') 

axInsertionVariableStrings = figVariableStrings.add_subplot(2,1,1)
axRetrievalsVariableStrings = figVariableStrings.add_subplot(2,1,2)

axInsertionVariableStrings.set_title('Isertion of Variable Length Strings', color='#cccccc')
perfComparisonVariableStrings.plot_insertion(axInsertionVariableStrings);

axRetrievalsVariableStrings.set_title('Retrieval of Variable Length Strings', color='#cccccc')
perfComparisonVariableStrings.plot_retrieval(axRetrievalsVariableStrings);

figVariableStrings.set_size_inches(16.0, 10.5)
vstr_perf = '/home/fima/texmf/kmbeamer-master/examples/vstr_perf.png'
plt.savefig(vstr_perf, dpi=100, facecolor=figVariableStrings.get_facecolor(), edgecolor='none')

#plt.show()
