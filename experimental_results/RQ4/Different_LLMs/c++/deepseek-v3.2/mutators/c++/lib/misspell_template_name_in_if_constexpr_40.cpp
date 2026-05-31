//source file
#include "../include/Misspell_Template_Name_In_If_Constexpr_40.h"
#include <string>
#include <cctype>
#include <random>

// ========================================================================================================
#define MUT40_OUTPUT 1

std::string MutatorFrontendAction_40::Callback::generateMisspelling(const std::string &original2) {
    if (original2.empty()) return original2;
    std::string misspelled = original2;
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, 2);
    int operation = dist(rng);
    std::uniform_int_distribution<> posDist(0, misspelled.size() - 1);
    size_t pos = posDist(rng);
    
    switch(operation) {
        case 0: // duplicate a letter
            if (pos < misspelled.size()) {
                misspelled.insert(pos, 1, misspelled[pos]);
            }
            break;
        case 1: // omit a letter
            if (misspelled.size() > 1) {
                misspelled.erase(pos, 1);
            }
            break;
        case 2: // substitute a vowel
            if (pos < misspelled.size()) {
                char c = misspelled[pos];
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
                    c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
                    char newVowel;
                    do {
                        static const char vowels[] = {'a','e','i','o','u','A','E','I','O','U'};
                        std::uniform_int_distribution<> vowelDist(0, 9);
                        newVowel = vowels[vowelDist(rng)];
                    } while (newVowel == c);
                    misspelled[pos] = newVowel;
                }
            }
            break;
    }
    return misspelled;
}

bool MutatorFrontendAction_40::Callback::isDeclaredIdentifier(ASTContext &Context, const std::string &id) {
    IdentifierInfo &II = Context.Idents.get(id);
    auto lookupResult = Context.getTranslationUnitDecl()->lookup(&II);
    return !lookupResult.empty();
}

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *IF = Result.Nodes.getNodeAs<clang::IfStmt>("IfConstexpr")) {
      //Filter nodes in header files
      if (!IF || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IF->getBeginLoc()))
        return;
      
      if (!IF->isConstexpr()) return;
      
      auto *cond = IF->getCond();
      if (!cond) return;
      
      // Look for a template-id in the condition
      std::string condText = stringutils::rangetoStr(*(Result.SourceManager), 
                                                     cond->getSourceRange());
      
      // Find template-id pattern: identifier followed by '<'
      size_t templateStart = condText.find('<');
      if (templateStart == std::string::npos) return;
      
      // Find the identifier before '<'
      size_t idStart = templateStart;
      while (idStart > 0 && (isalnum(condText[idStart-1]) || condText[idStart-1] == '_' || 
                             condText[idStart-1] == ':')) {
        idStart--;
      }
      
      if (idStart >= templateStart) return;
      
      // Extract the full identifier (could be qualified with namespace)
      std::string fullId = condText.substr(idStart, templateStart - idStart);
      
      // Find the last component (the actual template name)
      size_t lastColon = fullId.rfind("::");
      std::string templateName;
      std::string prefix;
      if (lastColon != std::string::npos) {
        prefix = fullId.substr(0, lastColon + 2);
        templateName = fullId.substr(lastColon + 2);
      } else {
        templateName = fullId;
      }
      
      if (templateName.empty()) return;
      
      // Generate misspelling
      std::string misspelled;
      int attempts = 0;
      do {
        misspelled = generateMisspelling(templateName);
        attempts++;
      } while ((misspelled == templateName || 
               isDeclaredIdentifier(*Result.Context, prefix + misspelled)) && 
               attempts < 10);
      
      if (misspelled == templateName || attempts >= 10) return;
      
      //Get the source code text of target node
      std::string mutatedCond = condText;
      std::string newFullId = prefix + misspelled;
      mutatedCond.replace(idStart, templateStart - idStart, newFullId);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(cond->getSourceRange()), 
                          mutatedCond);
    }
}
  
void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = ifStmt(isConstexpr()).bind("IfConstexpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}