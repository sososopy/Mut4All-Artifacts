//header file
#pragma once
#include "Mutator_base.h"

/**
 * Unmatched_Control_Flow_Labels_364
 */ 
class MutatorFrontendAction_364 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(364)

private:
    class MutatorASTConsumer_364 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_364(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Unmatched_Control_Flow_Labels_364.h"

// ========================================================================================================
#define MUT364_OUTPUT 1

void MutatorFrontendAction_364::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncWithDestructors")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto body = FD->getBody();
            for (auto &stmt : body->children()) {
                if (auto *declStmt = llvm::dyn_cast<clang::DeclStmt>(stmt)) {
                    for (auto *decl : declStmt->decls()) {
                        if (auto *varDecl = llvm::dyn_cast<clang::VarDecl>(decl)) {
                            if (varDecl->getType().isDestructedType()) {
                                std::string labelName = "SKIP_" + varDecl->getNameAsString();
                                std::string gotoStmt = "goto " + labelName + ";";
                                std::string labelStmt = labelName + ":;";

                                SourceLocation insertLoc = varDecl->getBeginLoc();
                                Rewrite.InsertTextBefore(insertLoc, labelStmt + "\n");
                                Rewrite.InsertTextBefore(stmt->getBeginLoc(), gotoStmt + "\n");
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_364::MutatorASTConsumer_364::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasDescendant(varDecl(hasType(isDestructedType())))).bind("FuncWithDestructors");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}