//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Constexpr_Function_Returning_Size_377
 */ 
class MutatorFrontendAction_377 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(377)

private:
    class MutatorASTConsumer_377 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_377(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_constexpr_function_returning_size_377.h"

// ========================================================================================================
#define MUT377_OUTPUT 1

void MutatorFrontendAction_377::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("ConstexprSizeMethod")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->hasBody() && MT->isConstexpr() && MT->getReturnType()->isIntegerType()) {
        //Get the source code text of target node
        auto body = MT->getBody();
        if (auto *RS = llvm::dyn_cast<clang::ReturnStmt>(body->child_begin())) {
          //Perform mutation on the source code text by applying string replacement
          std::string newReturn = "/*mut377*/return -1;";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(RS->getSourceRange(), newReturn);
        }
      }
    }
}
  
void MutatorFrontendAction_377::MutatorASTConsumer_377::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(
        isConstexpr(),
        returns(isInteger()),
        ofClass(cxxRecordDecl(isDefinition()))
    ).bind("ConstexprSizeMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}