//header file
#pragma once
#include "Mutator_base.h"

/**
 * Default_Argument_With_Overloaded_Function_167
 */ 
class MutatorFrontendAction_167 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(167)

private:
    class MutatorASTConsumer_167 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_167(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionDecl *OriginalFunc = nullptr;
    };
};

//source file
#include "../include/Default_Argument_With_Overloaded_Function_167.h"

// ========================================================================================================
#define MUT167_OUTPUT 1

void MutatorFrontendAction_167::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (const FunctionDecl *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        // Ensure the function has default arguments
        bool hasDefaultArg = false;
        for (unsigned i = 0; i < FD->getNumParams(); ++i) {
            if (FD->getParamDecl(i)->hasDefaultArg()) {
                hasDefaultArg = true;
                break;
            }
        }

        if (!hasDefaultArg)
            return;

        // Record the original function declaration
        OriginalFunc = FD;

        // Generate an overloaded version
        std::string overloadedFunc = FD->getReturnType().getAsString() + " " + FD->getNameAsString() + "(double a) {\n";
        overloadedFunc += "    return static_cast<int>(a * 2);\n";
        overloadedFunc += "}\n";

        // Insert the overloaded function after the original
        SourceLocation insertLoc = FD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertTextAfter(insertLoc, "\n/*mut167*/" + overloadedFunc);
    }
}

void MutatorFrontendAction_167::MutatorASTConsumer_167::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}