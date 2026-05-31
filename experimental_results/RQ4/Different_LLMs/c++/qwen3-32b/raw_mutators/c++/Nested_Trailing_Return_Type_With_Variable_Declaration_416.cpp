//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Trailing_Return_Type_With_Variable_Declaration_416
 */ 
class MutatorFrontendAction_416 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(416)
private:
    class MutatorASTConsumer_416 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_416(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Nested_Trailing_Return_Type_With_Variable_Declaration_416.h"

// ========================================================================================================
#define MUT416_OUTPUT 1

void MutatorFrontendAction_416::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *funcDecl = Result.Nodes.getNodeAs<clang::CXXFunctionDecl>("func")) {
        // Check if it's in the main file
        if (!funcDecl || !Result.Context->getSourceManager().isWrittenInMainFile(funcDecl->getLocation()))
            return;

        // Get the return type of the function
        QualType returnType = funcDecl->getReturnType();
        std::string typeName = returnType.getAsString();

        // Generate the new parameter
        std::string newParam = typeName + " temp_val = 0";

        // Get the parameter list source range
        SourceLocation lParenLoc = funcDecl->getLParams();
        SourceLocation rParenLoc = funcDecl->getRParams();

        // Modify the parameter list
        std::string paramListText = Rewrite.getRewrittenText(SourceRange(lParenLoc, rParenLoc));
        if (paramListText.empty()) {
            Rewrite.ReplaceText(SourceRange(lParenLoc, rParenLoc), newParam);
        } else {
            Rewrite.InsertTextBefore(rParenLoc, ", " + newParam);
        }

        // Replace the trailing return type
        TypeLoc trailingReturnTypeLoc = funcDecl->getTrailingReturnType();
        SourceRange trailingReturnRange = trailingReturnTypeLoc.getSourceRange();
        std::string newReturnType = "decltype(decltype(temp_val)())";
        Rewrite.ReplaceText(trailingReturnRange, newReturnType);
    }
}
  
void MutatorFrontendAction_416::MutatorASTConsumer_416::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxFunctionDecl(hasTrailingReturnTypes()).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}