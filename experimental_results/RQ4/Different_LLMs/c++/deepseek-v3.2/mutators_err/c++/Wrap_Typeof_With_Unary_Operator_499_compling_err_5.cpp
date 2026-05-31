//header file
#pragma once
#include "Mutator_base.h"

/**
 * Wrap_Typeof_With_Unary_Operator_499
 */ 
class MutatorFrontendAction_499 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(499)

private:
    class MutatorASTConsumer_499 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_499(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Wrap_Typeof_With_Unary_Operator_499.h"

// ========================================================================================================
#define MUT499_OUTPUT 1

void MutatorFrontendAction_499::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("typeofVarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto typeSourceRange = VD->getTypeSourceInfo()->getTypeLoc().getSourceRange();
      auto typeSourceText = stringutils::rangetoStr(*(Result.SourceManager), typeSourceRange);
      //Check if the type is a typeof expression
      if (typeSourceText.find("__typeof__") == std::string::npos)
        return;
      //Extract the inner expression from typeof
      size_t start = typeSourceText.find("(");
      size_t end = typeSourceText.rfind(")");
      if (start == std::string::npos || end == std::string::npos || start >= end)
        return;
      std::string innerExpr = typeSourceText.substr(start + 1, end - start - 1);
      //Choose a random unary operator
      std::vector<std::string> operators = {"+", "-", "*", "&", "!"};
      int opIndex = getrandom::getRandomIndex(operators.size() - 1);
      std::string chosenOp = operators[opIndex];
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedType = "__typeof__(" + chosenOp + " " + innerExpr + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(typeSourceRange, mutatedType);
    }
}
  
void MutatorFrontendAction_499::MutatorASTConsumer_499::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(hasUnqualifiedDesugaredType(hasTypeOfExpr()))).bind("typeofVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}