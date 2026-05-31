//source file
#include "../include/Ref_to_RRef_Conversion_In_Local_Struct_Method_7.h"

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("LocalStructMethod")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto body = MT->getBody();
      auto refs = body->children();
      for (auto ref : refs) {
        if (auto *DRE = dyn_cast<DeclRefExpr>(ref)) {
          auto VD = DRE->getDecl();
          if (VD->getKind() == Decl::Kind::Var) {
            auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                                DRE->getSourceRange());
            text = "std::move(" + text + ")";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(DRE->getSourceRange()), text);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
                                      hasParent(cxxRecordDecl(isStruct())),
                                      hasAncestor(functionDecl()))
                                      .bind("LocalStructMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}