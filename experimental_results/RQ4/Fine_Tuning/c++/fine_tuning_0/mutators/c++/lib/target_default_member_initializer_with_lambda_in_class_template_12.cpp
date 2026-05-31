//source file
#include "../include/Target_Default_Member_Initializer_With_Lambda_In_Class_Template_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassTemplates")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if ((!MT->isStruct() && !MT->isClass()) || MT->isLambda())
        return;
      if (!MT->isCompleteDefinition())
        return;
      if (!MT->isTemplateDecl())
        return;
      auto fields = MT->fields();
      for (auto field : fields) {
        if (field->hasInClassInitializer()) {
          auto inclassinit = field->getInClassInitializer();
          if (inclassinit->getStmtClass() == Stmt::StmtClass::LambdaExprClass) {
            auto initrange = inclassinit->getSourceRange();
            auto initstr = stringutils::rangetoStr(*(Result.SourceManager), initrange);
            llvm::outs() << initstr << '\n';
            Rewrite.ReplaceText(inclassinit->getSourceRange(), "0");
          }
        }
      }
    }
}
  
void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("ClassTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}