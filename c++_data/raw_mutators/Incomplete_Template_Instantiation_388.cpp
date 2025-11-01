//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Template_Instantiation_388
 */ 
class MutatorFrontendAction_388 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(388)

private:
    class MutatorASTConsumer_388 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_388(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Incomplete_Template_Instantiation_388.h"

// ========================================================================================================
#define MUT388_OUTPUT 1

void MutatorFrontendAction_388::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("templateFuncCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;

      if (auto *DRE = dyn_cast<DeclRefExpr>(CE->getCallee())) {
        if (auto *FD = dyn_cast<FunctionDecl>(DRE->getDecl())) {
          if (FD->getTemplatedKind() == FunctionDecl::TK_FunctionTemplateSpecialization) {
            //Get the source code text of target node
            auto callText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
            //Perform mutation on the source code text by applying string replacement
            auto parenPos = callText.find('(');
            if (parenPos != std::string::npos) {
              callText = callText.substr(0, parenPos);
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), callText);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_388::MutatorASTConsumer_388::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(
                      callee(functionDecl(isTemplateInstantiation()))
                    ).bind("templateFuncCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}