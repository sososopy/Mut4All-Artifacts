//source file
#include "../include/insert_unused_template_parameter_276.h"

// ========================================================================================================
#define MUT276_OUTPUT 1

void MutatorFrontendAction_276::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation() || FD->isFunctionTemplateSpecialization())
        return;

      if (!FD->hasBody())
        return;

      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());

      std::string templateParam = "template<typename UnusedParam>\n";
      declaration = templateParam + declaration;

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_276::MutatorASTConsumer_276::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(cxxConstructorDecl())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}