//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Compound_Conditional_25
 */ 
class MutatorFrontendAction_25 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(25)

private:
    class MutatorASTConsumer_25 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_25(Rewriter &R) : TheRewriter(R) {}
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
        const VarDecl *existingVarDecl = nullptr;
    };
};

//source file
#include "../include/Nested_Compound_Conditional_25.h"

// ========================================================================================================
#define MUT25_OUTPUT 1

void MutatorFrontendAction_25::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *IF = Result.Nodes.getNodeAs<clang::IfStmt>("ifStmt")) {
        //Filter nodes in header files
        if (!IF || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IF->getBeginLoc()))
            return;

        // Find an existing variable declaration in the same function
        if (!existingVarDecl) {
            if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
                for (auto *D : FD->decls()) {
                    if (auto *VD = llvm::dyn_cast<clang::VarDecl>(D)) {
                        existingVarDecl = VD;
                        break;
                    }
                }
            }
        }

        if (!existingVarDecl)
            return;

        //Get the source code text of target node
        auto condRange = IF->getCond()->getSourceRange();
        std::string condText = stringutils::rangetoStr(*(Result.SourceManager), condRange);

        //Perform mutation on the source code text by applying string replacement
        std::string newCond = "({ " + existingVarDecl->getType().getAsString() + " tempVar = " + existingVarDecl->getNameAsString() + "; if (tempVar) break; tempVar; })";

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(condRange), newCond);
    }
}

void MutatorFrontendAction_25::MutatorASTConsumer_25::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = ifStmt(hasAncestor(functionDecl().bind("funcDecl"))).bind("ifStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}