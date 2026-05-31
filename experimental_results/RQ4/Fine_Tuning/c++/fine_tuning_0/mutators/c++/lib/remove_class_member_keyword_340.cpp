//source file
#include "../include/Remove_Class_Member_Keyword_340.h"

// ========================================================================================================
#define MUT340_OUTPUT 1

void MutatorFrontendAction_340::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      auto decls = CL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Ordinary) {
          llvm::outs() << "Found a class member declaration!\n";
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 decl->getSourceRange());
          llvm::outs() << content << "\n";
          if (content.find(' ') != string::npos) {
            content.erase(0, content.find(' ') + 1);
          }
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(decl->getSourceRange()), content);
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_340::MutatorASTConsumer_340::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}