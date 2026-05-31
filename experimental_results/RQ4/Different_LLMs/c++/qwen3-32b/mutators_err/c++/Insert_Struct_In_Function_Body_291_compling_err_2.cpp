//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Struct_In_Function_Body_291
 */ 
class MutatorFrontendAction_Insert_Struct_In_Function_Body_291 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Insert_Struct_In_Function_Body_291)

private:
    class MutatorASTConsumer_Insert_Struct_In_Function_Body_291 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Insert_Struct_In_Function_Body_291(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Insert_Struct_In_Function_Body_291.h"

// ========================================================================================================
#define MUTInsert_Struct_In_Function_Body_291_OUTPUT 1

void MutatorFrontendAction_Insert_Struct_In_Function_Body_291::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!FD->hasBody())
            return;

        if (const auto *Body = dyn_cast<clang::CompoundStmt>(FD->getBody())) {
            SourceLocation LBraceLoc = Body->getLBraceLoc();
            if (LBraceLoc.isInvalid())
                return;

            Rewrite.InsertTextAfterToken(LBraceLoc, " struct { } s;");
        }
    }
}
  
void MutatorFrontendAction_Insert_Struct_In_Function_Body_291::MutatorASTConsumer_Insert_Struct_In_Function_Body_291::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt())).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}