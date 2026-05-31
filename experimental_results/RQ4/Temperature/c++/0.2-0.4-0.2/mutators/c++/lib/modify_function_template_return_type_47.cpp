//source file
#include "../include/modify_function_template_return_type_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      if (auto *FD = FT->getTemplatedDecl()) {
        if (FD->getReturnType()->isUndeducedType()) {
          std::string newReturnType = "int"; // Example: Change to 'int'
          std::string funcName = FD->getNameAsString();
          auto sourceRange = FD->getSourceRange();
          std::string funcDecl = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
          
          size_t pos = funcDecl.find("auto");
          if (pos != std::string::npos) {
            funcDecl.replace(pos, 4, newReturnType);
            if (FD->hasBody()) {
              size_t bodyPos = funcDecl.find("{");
              if (bodyPos != std::string::npos) {
                funcDecl.insert(bodyPos + 1, "\nreturn 0; // Ensure return type matches\n");
              }
            }
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), funcDecl);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(functionDecl(returns(autoType())))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}