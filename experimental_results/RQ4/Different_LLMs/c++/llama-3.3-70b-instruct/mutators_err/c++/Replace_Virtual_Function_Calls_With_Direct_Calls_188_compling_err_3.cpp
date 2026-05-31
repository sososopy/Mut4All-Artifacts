//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Virtual_Function_Calls_With_Direct_Calls_188
 */ 
class MutatorFrontendAction_188 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_188>(TheRewriter);
    }

private:
    class MutatorASTConsumer_188 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_188(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
    Rewriter TheRewriter;
};

//source file
#include "../include/Replace_Virtual_Function_Calls_With_Direct_Calls_188.h"

// ========================================================================================================
#define MUT188_OUTPUT 1

void MutatorFrontendAction_188::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("VirtualCalls")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc().getLocWithOffset(0)))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace virtual function call with direct call
      if (auto callee = dyn_cast<clang::DeclRefExpr>(MT->getCallee())) {
        std::string directCall = callee->getNameAsString();
        declaration.replace(declaration.find(callee->getNameAsString()), 
                            callee->getNameAsString().length(), directCall);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_188::MutatorASTConsumer_188::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMemberCallExpr().bind("VirtualCalls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}