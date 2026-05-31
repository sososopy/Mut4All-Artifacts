//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Noexcept_With_Requires_121
 */ 
class MutatorFrontendAction_121 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(121)

private:
    class MutatorASTConsumer_121 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_121(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Lambda_Noexcept_With_Requires_121.h"

// ========================================================================================================
#define MUT121_OUTPUT 1

void MutatorFrontendAction_121::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto lambda_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getSourceRange());
      llvm::outs() << "Lambda: " << lambda_str << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto noex_pos = lambda_str.find("noexcept");
      if (noex_pos == string::npos)
        return;
      auto left_brac_pos = lambda_str.find('(', noex_pos);
      if (left_brac_pos == string::npos)
        return;
      auto right_brac_pos = lambda_str.find(')', left_brac_pos);
      if (right_brac_pos == string::npos)
        return;
      auto noex_content =
          lambda_str.substr(left_brac_pos + 1,
                            right_brac_pos - left_brac_pos - 1); // get the content of noexcept
      llvm::outs() << "Noexcept content: " << noex_content << '\n';
      auto requires_pos = lambda_str.find("requires");
      if (requires_pos == string::npos)
        return;
      auto requires_left_brac_pos = lambda_str.find('{', requires_pos);
      if (requires_left_brac_pos == string::npos)
        return;
      auto requires_right_brac_pos =
          lambda_str.find('}', requires_left_brac_pos);
      if (requires_right_brac_pos == string::npos)
        return;
      auto requires_content = lambda_str.substr(
          requires_left_brac_pos + 1,
          requires_right_brac_pos - requires_left_brac_pos - 1);
      llvm::outs() << "Requires content: " << requires_content << '\n';
      lambda_str.replace(left_brac_pos + 1,
                         right_brac_pos - left_brac_pos - 1, requires_content);
      llvm::outs() << "Mutated lambda: " << lambda_str << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), lambda_str);
    }
}
  
void MutatorFrontendAction_121::MutatorASTConsumer_121::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}