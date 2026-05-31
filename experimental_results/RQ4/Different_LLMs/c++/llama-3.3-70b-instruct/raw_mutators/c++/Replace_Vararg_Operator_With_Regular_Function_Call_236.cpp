//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Vararg_Operator_With_Regular_Function_Call_236
 */ 
class MutatorFrontendAction_236 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(236)

private:
    class MutatorASTConsumer_236 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_236(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Vararg_Operator_With_Regular_Function_Call_236.h"

// ========================================================================================================
#define MUT236_OUTPUT 1

void MutatorFrontendAction_236::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::BinaryOperator>(("VarargOperator"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      declaration = "func(" + declaration.substr(0, declaration.find(',')) + ", " + declaration.substr(declaration.find(',') + 1) + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_236::MutatorASTConsumer_236::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = binaryOperator(hasOperatorName(","), hasLHS(ignoringParenImpCasts(declRefExpr(to(varDecl(hasType(asString("StaticAsserter")))))))).bind("VarargOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}