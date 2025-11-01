//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Conflicting_Parameter_Pack_90
 */ 
class MutatorFrontendAction_90 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(90)

private:
    class MutatorASTConsumer_90 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_90(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Introduce_Conflicting_Parameter_Pack_90.h"

// ========================================================================================================
#define MUT90_OUTPUT 1

void MutatorFrontendAction_90::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getTemplatedDecl()->hasBody()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());

        size_t lambdaPos = declaration.find("return [&]<size_t... Idxs>(");
        if (lambdaPos != std::string::npos) {
          size_t lambdaBodyPos = declaration.find("{", lambdaPos);
          if (lambdaBodyPos != std::string::npos) {
            size_t lambdaEndPos = declaration.find("}(args...);", lambdaBodyPos);
            if (lambdaEndPos != std::string::npos) {
              std::string extraPack = "auto... extraArgs";
              std::string extraUsage = "extraArgs...";
              declaration.insert(lambdaBodyPos + 1, extraPack + ", ");
              declaration.insert(lambdaEndPos, ", " + extraUsage);
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_90::MutatorASTConsumer_90::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(lambdaExpr())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}