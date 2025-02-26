from proctest import ProcTest

class ParadigmTest(ProcTest):
    inputs = ['ab<n><*>', 'y<*>', '*<n><def>']
    expectedOutputs = ['ab<n><def>:abc\nab<n><ind>:ab',
                       'y<n><ind>:y',
                       'ab<n><def>:abc']
    procdix = 'data/minimal-mono.dix'
    procdir = 'rl'
    sortoutput = True

    def runTestFlush(self, tmpd):
        proc = self.openPipe('lt-paradigm',
                             self.procflags+[tmpd+'/compiled.bin'])
        self.assertEqual(len(self.inputs), len(self.expectedOutputs))
        for inp, exp in zip(self.inputs, self.expectedOutputs):
            out = self.communicateFlush(inp + '\n', proc).strip()
            if self.sortoutput:
                srt = '\n'.join(sorted(out.splitlines()))
                self.assertEqual(exp, srt)
            else:
                self.assertEqual(exp, out)
        self.closePipe(proc, expectFail=self.expectedRetCodeFail)

class ParadigmAnalyzerTest(ParadigmTest):
    procdir = 'lr'
    procflags = ['-a']

class ExcludeTest(ParadigmTest):
    procflags = ['-e', '<ind>']
    inputs = ['*<n><*>']
    expectedOutputs = ['ab<n><def>:abc']

class SortTest(ParadigmTest):
    procflags = ['-s']
    inputs = ['*<n><*>']
    expectedOutputs = ['ab<n><def>:abc\nab<n><ind>:ab\nn<n><ind>:n\ny<n><ind>:y']
    sortoutput = False
