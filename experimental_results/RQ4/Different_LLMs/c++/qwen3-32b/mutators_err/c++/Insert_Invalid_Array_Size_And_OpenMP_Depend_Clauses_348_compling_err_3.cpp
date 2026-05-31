//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Array_Size_And_OpenMP_Depend_Clauses_348
 */ 
class MutatorFrontendAction_348 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(348)

private:
    class MutatorASTConsumer_348 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_348(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> mutatedArrays;
    };
};

//source file
#include "../include/Insert_Invalid_Array_Size_And_OpenMP_Depend_Clauses_348.h"

// ========================================================================================================
#define MUT348_OUTPUT 1

void MutatorFrontendAction_348::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (auto *SizeExpr = Result.Nodes.getNodeAs<clang::Expr>("sizeExpr")) {
            std::string newSize = getrandom::getRandomIndex(1) ? "-42" : "0.5";
            SourceLocation StartLoc = SizeExpr->getBeginLoc();
            SourceLocation EndLoc = SizeExpr->getEndLoc();
            if (StartLoc.isValid() && EndLoc.isValid()) {
                Rewrite.ReplaceText(SourceRange(StartLoc, EndLoc), newSize);
            }
            mutatedArrays.push_back(VD->getNameAsString());
        }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!FD->hasBody())
            return;

        if (mutatedArrays.empty())
            return;

        std::string arrayName = mutatedArrays[0];
        std::string ompPragma = "#pragma omp task depend(inout: " + arrayName + "[0.75:2])\n";
        const Stmt *Body = FD->getBody();
        if (Body) {
            SourceLocation BodyLoc = Body->getBeginLoc();
            if (BodyLoc.isValid()) {
                SourceLocation InsertLoc = BodyLoc.getLocWithOffset(1);
                Rewrite.InsertText(InsertLoc, ompPragma);
            }
        }
    }
}
  
void MutatorFrontendAction_348::MutatorASTConsumer_348::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher arrayMatcher = varDecl(
        hasType(arrayType(hasSizeExpr(ignoringImpCasts(expr().bind("sizeExpr"))))),
        unless(isStaticStorageClass()),
        unless(hasAttr(attr::ExternC))
    ).bind("arrayDecl");
    DeclarationMatcher funcMatcher = functionDecl(hasBody(compoundStmt())).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(arrayMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}