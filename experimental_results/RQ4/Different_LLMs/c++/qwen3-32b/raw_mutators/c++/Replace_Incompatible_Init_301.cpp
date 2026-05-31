//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Incompatible_Init_301
 */ 
class MutatorFrontendAction_301 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(301)

private:
    class MutatorASTConsumer_301 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_301(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Incompatible_Init_301.h"

// ========================================================================================================
#define MUT301_OUTPUT 1

void MutatorFrontendAction_301::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *varDecl = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        // Filter nodes in header files
        if (!varDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                       varDecl->getLocation()))
            return;

        if (!varDecl->hasInit()) return;

        auto init = varDecl->getInit();
        auto *initList = dyn_cast<clang::InitListExpr>(init);
        if (!initList) return;

        QualType varType = varDecl->getType();
        if (!varType->isScalarType() && !varType->isArrayType()) {
            return;
        }

        for (unsigned i = 0; i < initList->getNumInits(); ++i) {
            auto *element = initList->getInit(i);
            if (!element) continue;

            QualType elementType = element->getType();

            std::string replacement;
            if (elementType->isIntegerType()) {
                replacement = "3.14";
            } else if (elementType->isFloatingType()) {
                replacement = "nullptr";
            } else if (elementType->isPointerType()) {
                replacement = "0";
            } else {
                continue;
            }

            SourceRange range = element->getSourceRange();
            if (range.isInvalid()) continue;

            Rewrite.ReplaceText(range, replacement);
        }
    }
}
  
void MutatorFrontendAction_301::MutatorASTConsumer_301::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = varDecl(
        (hasStorageClass(clang::StorageClass::SC_Static) || hasExternalStorage()),
        hasInitializer(ignoringImplicit(implicitCastExpr(ignoringParenImpCasts(
            initListExpr().bind("initList")))))
    ).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}