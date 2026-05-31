//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Binary_Operator_Overload_478
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Binary_Operator_Overload_478.h"

// ========================================================================================================
#define MUT478_OUTPUT 1

void MutatorFrontendAction_478::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("BinaryOperatorOverload")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (MT->isOverloadedOperator() &&
          MT->getOverloadedOperator() == OverloadedOperatorKind::OO_Plus) {
        auto body = MT->getBody();
        llvm::outs() << "Mut478: " << body << '\n';
        auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                body->getSourceRange());
        llvm::outs() << "Mut478: " << body_str << '\n';
        //Perform mutation on the source code text by applying string replacement
        stringutils::strReplace(body_str, "+", "*");
        body_str = "/*mut478*/" + body_str;
        llvm::outs() << "Mut478: " << body_str << '\n';
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(body->getSourceRange(), body_str);
      }
    }
}
  
void MutatorFrontendAction_478::MutatorASTConsumer_478::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl().bind("BinaryOperatorOverload");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}