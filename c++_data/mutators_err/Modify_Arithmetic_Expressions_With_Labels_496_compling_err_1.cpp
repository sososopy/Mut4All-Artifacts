//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Arithmetic_Expressions_With_Labels_496
 */ 
class MutatorFrontendAction_496 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(496)

private:
    class MutatorASTConsumer_496 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_496(Rewriter &R) : TheRewriter(R) {}
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
        const VarDecl *intVariable = nullptr; // Record an integer variable for use in mutation
    };
};

//source file
#include "../include/Modify_Arithmetic_Expressions_With_Labels_496.h"

// ========================================================================================================
#define MUT496_OUTPUT 1

void MutatorFrontendAction_496::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("arithmeticExpr")) {
      // Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getExprLoc()))
        return;

      // Get the source code text of target node
      std::string originalExpr = Lexer::getSourceText(CharSourceRange::getTokenRange(BO->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());

      if (!intVariable) {
        // Introduce a new integer variable if none exists
        intVariable = VarDecl::Create(*Result.Context, BO->getFunctionScopeInfo()->getCurFunctionDecl(), SourceLocation(), SourceLocation(), &Result.Context->Idents.get("mutVar"), Result.Context->IntTy, nullptr, SC_Static);
        intVariable->setInit(IntegerLiteral::Create(*Result.Context, llvm::APInt(32, 10), Result.Context->IntTy, SourceLocation()));
        Rewrite.InsertTextBefore(BO->getExprLoc(), "int mutVar = 10;\n");
      }

      // Perform mutation on the source code text by applying string replacement
      std::string mutatedExpr = "mutVar - " + originalExpr.substr(originalExpr.find("&&") + 2);
      
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(BO->getSourceRange(), mutatedExpr);
    }
}
  
void MutatorFrontendAction_496::MutatorASTConsumer_496::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = binaryOperator(hasOperatorName("-"), hasLHS(addrLabelExpr()), hasRHS(addrLabelExpr())).bind("arithmeticExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}