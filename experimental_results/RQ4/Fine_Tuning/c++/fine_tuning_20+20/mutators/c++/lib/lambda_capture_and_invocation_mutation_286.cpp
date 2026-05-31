//source file
#include "../include/lambda_capture_and_invocation_mutation_286.h"

// ========================================================================================================
#define MUT286_OUTPUT 1

void MutatorFrontendAction_286::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      cur_lambdas.push_back(LE);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DeclStmt>("Decls")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (auto var = dyn_cast<VarDecl>(decl)) {
          if (var->hasInit()) {
            auto type = var->getType();
            if (type->isStructureOrClassType()) {
              auto init = var->getInit();
              if (init->getStmtClass() == Stmt::StmtClass::LambdaExprClass) {
                auto target = init->IgnoreImplicit();
                for (auto lambda : cur_lambdas) {
                  if (target == lambda) {
                    llvm::outs() << "Find target lambda\n";
                    auto lambda_content = stringutils::rangetoStr(
                        *(Result.SourceManager), lambda->getSourceRange());
                    auto capture = lambda->getCaptureDefault();
                    if (capture == LambdaCaptureDefault::LCD_None) {
                      llvm::outs() << "None capture\n";
                      auto captures = lambda->captures();
                      for (auto cap : captures) {
                        auto capkind = cap.getCaptureKind();
                        if (capkind == LambdaCaptureKind::LCK_ByCopy) {
                          llvm::outs() << "Find by copy\n";
                          auto capvar = cap.getCapturedVar();
                          auto capname = capvar->getNameAsString();
                          llvm::outs() << capname << '\n';
                          string newcap = "c=std::move(" + capname + ")";
                          llvm::outs() << newcap << '\n';
                          auto caploc = cap.getLocation();
                          Rewrite.ReplaceText(caploc, capname.length(), newcap);
                          auto params = lambda->getCallOperator()->parameters();
                          std::vector<std::string> paras;
                          for (auto param : params) {
                            paras.push_back(param->getNameAsString());
                          }
                          string newbody = "{\nstatic_assert(std::is_same_v<decltype(std::declval<decltype(c)>()(";
                          for (size_t i = 0; i < paras.size(); ++i) {
                            if (i)
                              newbody += ',';
                            newbody += paras[i];
                          }
                          newbody +=
                              ")) , bool>);\nreturn c(" + paras[0] + ");\n}";
                          llvm::outs() << newbody << '\n';
                          Rewrite.ReplaceText(
                              CharSourceRange::getTokenRange(
                                  lambda->getBody()->getSourceRange()),
                              newbody);
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  
void MutatorFrontendAction_286::MutatorASTConsumer_286::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambda_matcher = lambdaExpr().bind("Lambdas");
    auto decl_matcher = declStmt().bind("Decls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}