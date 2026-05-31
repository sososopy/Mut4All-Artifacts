//header file
#pragma once
#include "Mutator_base.h"

/**
 * Recursive_Trailing_Return_Type_For_Variadic_Lambdas_169
 */ 
class MutatorFrontendAction_169 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(169)

private:
    class MutatorASTConsumer_169 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_169(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Recursive_Trailing_Return_Type_For_Variadic_Lambdas_169.h"

// ========================================================================================================
#define MUT169_OUTPUT 1

void MutatorFrontendAction_169::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        // Filter nodes in header files
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getBeginLoc()))
            return;

        // Check if it's a variadic lambda with auto parameters
        auto *callOp = lambda->getCallOperator();
        if (!callOp)
            return;

        auto params = callOp->parameters();
        if (params.empty())
            return;

        auto firstParam = params[0];
        if (!firstParam->getType()->isAutoType() || !firstParam->isParameterPack())
            return;

        // Generate nested return type
        std::string nested = "decltype(args)";
        for (int i = 0; i < 3; ++i) {
            nested = "complex_template<" + nested + ">";
        }
        nested = "decltype(" + nested + ")";

        // Find insertion point for trailing return type
        SourceLocation endParenLoc = lambda->getRParenLoc();
        if (endParenLoc.isInvalid()) {
            return; // Shouldn't happen for valid lambdas
        }

        // Insert -> and the nested return type
        std::string returnType = " -> " + nested;
        Rewrite.InsertText(endParenLoc, returnType, true, true);

        // Find and modify the return statement
        auto body = lambda->getBody();
        if (!body)
            return;

        if (auto *compound = dyn_cast<CompoundStmt>(body)) {
            for (Stmt *s : compound->body()) {
                if (auto *returnStmt = dyn_cast<ReturnStmt>(s)) {
                    // Generate new return statement
                    std::string inner = nested.substr(8, nested.size() - 9); // Remove "decltype(" and ")"
                    std::string newReturn = "return " + inner + ";";
                    Rewrite.ReplaceText(returnStmt->getSourceRange(), newReturn);
                    break;
                }
            }
        }
    }
}
  
void MutatorFrontendAction_169::MutatorASTConsumer_169::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to find variadic lambdas
    DeclarationMatcher matcher = lambdaExpr().bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}