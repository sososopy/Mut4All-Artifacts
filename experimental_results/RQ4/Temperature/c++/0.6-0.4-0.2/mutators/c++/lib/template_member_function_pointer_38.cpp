//source file
#include "../include/Mutator_38.h"

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberExpr>("MemberFunctionPointer")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
        return;

      auto sourceRange = MT->getSourceRange();
      auto sourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), 
                                             *Result.SourceManager, 
                                             Result.Context->getLangOpts());

      std::string mutatedText = sourceText.str();
      size_t templatePos = mutatedText.find("template");
      if (templatePos != std::string::npos) {
          mutatedText.erase(templatePos, 8); // Remove 'template' keyword
      }

      Rewrite.ReplaceText(sourceRange, mutatedText);
    }
}
  
void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = memberExpr(hasType(templateSpecializationType()), 
                              hasAncestor(cxxRecordDecl(isTemplateInstantiation())))
                              .bind("MemberFunctionPointer");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}