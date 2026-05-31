//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Const_Generics_478
 */ 
class MutatorFrontendAction_478 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(478)

private:
    class MutatorASTConsumer_478 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_478(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::vector<const clang::FunctionDecl *> cur_funcs;
    };
};

//source file
#include "../include/Add_Const_Generics_478.h"

// ========================================================================================================
#define MUT478_OUTPUT 1

void MutatorFrontendAction_478::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isTemplateInstantiation() || FD->isFunctionTemplateSpecialization())
        return;
      if (FD->getReturnType().getAsString() != "auto")
        return;
      if (FD->getReturnType()->isDependentType() == false)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->isConstexpr())
        return;
      auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                FD->getSourceRange());
      // llvm::outs() << definition << '\n';
      cur_funcs.push_back(FD);
      auto const_ins = "/*mut478*/const ";
      if (definition.find("auto") != string::npos)
        definition.insert(definition.find("auto"), const_ins);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()),
                          definition);
    } else if (auto *RD = Result.Nodes.getNodeAs<clang::ReturnStmt>(
                   "Return0Stmt")) {
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getBeginLoc()))
        return;

      size_t index = getrandom::getRandomIndex(cur_funcs.size() - 1);
      auto target = cur_funcs[index];
      auto ins = "/*mut478*/return " + target->getNameAsString() + "<int>()";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RD->getSourceRange()),
                          ins);
    }
}
  
void MutatorFrontendAction_478::MutatorASTConsumer_478::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto return0Stmt_macher =
        returnStmt(hasReturnValue(integerLiteral(equals(0))))
            .bind("Return0Stmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(return0Stmt_macher, &callback);
    matchFinder.matchAST(Context);
}