//source file
#include "../include/inline_attribute_overload_conflict_443.h"

// ========================================================================================================
#define MUT443_OUTPUT 1

void MutatorFrontendAction_443::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("templateDecl")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
        return;
      clang::FunctionDecl *FD = FTD->getTemplatedDecl();
      if (!FD || !FD->hasBody())
        return;
      clang::Stmt *BodyStmt = FD->getBody();
      clang::CompoundStmt *Body = clang::dyn_cast<clang::CompoundStmt>(BodyStmt);
      if (!Body)
        return;
      for (const auto *Stmt : Body->children()) {
        if (const auto *CE = clang::dyn_cast<clang::CallExpr>(Stmt)) {
          const clang::FunctionDecl *CalledFD = CE->getDirectCallee();
          if (!CalledFD || CalledFD->isTemplateInstantiation())
            continue;
          if (FD->getNameAsString() != CalledFD->getNameAsString())
            continue;
          if (FD->getNumParams() != CalledFD->getNumParams())
            continue;
          bool typesDiffer = false;
          for (unsigned i = 0; i < FD->getNumParams(); ++i) {
            if (FD->getParamDecl(i)->getType() != CalledFD->getParamDecl(i)->getType()) {
              typesDiffer = true;
              break;
            }
          }
          if (!typesDiffer)
            continue;
          clang::SourceLocation StartLoc = CE->getBeginLoc();
          clang::SourceLocation EndLoc = CE->getEndLoc();
          if (StartLoc.isInvalid() || EndLoc.isInvalid())
            continue;
          std::string InlineAttr = "[[clang::always_inline]] ";
          const clang::ParmVarDecl *CalledParam = CalledFD->getParamDecl(0);
          std::string CastType = CalledParam->getType().getAsString();
          const clang::Expr *Arg = CE->getArg(0);
          std::string ArgText = stringutils::rangetoStr(*Result.SourceManager, Arg->getSourceRange());
          std::string NewCall = InlineAttr + "f(reinterpret_cast<" + CastType + ">(" + ArgText + "))";
          Rewrite.ReplaceText(CE->getSourceRange(), NewCall);
        }
      }
    }
}
  
void MutatorFrontendAction_443::MutatorASTConsumer_443::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(functionDecl(hasBody(compoundStmt(hasDescendant(callExpr())))))).bind("templateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}