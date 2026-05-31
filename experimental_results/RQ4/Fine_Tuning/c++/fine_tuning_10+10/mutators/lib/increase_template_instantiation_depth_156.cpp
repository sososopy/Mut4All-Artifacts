//source file
#include "../include/Increase_Template_Instantiation_Depth_156.h"

// ========================================================================================================
#define MUT156_OUTPUT 1

void MutatorFrontendAction_156::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      if (FT->getTemplatedDecl()->hasBody()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FT->getTemplatedDecl()->getSourceRange());
        size_t pos = declaration.find("{");
        if (pos != std::string::npos) {
          declaration.insert(pos + 1, "\nif (value % 2 == 0) return value + recursiveFunction(value - 1) + recursiveFunction(value - 2);\n");
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getTemplatedDecl()->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_156::MutatorASTConsumer_156::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(ifStmt())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}