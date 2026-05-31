//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_Compound_Statement_In_Function_264
 */ 
class MutatorFrontendAction_264 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(264)

private:
    class MutatorASTConsumer_264 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_264(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Target_Compound_Statement_In_Function_264.h"

// ========================================================================================================
#define MUT264_OUTPUT 1

void MutatorFrontendAction_264::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isThisDeclarationADefinition())
        return;
      //Get the source code text of target node
      auto body = MT->getBody();
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      auto pos = body_str.find("{");
      if (pos == string::npos)
        return;
      body_str.insert(pos + 1, "/*mut264*/{ int y = 1; if ( ");
      llvm::outs() << body_str << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), body_str);
    }
}
  
void MutatorFrontendAction_264::MutatorASTConsumer_264::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}