//source file
#include "../include/modify_template_function_specialization_39.h"

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFuncSpec")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getTemplatedKind() == FunctionDecl::TK_FunctionTemplateSpecialization) {
        auto &SM = Rewrite.getSourceMgr();
        auto &LangOpts = Rewrite.getLangOpts();
        SourceLocation StartLoc = FD->getBeginLoc();
        SourceLocation EndLoc = FD->getEndLoc();
        if (StartLoc.isValid() && EndLoc.isValid()) {
          std::string FuncText = Lexer::getSourceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), SM, LangOpts).str();
          size_t pos = FuncText.find("template<>");
          if (pos != std::string::npos) {
            FuncText.replace(pos, 10, "template<typename U>");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), FuncText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isExplicitTemplateSpecialization()).bind("TemplateFuncSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}