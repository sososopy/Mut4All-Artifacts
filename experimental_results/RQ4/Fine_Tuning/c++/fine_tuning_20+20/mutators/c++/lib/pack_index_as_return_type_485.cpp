//source file
#include "../include/Pack_Index_As_Return_Type_485.h"

// ========================================================================================================
#define MUT485_OUTPUT 1

void MutatorFrontendAction_485::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("Packs")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      if (TP->isParameterPack() == false)
        return;
      cur_packs.push_back(TP);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Returns")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isTemplateInstantiation() || FD->isFunctionTemplateSpecialization())
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (cur_packs.empty())
        return;
      llvm::outs() << "mut485: " << cur_packs.size() << '\n';
      auto return_type = FD->getReturnType();
      if (return_type->isDependentType() == false)
        return;
      auto tp = return_type.getTypePtr();
      if (tp->getAs<PackExpansionType>() == nullptr)
        return;
      auto idx = getrandom::getRandomIndex(cur_packs.size() - 1);
      auto pack_name = cur_packs[idx]->getNameAsString();
      auto new_return_type = pack_name + "...[0]";
      llvm::outs() << "mut485: " << new_return_type << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getReturnTypeSourceRange()), new_return_type);
    }
}
  
void MutatorFrontendAction_485::MutatorASTConsumer_485::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto pack_matcher = templateTypeParmDecl().bind("Packs");
    auto return_matcher = functionDecl().bind("Returns");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(pack_matcher, &callback);
    matchFinder.addMatcher(return_matcher, &callback);
    matchFinder.matchAST(Context);
}