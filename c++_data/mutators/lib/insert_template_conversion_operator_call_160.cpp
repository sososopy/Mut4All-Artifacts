//source file
#include "../include/insert_template_conversion_operator_call_160.h"

// ========================================================================================================
#define MUT160_OUTPUT 1

void MutatorFrontendAction_160::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Method")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->hasBody()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        std::string insertion = "0, operator float();\n";
        declaration.insert(declaration.find("{") + 1, insertion);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_160::MutatorASTConsumer_160::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(ofClass(cxxRecordDecl(isTemplateInstantiation()))).bind("Method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}