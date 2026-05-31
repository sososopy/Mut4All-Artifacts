//source file
#include "../include/UCN_To_Raw_Character_457.h"
#include <sstream>

// ========================================================================================================
#define MUT457_OUTPUT 1

void MutatorFrontendAction_457::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::NamedDecl>("NamedDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      std::string mutatedName = name;
      size_t pos = 0;
      bool changed = false;
      while (pos < mutatedName.length()) {
        if (mutatedName[pos] == '\\') {
          if (pos + 1 < mutatedName.length() && mutatedName[pos + 1] == 'u') {
            // \uXXXX
            if (pos + 6 <= mutatedName.length()) {
              std::string hexStr = mutatedName.substr(pos + 2, 4);
              unsigned int codePoint;
              std::istringstream iss(hexStr);
              iss >> std::hex >> codePoint;
              if (!iss.fail()) {
                // Check if character is allowed in identifiers (letter, digit, underscore, etc.)
                // Simplified check: allow letters, digits, underscore, and some punctuation allowed in identifiers
                // This is a simplified implementation; full Unicode identifier checks are complex
                if ((codePoint >= 0x0041 && codePoint <= 0x005A) || // Latin uppercase
                    (codePoint >= 0x0061 && codePoint <= 0x007A) || // Latin lowercase
                    (codePoint >= 0x0030 && codePoint <= 0x0039) || // digits
                    codePoint == 0x005F || // underscore
                    (codePoint >= 0x00C0 && codePoint <= 0x00D6) || // Latin extended
                    (codePoint >= 0x00D8 && codePoint <= 0x00F6) ||
                    (codePoint >= 0x00F8 && codePoint <= 0x00FF)) {
                  // Convert to UTF-8 (simplified for basic Latin range)
                  char replacement;
                  if (codePoint <= 0x7F) {
                    replacement = static_cast<char>(codePoint);
                    mutatedName.replace(pos, 6, 1, replacement);
                    changed = true;
                    pos += 1;
                  } else {
                    // For non-ASCII, we skip mutation in this simplified version
                    pos += 6;
                  }
                } else {
                  pos += 6;
                }
              } else {
                pos++;
              }
            } else {
              pos++;
            }
          } else if (pos + 1 < mutatedName.length() && mutatedName[pos + 1] == 'U') {
            // \UXXXXXXXX
            if (pos + 10 <= mutatedName.length()) {
              std::string hexStr = mutatedName.substr(pos + 2, 8);
              unsigned int codePoint;
              std::istringstream iss(hexStr);
              iss >> std::hex >> codePoint;
              if (!iss.fail()) {
                // Similar check for allowed characters
                if ((codePoint >= 0x0041 && codePoint <= 0x005A) ||
                    (codePoint >= 0x0061 && codePoint <= 0x007A) ||
                    (codePoint >= 0x0030 && codePoint <= 0x0039) ||
                    codePoint == 0x005F ||
                    (codePoint >= 0x00C0 && codePoint <= 0x00D6) ||
                    (codePoint >= 0x00D8 && codePoint <= 0x00F6) ||
                    (codePoint >= 0x00F8 && codePoint <= 0x00FF)) {
                  if (codePoint <= 0x7F) {
                    char replacement = static_cast<char>(codePoint);
                    mutatedName.replace(pos, 10, 1, replacement);
                    changed = true;
                    pos += 1;
                  } else {
                    pos += 10;
                  }
                } else {
                  pos += 10;
                }
              } else {
                pos++;
              }
            } else {
              pos++;
            }
          } else {
            pos++;
          }
        } else {
          pos++;
        }
      }
      //Perform mutation on the source code text by applying string replacement
      if (changed) {
        //Replace the original AST node with the mutated one
        SourceLocation nameLoc = MT->getLocation();
        Rewrite.ReplaceText(nameLoc, name.length(), mutatedName);
      }
    }
}
  
void MutatorFrontendAction_457::MutatorASTConsumer_457::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namedDecl().bind("NamedDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}