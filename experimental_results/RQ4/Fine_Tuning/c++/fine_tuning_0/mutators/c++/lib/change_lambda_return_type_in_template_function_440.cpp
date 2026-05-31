//source file
#include "../include/Change_Lambda_Return_Type_In_Template_Function_440.h"

// ========================================================================================================
#define MUT440_OUTPUT 1

void MutatorFrontendAction_440::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LF = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LF || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LF->getBeginLoc()))
        return;
      auto DL = LF->getLambdaClass();
      if (DL == nullptr)
        return;
      auto method = DL->methods();
      for (auto m : method) {
        if (m->getNameAsString() == "operator()") {
          auto ret_type = m->getReturnType();
          if (ret_type->isVoidType() == true)
            return;
        }
      }
      auto ret_type = stringutils::rangetoStr(*(Result.SourceManager),
                                              LF->getSourceRange());
      llvm::outs() << ret_type << '\n';
      if (ret_type == "")
        return;
      if (var_decl != nullptr) {
        auto name = var_decl->getNameAsString();
        ret_type = "decltype(" + name + ")";
        ret_type = "/*mut440*/" + ret_type;
        llvm::outs() << ret_type << '\n';
        Rewrite.ReplaceText(LF->getBeginLoc(), 4, ret_type);
      }
    } else if (auto *TF = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                   "Template")) {
      if (!TF || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TF->getBeginLoc()))
        return;
      auto DL = TF->getTemplatedDecl();
      if (DL->hasBody() == false)
        return;
      auto vars = DL->decls();
      for (auto var : vars) {
        if (var->getKind() == clang::Decl::Kind::Var) {
          var_decl = dyn_cast<VarDecl>(var);
          return;
        }
      }
    }
}
  
void MutatorFrontendAction_440::MutatorASTConsumer_440::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambda_matcher = lambdaExpr().bind("Lambda");
    auto template_matcher = functionTemplateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}