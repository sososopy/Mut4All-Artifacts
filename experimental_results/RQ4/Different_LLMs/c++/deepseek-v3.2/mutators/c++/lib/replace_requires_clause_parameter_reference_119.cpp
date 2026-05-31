//source file
#include "../include/Replace_Requires_Clause_Parameter_Reference_119.h"

// ========================================================================================================
#define MUT119_OUTPUT 1

void MutatorFrontendAction_119::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithRequires")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Collect parameters for potential replacement
      functionParameters.clear();
      for (unsigned i = 0; i < FD->getNumParams(); ++i) {
        functionParameters.push_back(FD->getParamDecl(i));
      }
      if (functionParameters.empty()) return;
      
      // Find requires clause
      if (auto *TR = FD->getTrailingRequiresClause()) {
        //Get the source code text of target node
        auto requiresRange = TR->getSourceRange();
        if (!requiresRange.isValid()) return;
        
        std::string requiresText = stringutils::rangetoStr(*(Result.SourceManager), requiresRange);
        
        // Look for decltype expressions referencing parameters
        size_t decltypePos = requiresText.find("decltype");
        if (decltypePos == std::string::npos) return;
        
        // Find the parameter reference within decltype
        size_t openParen = requiresText.find('(', decltypePos);
        if (openParen == std::string::npos) return;
        
        size_t closeParen = requiresText.find(')', openParen);
        if (closeParen == std::string::npos) return;
        
        std::string innerExpr = requiresText.substr(openParen + 1, closeParen - openParen - 1);
        
        // Check if inner expression contains std::forward or similar
        if (innerExpr.find("std::forward") == std::string::npos && 
            innerExpr.find("forward") == std::string::npos) {
          return;
        }
        
        // Choose a different parameter to replace with
        int paramIndex = getrandom::getRandomIndex(functionParameters.size() - 1);
        std::string newParamName = functionParameters[paramIndex]->getNameAsString();
        if (newParamName.empty()) return;
        
        //Perform mutation on the source code text by applying string replacement
        std::string newInnerExpr = newParamName;
        
        // Replace the inner expression
        std::string mutatedText = requiresText;
        mutatedText.replace(openParen + 1, closeParen - openParen - 1, newInnerExpr);
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(requiresRange), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_119::MutatorASTConsumer_119::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasTrailingReturn(), unless(isImplicit())).bind("FunctionWithRequires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}