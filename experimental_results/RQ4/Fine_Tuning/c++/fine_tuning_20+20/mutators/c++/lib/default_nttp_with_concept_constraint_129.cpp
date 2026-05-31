//source file
#include "../include/Default_NTTP_With_Concept_Constraint_129.h"

// ========================================================================================================
#define MUT129_OUTPUT 1

void MutatorFrontendAction_129::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TP = Result.Nodes.getNodeAs<clang::NonTypeTemplateParmDecl>("Template")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      if (TP->hasDefaultArgument())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TP->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("Invokable") != string::npos) {
        content += "/*mut129*/=[]{return 1;}";
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(TP->getSourceRange()), content);
      }
    }
}
  
void MutatorFrontendAction_129::MutatorASTConsumer_129::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = nonTypeTemplateParmDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}