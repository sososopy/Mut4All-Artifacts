//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Std_Feature_In_Function_Compound_34
 */ 
class MutatorFrontendAction_34 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(34)

private:
    class MutatorASTConsumer_34 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_34(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Std_Feature_In_Function_Compound_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Func")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->hasBody())
        return;
      //Get the source code text of target node
      auto body = MT->getBody();
      if (!body->isCompoundStmt())
        return;
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      body_str.insert(body_str.find('{') + 1,
                      "/*mut34*/std::string_view s = \"example\";\n");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), body_str);
    }
}
  
void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}