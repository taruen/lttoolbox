/*
 * Copyright (C) 2005 Universitat d'Alacant / Universidad de Alicante
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <https://www.gnu.org/licenses/>.
 */
#include <lttoolbox/fst_processor.h>
#include <lttoolbox/file_utils.h>
#include <lttoolbox/my_stdio.h>
#include <lttoolbox/lt_locale.h>

#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <libgen.h>

void endProgram(char *name)
{
  std::cout << basename(name) << ": process a stream with a letter transducer" << std::endl;
  std::cout << "USAGE: " << basename(name) << " [ -a | -b | -c | -d | -e | -g | -n | -p | -x | -s | -t | -v | -h | -z | -w ] [-W] [-N N] [-L N] [ -i icx_file ] [ -r rcx_file ] fst_file [input_file [output_file]]" << std::endl;
  std::cout << "Options:" << std::endl;
#if HAVE_GETOPT_LONG
  std::cout << "  -a, --analysis:          morphological analysis (default behavior)" << std::endl;
  std::cout << "  -b, --bilingual:         lexical transfer" << std::endl;
  std::cout << "  -c, --case-sensitive:    use the literal case of the incoming characters" << std::endl;
  std::cout << "  -d, --debugged-gen       morph. generation with all the stuff" << std::endl;
  std::cout << "  -e, --decompose-nouns:   Try to decompound unknown words" << std::endl;
  std::cout << "  -g, --generation:        morphological generation" << std::endl;
  std::cout << "  -i, --ignored-chars:     specify file with characters to ignore" << std::endl;
  std::cout << "  -r, --restore-chars:     specify file with characters to diacritic restoration" << std::endl;
  std::cout << "  -l, --tagged-gen:        morphological generation keeping lexical forms" << std::endl;
  std::cout << "  -m, --tagged-nm-gen:     same as -l but without unknown word marks" << std::endl;
  std::cout << "  -n, --non-marked-gen     morph. generation without unknown word marks" << std::endl;
  std::cout << "  -o, --surf-bilingual:    lexical transfer with surface forms" << std::endl;
  std::cout << "  -p, --post-generation:   post-generation" << std::endl;
  std::cout << "  -x, --inter-generation:  inter-generation" << std::endl;
  std::cout << "  -s, --sao:               SAO annotation system input processing" << std::endl;
  std::cout << "  -t, --transliteration:   apply transliteration dictionary" << std::endl;
  std::cout << "  -v, --version:           version" << std::endl;
  std::cout << "  -z, --null-flush:        flush output on the null character " << std::endl;
  std::cout << "  -w, --dictionary-case:   use dictionary case instead of surface case" << std::endl;
  std::cout << "  -C, --careful-case:      use dictionary case if present, else surface" << std::endl;
  std::cout << "  -I, --no-default-ignore: skips loading the default ignore characters" << std::endl;
  std::cout << "  -W, --show-weights:      Print final analysis weights (if any)" << std::endl;
  std::cout << "  -N, --analyses:          Output no more than N analyses (if the transducer is weighted, the N best analyses)" << std::endl;
  std::cout << "  -L, --weight-classes:    Output no more than N best weight classes (where analyses with equal weight constitute a class)" << std::endl;
  std::cout << "  -h, --help:              show this help" << std::endl;
#else
  std::cout << "  -a:   morphological analysis (default behavior)" << std::endl;
  std::cout << "  -b:   lexical transfer" << std::endl;
  std::cout << "  -c:   use the literal case of the incoming characters" << std::endl;
  std::cout << "  -d:   morph. generation with all the stuff" << std::endl;
  std::cout << "  -e:   try to decompose unknown words as compounds" << std::endl;
  std::cout << "  -g:   morphological generation" << std::endl;
  std::cout << "  -i:   specify file with characters to ignore" << std::endl;
  std::cout << "  -r:   specify file with characters to diacritic restoration" << std::endl;
  std::cout << "  -l:   morphological generation keeping lexical forms" << std::endl;
  std::cout << "  -n:   morph. generation without unknown word marks" << std::endl;
  std::cout << "  -o:   lexical transfer with surface forms" << std::endl;
  std::cout << "  -p:   post-generation" << std::endl;
  std::cout << "  -x:   inter-generation" << std::endl;
  std::cout << "  -s:   SAO annotation system input processing" << std::endl;
  std::cout << "  -t:   apply transliteration dictionary" << std::endl;
  std::cout << "  -v:   version" << std::endl;
  std::cout << "  -z:   flush output on the null character " << std::endl;
  std::cout << "  -C:   use dictionary case if present, else surface" << std::endl;
  std::cout << "  -W:   Print final analysis weights (if any)" << std::endl;
  std::cout << "  -N:   Output no more than N analyses" << std::endl;
  std::cout << "  -L:   Output no more than N best weight classes" << std::endl;
  std::cout << "  -I:   skips loading the default ignore characters" << std::endl;
  std::cout << "  -w:   use dictionary case instead of surface case" << std::endl;
  std::cout << "  -h:   show this help" << std::endl;
#endif
  exit(EXIT_FAILURE);
}

