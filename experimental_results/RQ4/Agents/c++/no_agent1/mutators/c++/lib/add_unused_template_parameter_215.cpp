//source file
#include "../include/add_unused_template_parameter_215.h"

// ========================================================================================================
#define MUT215_OUTPUT 1

void MutatorFrontendAction_215::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getTemplatedKind() == FunctionDecl::TK_NonTemplate) {
        auto functionText = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FD->getSourceRange());
        auto templateText = "template<typename UnusedType>\n";
        functionText.insert(0, templateText);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
      }
    }
}
  
void MutatorFrontendAction_215::MutatorASTConsumer_215::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(isTemplateInstantiation())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}