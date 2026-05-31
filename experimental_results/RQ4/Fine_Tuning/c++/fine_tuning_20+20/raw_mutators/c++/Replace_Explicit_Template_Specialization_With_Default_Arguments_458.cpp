//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Explicit_Template_Specialization_With_Default_Arguments_458
 */ 
class MutatorFrontendAction_458 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(458)

private:
    class MutatorASTConsumer_458 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_458(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Explicit_Template_Specialization_With_Default_Arguments_458.h"

// ========================================================================================================
#define MUT458_OUTPUT 1

void MutatorFrontendAction_458::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("MemberCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      auto callee = MT->getMethodDecl();
      if (callee == nullptr)
        return;
      auto parent = callee->getParent();
      if (parent == nullptr)
        return;
      if (parent->getNameAsString() != "A1")
        return;
      if (callee->getNameAsString() != "b")
        return;
      if (MT->getNumArgs() != 1)
        return;
      if (content.find("SomeImpl") == string::npos)
        return;
      //Perform mutation on the source code text by applying string replacement
      content = "/*mut458*/" + content.substr(0, content.find('(') + 1) + ')';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_458::MutatorASTConsumer_458::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMemberCallExpr().bind("MemberCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}