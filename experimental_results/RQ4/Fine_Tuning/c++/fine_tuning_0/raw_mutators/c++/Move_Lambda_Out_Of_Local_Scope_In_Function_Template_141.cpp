//header file
#pragma once
#include "Mutator_base.h"

/**
 * Move_Lambda_Out_Of_Local_Scope_In_Function_Template_141
 */ 
class MutatorFrontendAction_141 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(141)

private:
    class MutatorASTConsumer_141 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_141(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/move_lambda_out_of_local_scope_in_function_template_141.h"

// ========================================================================================================
#define MUT141_OUTPUT 1

void MutatorFrontendAction_141::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto lambda_pos = declaration.find("[]");
      if (lambda_pos == string::npos)
        return;
      auto lambda_end = declaration.find(";", lambda_pos);
      if (lambda_end == string::npos)
        return;
      auto lambda_expr = declaration.substr(lambda_pos, lambda_end - lambda_pos + 1);
      llvm::outs() << lambda_expr << '\n';
      declaration.erase(lambda_pos, lambda_end - lambda_pos + 1);
      auto brace_pos = declaration.find("{");
      if (brace_pos == string::npos)
        return;
      declaration.insert(brace_pos + 1, lambda_expr);
      llvm::outs() << declaration << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_141::MutatorASTConsumer_141::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}