//source file
#include "../include/Replace_Typedef_Array_Brace_Init_142.h"

// ========================================================================================================
#define MUT142_OUTPUT 1

void MutatorFrontendAction_142::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("Typedef")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      auto type = TD->getUnderlyingType();
      if (type->isArrayType()) {
        llvm::outs() << "Typedef: " << TD->getNameAsString() << '\n';
        if (auto *VD =
                Result.Nodes.getNodeAs<clang::VarDecl>("VarWithTypedef")) {
          if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                         VD->getLocation()))
            return;
          if (VD->getType().getAsString() == TD->getNameAsString()) {
            if (VD->hasInit()) {
              llvm::outs() << "Var: " << VD->getNameAsString() << '\n';
              auto init = VD->getInit();
              if (auto *IL = dyn_cast<InitListExpr>(init)) {
                auto inits = IL->inits();
                string init_str = "(";
                for (auto subinit : inits) {
                  init_str +=
                      stringutils::rangetoStr(*(Result.SourceManager),
                                              subinit->getSourceRange()) +
                      ",";
                }
                init_str.pop_back();
                init_str += ")";
                llvm::outs() << init_str << '\n';
                Rewrite.ReplaceText(IL->getSourceRange(), init_str);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_142::MutatorASTConsumer_142::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto typedef_matcher = typedefDecl().bind("Typedef");
    auto var_matcher = varDecl().bind("VarWithTypedef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typedef_matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.matchAST(Context);
}