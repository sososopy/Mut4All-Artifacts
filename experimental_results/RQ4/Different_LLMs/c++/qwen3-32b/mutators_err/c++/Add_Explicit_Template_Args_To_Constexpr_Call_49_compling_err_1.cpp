//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Explicit_Template_Args_To_Constexpr_Call_49
 */ 
class MutatorFrontendAction_49 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(49)

private:
    class MutatorASTConsumer_49 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_49(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Add_Explicit_Template_Args_To_Constexpr_Call_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("call")) {
        // Filter nodes in header files
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        // Check if the callee is a FunctionDecl and not a template
        if (auto *DR = dyn_cast<clang::DeclRefExpr>(CE->getCallee()->IgnoreImplicit())) {
            if (auto *FD = dyn_cast<clang::FunctionDecl>(DR->getDecl())) {
                if (FD->getTemplateParameters() && FD->getTemplateParameters()->size() > 0) {
                    return; // Skip if it's a template
                }

                // Get the source location after the function name
                SourceLocation funcEndLoc = DR->getSourceRange().getEnd();
                // Insert template arguments with an invalid type
                Rewrite.InsertText(funcEndLoc, "<InvalidType>", true, true);
            }
        }
    }
}

void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = callExpr().bind("call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}