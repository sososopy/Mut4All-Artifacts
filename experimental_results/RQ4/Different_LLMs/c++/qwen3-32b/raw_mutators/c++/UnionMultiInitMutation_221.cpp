//header file
#pragma once
#include "Mutator_base.h"

/**
 * UnionMultiInitMutation_221
 */ 
class MutatorFrontendAction_221 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(221)

private:
    class MutatorASTConsumer_221 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_221(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/UnionMultiInitMutation_221.h"

// ========================================================================================================
#define MUT221_OUTPUT 1

void MutatorFrontendAction_221::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *newExpr = Result.Nodes.getNodeAs<clang::CXXNewExpr>("newExpr")) {
        if (!newExpr || !Result.Context->getSourceManager().isWrittenInMainFile(newExpr->getBeginLoc()))
            return;

        QualType newType = newExpr->getType();
        if (!newType->isUnionType())
            return;

        const RecordDecl *unionDecl = newType->getAs<RecordType>()->getDecl();
        if (!unionDecl || !isa<UnionDecl>(unionDecl))
            return;

        if (unionDecl->getNumFields() < 2)
            return;

        const FieldDecl *firstField = unionDecl->getField(0);
        const FieldDecl *secondField = unionDecl->getField(1);
        if (!firstField || !secondField)
            return;

        if (const InitListExpr *initList = dyn_cast<InitListExpr>(newExpr->getInitializer())) {
            if (initList->getNumInits() != 1)
                return;

            SourceLocation lBraceLoc = initList->getLBraceLoc();
            SourceLocation rBraceLoc = initList->getRBraceLoc();
            if (lBraceLoc.isInvalid() || rBraceLoc.isInvalid())
                return;

            std::string originalText = stringutils::rangetoStr(
                *Result.SourceManager,
                CharSourceRange::getCharRange(lBraceLoc, rBraceLoc)
            );

            std::string newInitList = originalText + ", ." + secondField->getNameAsString() + "=1";
            Rewrite.ReplaceText(
                CharSourceRange::getCharRange(lBraceLoc, rBraceLoc),
                newInitList
            );
        }
    }
}

void MutatorFrontendAction_221::MutatorASTConsumer_221::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxNewExpr(hasInitializer(ignoringImpCasts(InitListExpr()))).bind("newExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}