//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Global_New_Delete_396
 */ 
class MutatorFrontendAction_396 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(396)

private:
    class MutatorASTConsumer_396 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_396(Rewriter &R) : TheRewriter(R) {}
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
        bool newFound = false;
        std::vector<const clang::FunctionDecl*> candidateFunctions;
    };
};

//source file
#include "../include/Insert_Global_New_Delete_396.h"

// ========================================================================================================
#define MUT396_OUTPUT 1

void MutatorFrontendAction_396::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *newDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("newDecl")) {
        newFound = true;
        return;
    } else if (auto *funcDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (funcDecl->hasBody()) {
            candidateFunctions.push_back(funcDecl);
        }
    }
}
  
void MutatorFrontendAction_396::MutatorASTConsumer_396::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher newMatcher = functionDecl(
        hasName("operator new"),
        hasParameter(0, hasType(asString("std::size_t"))),
        hasReturnType(qualType(pointsTo(asString("void"))))
    ).bind("newDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(newMatcher, &callback);
    matchFinder.matchAST(Context);
    
    if (!callback.newFound) {
        SourceManager &SM = TheRewriter.getSourceMgr();
        FileID fileID = SM.getMainFileID();
        SourceLocation loc = SM.getLocForStartOfFile(fileID);
        TheRewriter.InsertText(loc, "__host__ void* operator new(std::size_t);\n", true, true);
    }
    
    DeclarationMatcher funcMatcher = functionDecl(hasBody(compoundStmt())).bind("funcDecl");
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
    
    if (!callback.candidateFunctions.empty()) {
        const clang::FunctionDecl *func = callback.candidateFunctions[0];
        const clang::CompoundStmt *body = func->getBody();
        if (body) {
            SourceLocation startLoc = body->getLBracLoc();
            TheRewriter.InsertTextAfterToken(startLoc, "delete (void*)0;\n");
        }
    }
}