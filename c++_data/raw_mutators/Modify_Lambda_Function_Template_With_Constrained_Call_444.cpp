//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Function_Template_With_Constrained_Call_444
 */ 
class MutatorFrontendAction_444 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(444)

private:
    class MutatorASTConsumer_444 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_444(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_lambda_function_template_with_constrained_call_444.h"

// ========================================================================================================
#define MUT444_OUTPUT 1

void MutatorFrontendAction_444::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("lambdaVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                  VD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t pos = lambdaSource.find("requires c<void>");
      if (pos != std::string::npos) {
        lambdaSource.replace(pos, 17, "requires c<int>");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_444::MutatorASTConsumer_444::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(autoType()), hasInitializer(lambdaExpr(hasRequiresClause()))).bind("lambdaVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}