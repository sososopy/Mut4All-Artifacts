//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Compound_Statements_Using_String_View_430
 */ 
class MutatorFrontendAction_430 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(430)

private:
    class MutatorASTConsumer_430 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_430(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_compound_statements_using_string_view_430.h"

// ========================================================================================================
#define MUT430_OUTPUT 1

void MutatorFrontendAction_430::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto body = FD->getBody();
        auto bodyStr = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string tempVar = "\nstd::string_view temp_view = input.substr(0, 3);\n";
        std::string modifiedBody = "{ " + tempVar + bodyStr + " }";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), modifiedBody);
      }
    }
}
  
void MutatorFrontendAction_430::MutatorASTConsumer_430::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasParameter(0, hasType(asString("std::string_view")))).bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}