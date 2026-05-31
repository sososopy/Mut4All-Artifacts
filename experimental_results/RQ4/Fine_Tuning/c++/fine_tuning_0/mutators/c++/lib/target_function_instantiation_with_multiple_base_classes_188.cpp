//source file
#include "../include/Target_Function_Instantiation_with_Multiple_Base_Classes_188.h"

// ========================================================================================================
#define MUT188_OUTPUT 1

void MutatorFrontendAction_188::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->getNumBases() < 2)
        return;
      llvm::outs() << DL->getNameAsString() << '\n';
      for (auto base : DL->bases()) {
        auto base_type = base.getType();
        auto base_record = base_type->getAsCXXRecordDecl();
        cur_bases.push_back(base_record);
      }
      for (auto base : cur_bases) {
        llvm::outs() << base->getNameAsString() << '\n';
        auto methods = base->methods();
        bool has_public_method = false;
        for (auto method : methods) {
          if (method->getAccess() == AccessSpecifier::AS_public) {
            has_public_method = true;
            break;
          }
        }
        if (!has_public_method) {
          std::string ins = "public:\nvoid mut_188(){}\n";
          Rewrite.ReplaceText(base->getEndLoc(), 0, ins);
        }
      }
      std::string ins = "public:\nvoid mut_188(){";
      for (auto base : cur_bases) {
        auto methods = base->methods();
        for (auto method : methods) {
          if (method->getAccess() == AccessSpecifier::AS_public) {
            ins += method->getNameAsString() + "();";
            break;
          }
        }
      }
      ins += "}\n";
      Rewrite.ReplaceText(DL->getEndLoc(), 0, ins);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Funcs")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isMain()) {
        Rewrite.ReplaceText(FD->getEndLoc(), 0,
                            "/*mut188*/{Derived mut_188_target;mut_188_target."
                            "mut_188();}\n");
      }
    }
  }
  
void MutatorFrontendAction_188::MutatorASTConsumer_188::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher =
        cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto func_matcher = functionDecl().bind("Funcs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}