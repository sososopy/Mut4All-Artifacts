//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Data_Member_Access_With_Templated_Type_Dependency_429
 */ 
class MutatorFrontendAction_429 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(429)

private:
    class MutatorASTConsumer_429 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_429(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Static_Data_Member_Access_With_Templated_Type_Dependency_429.h"

// ========================================================================================================
#define MUT429_OUTPUT 1

void MutatorFrontendAction_429::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberExpr>(("MemberExpr"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Check if the member expression is accessing a static data member
      if (MT->isStatic()) {
        //Get the type of the static data member
        auto type = MT->getType();
        //Get the name of the static data member
        auto name = MT->getMemberDecl()->getNameAsString();
        //Perform mutation on the source code text by applying string replacement
        //Replace the access to the static data member with a templated type
        auto templatedType = "typename " + name + "<" + type.getAsString() + ">";
        declaration.replace(declaration.find(name), name.length(), templatedType);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_429::MutatorASTConsumer_429::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = memberExpr(hasType(isInteger())).bind("MemberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}