//source file
#include "../include/Replace_Type_Keyword_With_Attribute_315.h"

// ========================================================================================================
#define MUT315_OUTPUT 1

void MutatorFrontendAction_315::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAliases")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto aliasedType = DL->getTypeSourceInfo()->getType();
      auto aliasedTypeStr = aliasedType.getAsString();
      llvm::outs() << "Aliased type: " << aliasedTypeStr << '\n';
      string attr = "[[nodiscard]]";
      Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut315*/" + attr);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::TypedefDecl>("Typedefs")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto aliasedType = DL->getTypeSourceInfo()->getType();
      auto aliasedTypeStr = aliasedType.getAsString();
      llvm::outs() << "Aliased type: " << aliasedTypeStr << '\n';
      string attr = "[[nodiscard]]";
      Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut315*/" + attr);
    }
}

void MutatorFrontendAction_315::MutatorASTConsumer_315::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto typeAliasMatcher = typeAliasDecl().bind("TypeAliases");
    auto typedefMatcher = typedefDecl().bind("Typedefs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typeAliasMatcher, &callback);
    matchFinder.addMatcher(typedefMatcher, &callback);
    matchFinder.matchAST(Context);
}