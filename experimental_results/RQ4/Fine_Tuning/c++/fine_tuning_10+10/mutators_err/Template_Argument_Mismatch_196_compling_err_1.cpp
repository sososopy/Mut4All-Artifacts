//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Argument_Mismatch_196
 */ 
class MutatorFrontendAction_196 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(196)

private:
    class MutatorASTConsumer_196 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_196(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Template_Argument_Mismatch_196.h"

// ========================================================================================================
#define MUT196_OUTPUT 1

void MutatorFrontendAction_196::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("templateCall")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;

      if (auto *FD = CE->getDirectCallee()) {
        if (FD->getTemplatedKind() == FunctionDecl::TK_FunctionTemplate) {
          if (auto *FTS = FD->getDescribedFunctionTemplate()) {
            if (FTS->getTemplateParameters()->size() > 0) {
              std::string mutatedCall = "/*mut196*/" + FD->getNameAsString() + "<int>(";
              for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
                if (i > 0) mutatedCall += ", ";
                mutatedCall += Lexer::getSourceText(CharSourceRange::getTokenRange(CE->getArg(i)->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str();
              }
              mutatedCall += ")";
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCall);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_196::MutatorASTConsumer_196::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasTemplateArgumentList())).bind("templateCall"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}