void checkValidity(FSTProcessor const &fstp)
{
  if(!fstp.valid())
  {
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[])
{
  LtLocale::tryToSetLocale();

  int cmd = 0;
  int maxAnalyses;
  int maxWeightClasses;
  FSTProcessor fstp;

#if HAVE_GETOPT_LONG
  static struct option long_options[]=
    {
      {"analysis",          0, 0, 'a'},
      {"bilingual",         0, 0, 'b'},
      {"surf-bilingual",    0, 0, 'o'},
      {"generation",        0, 0, 'g'},
      {"ignored-chars",     1, 0, 'i'},
      {"restore-chars",     1, 0, 'r'},
      {"non-marked-gen",    0, 0, 'n'},
      {"debugged-gen",      0, 0, 'd'},
      {"tagged-gen",        0, 0, 'l'},
      {"tagged-nm-gen",     0, 0, 'm'},
      {"post-generation",   0, 0, 'p'},
      {"inter-generation",  0, 0, 'x'},
      {"sao",               0, 0, 's'},
      {"transliteration",   0, 0, 't'},
      {"null-flush",        0, 0, 'z'},
      {"dictionary-case",   0, 0, 'w'},
      {"version",           0, 0, 'v'},
      {"case-sensitive",    0, 0, 'c'},
      {"careful-case",      0, 0, 'C'},
      {"no-default-ignore", 0, 0, 'I'},
      {"show-weights",      0, 0, 'W'},
      {"analyses",          1, 0, 'N'},
      {"weight-classes",    1, 0, 'L'},
      {"help",              0, 0, 'h'}
    };
#endif

  GenerationMode bilmode = gm_unknown;
  // more than one option sets generation mode, but -gb also sets gm_unknown
  bool really_g = false;
  while(true)
  {
#if HAVE_GETOPT_LONG
    int option_index;
    int c = getopt_long(argc, argv, "abcegi:r:lmndopxstzwvCIWN:L:h", long_options, &option_index);
#else
    int c = getopt(argc, argv, "abcegi:r:lmndopxstzwvCIWN:L:h");
#endif

    if(c == -1)
    {
      break;
    }

    switch(c)
    {
    case 'c':
      fstp.setCaseSensitiveMode(true);
      break;

    case 'i':
      fstp.setIgnoredChars(true);
      fstp.parseICX(optarg);
      break;

    case 'r':
      fstp.setRestoreChars(true);
      fstp.parseRCX(optarg);
      fstp.setUseDefaultIgnoredChars(false);
      break;

    case 'I':
      fstp.setUseDefaultIgnoredChars(false);
      break;

    case 'W':
      fstp.setDisplayWeightsMode(true);
      break;

    case 'N':
      maxAnalyses = atoi(optarg);
      if (maxAnalyses < 1)
      {
        std::cerr << "Invalid or no argument for analyses count" << std::endl;
        exit(EXIT_FAILURE);
      }
      fstp.setMaxAnalysesValue(maxAnalyses);
      break;

    case 'L':
      maxWeightClasses = atoi(optarg);
      if (maxWeightClasses < 1)
      {
        std::cerr << "Invalid or no argument for weight class count" << std::endl;
        exit(EXIT_FAILURE);
      }
      fstp.setMaxWeightClassesValue(maxWeightClasses);
      break;

    case 'e':
    case 'a':
    case 'b':
    case 'o':
    case 'g':
    case 'p':
    case 'x':
    case 't':
    case 's':
      if(cmd == 0)
      {
        cmd = c;
        if (cmd == 'g') really_g = true;
      }
      else if(cmd == 'g' && c == 'b') {
        // "lt-proc -g -b generador.bin" should run biltrans, keeping unknown-marks
        if (really_g) bilmode = gm_unknown;
        cmd = 'b';
      }
      else
      {
        endProgram(argv[0]);
      }
      break;

    case 'd':
      if (cmd == 0) cmd = 'g';
      bilmode = gm_all;
      break;

    case 'l':
      if (cmd == 0) cmd = 'g';
      bilmode = gm_tagged;
      break;

    case 'm':
      if (cmd == 0) cmd = 'g';
      bilmode = gm_tagged_nm;
      break;

    case 'n':
      if (cmd == 0) cmd = 'g';
      bilmode = gm_clean;
      break;

    case 'C':
      if (cmd == 0) cmd = 'g';
      bilmode = gm_carefulcase;
      break;

    case 'z':
      fstp.setNullFlush(true);
      break;

    case 'w':
      fstp.setDictionaryCaseMode(true);
      break;

    case 'v':
      std::cout << basename(argv[0]) << " version " << PACKAGE_VERSION << std::endl;
      exit(EXIT_SUCCESS);
      break;
    case 'h':
    default:
      endProgram(argv[0]);
      break;
    }
  }

  InputFile input;
  UFILE* output = u_finit(stdout, NULL, NULL);

  if(optind == (argc - 3))
  {
    FILE* in = openInBinFile(argv[optind]);
    input.open_or_exit(argv[optind+1]);
    output = openOutTextFile(argv[optind+2]);
    fstp.load(in);
    fclose(in);
  }
  else if(optind == (argc -2))
  {
    FILE* in = openInBinFile(argv[optind]);
    input.open_or_exit(argv[optind+1]);
    fstp.load(in);
    fclose(in);
  }
  else if(optind == (argc - 1))
  {
    FILE* in = openInBinFile(argv[optind]);
    fstp.load(in);
    fclose(in);
  }
  else
  {
    endProgram(argv[0]);
  }

  try
  {
    switch(cmd)
    {
      case 'g':
        fstp.initGeneration();
        checkValidity(fstp);
        fstp.generation(input, output, bilmode);
        break;

      case 'p':
        fstp.initPostgeneration();
        checkValidity(fstp);
        fstp.postgeneration(input, output);
        break;

      case 'x':
        fstp.initPostgeneration();
        checkValidity(fstp);
        fstp.intergeneration(input, output);
        break;

      case 's':
        fstp.initAnalysis();
        checkValidity(fstp);
        fstp.SAO(input, output);
        break;

      case 't':
        fstp.initPostgeneration();
        checkValidity(fstp);
        fstp.transliteration(input, output);
        break;

      case 'o':
        fstp.initBiltrans();
        checkValidity(fstp);
        fstp.setBiltransSurfaceForms(true);
        fstp.bilingual(input, output, bilmode);
        break;

      case 'b':
        fstp.initBiltrans();
        checkValidity(fstp);
        fstp.bilingual(input, output, bilmode);
        break;

      case 'e':
        fstp.initDecomposition();
        checkValidity(fstp);
        fstp.analysis(input, output);
        break;

      case 'a':
      default:
        fstp.initAnalysis();
        checkValidity(fstp);
        fstp.analysis(input, output);
        break;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what();
    if (fstp.getNullFlush()) {
      u_fputc('\0', output);
    }

    exit(1);
  }

  u_fclose(output);
  return EXIT_SUCCESS;
}
