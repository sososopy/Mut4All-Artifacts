//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Access_With_Pointer_Member_Access_380
 */ 
class MutatorFrontendAction_380 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(380)

private:
    class MutatorASTConsumer_380 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_380(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Member_Access_With_Pointer_Member_Access_380.h"

// ========================================================================================================
#define MUT380_OUTPUT 1

void MutatorFrontendAction_380::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MA = Result.Nodes.getNodeAs<clang::MemberExpr>(("MemberAccess"))) {
      //Filter nodes in header files
      if (!MA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MA->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MA->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedDeclaration = declaration;
      size_t dotPosition = mutatedDeclaration.find('.');
      if (dotPosition != std::string::npos) {
        mutatedDeclaration.replace(dotPosition, 1, "->");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MA->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_380::MutatorASTConsumer_380::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = memberExpr().bind("MemberAccess");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}