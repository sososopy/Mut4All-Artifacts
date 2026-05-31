//source file
#include "../include/modify_template_function_return_type_to_decltype_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getTemplatedDecl()->getReturnType()->isUndeducedAutoType()) {
        auto declRange = FD->getTemplatedDecl()->getSourceRange();
        std::string sourceCode = stringutils::rangetoStr(*(Result.SourceManager), declRange);
        
        size_t pos = sourceCode.find("auto");
        if (pos != std::string::npos) {
          sourceCode.replace(pos, 4, "decltype(auto)");
        }
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(declRange), sourceCode);
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