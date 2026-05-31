//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Return_Type_In_Template_Function_256
 */ 
class MutatorFrontendAction_256 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(256)

private:
    class MutatorASTConsumer_256 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_256(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Modify_Lambda_Return_Type_In_Template_Function_256.h"

// ========================================================================================================
#define MUT256_OUTPUT 1

void MutatorFrontendAction_256::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambda_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << lambda_text << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto rtype = lambda_text.substr(lambda_text.find("->") + 2,
                                      lambda_text.find("{") - 1);
      if (rtype.find("decltype") != string::npos) {
        rtype = rtype.substr(rtype.find("(") + 1, rtype.find(")"));
        llvm::outs() << rtype << '\n';
        rtype = "decltype(" + rtype + "_mut256)";
        lambda_text.replace(lambda_text.find("->") + 2,
                            lambda_text.find("{") - 1, rtype);
      }
      llvm::outs() << lambda_text << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), lambda_text);
    }
}
  
void MutatorFrontendAction_256::MutatorASTConsumer_256::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasAncestor(functionDecl(hasAnyTemplateArgument())))
                       .bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}