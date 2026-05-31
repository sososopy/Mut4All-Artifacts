//source file
#include "../include/Exception_Specification_Mutation_In_Function_Templates_57.h"

// ========================================================================================================
#define MUT57_OUTPUT 1

void MutatorFrontendAction_57::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FT->getSourceRange());
      llvm::outs() << declaration << '\n';
      if (FT->getTemplatedDecl()->getExceptionSpecType() == clang::EST_Dynamic) {
        auto pos = declaration.find("throw()");
        if (pos != string::npos) {
          declaration.replace(pos, 7, "");
        }
      } else if (FT->getTemplatedDecl()->getExceptionSpecType() == clang::EST_None) {
        auto pos = declaration.find("noexcept");
        if (pos != string::npos) {
          declaration.replace(pos, 8, "throw()");
        }
      } else {
        auto pos = declaration.find("{");
        if (pos != string::npos) {
          declaration.insert(pos, "noexcept ");
        }
      }
      llvm::outs() << declaration << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_57::MutatorASTConsumer_57::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}