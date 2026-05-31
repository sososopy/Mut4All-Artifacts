//header file
#pragma once
#include "Mutator_base.h"

/**
 * This_Pointer_In_Static_Methods_14
 */ 
class MutatorFrontendAction_14 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(14)

private:
    class MutatorASTConsumer_14 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_14(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/This_Pointer_In_Static_Methods_14.h"

// ========================================================================================================
#define MUT14_OUTPUT 1

void MutatorFrontendAction_14::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("StaticMethods")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isStatic())
        return;
      //Get the source code text of target node
      auto method_name = MT->getNameAsString();
      auto method_body = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << method_name << '\n' << method_body << '\n';
      //Perform mutation on the source code text by applying string replacement
      string this_pointer_expr = "this->" + method_name + "()";
      string noexcept_expr = "noexcept(" + this_pointer_expr + ")";
      string ins = "/*mut14*/" + noexcept_expr;
      method_body.insert(method_body.find('{'), ins);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), method_body);
    }
}
  
void MutatorFrontendAction_14::MutatorASTConsumer_14::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl().bind("StaticMethods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}