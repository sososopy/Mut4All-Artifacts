//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_TypeAlias_WithValue_375
 */ 
class MutatorFrontendAction_375 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(375)
private:
    class MutatorASTConsumer_375 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_375(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_TypeAlias_WithValue_375.h"

// ========================================================================================================
#define MUT375_OUTPUT 1

void MutatorFrontendAction_375::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TATD = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("aliasTemplate")) {
        if (!TATD || !Result.Context->getSourceManager().isWrittenInMainFile(TATD->getLocation()))
            return;
        
        clang::TypeAliasDecl *TAD = TATD->getTemplatedDecl();
        if (!TAD || !TAD->getTypeSourceInfo())
            return;
        
        clang::TypeLoc TypeLoc = TAD->getTypeSourceInfo()->getTypeLoc();
        if (!TypeLoc.isValid())
            return;
        
        SourceRange TypeRange = TypeLoc.getSourceRange();
        if (TypeRange.isInvalid())
            return;
        
        std::string OriginalType = stringutils::rangetoStr(*Result.SourceManager, TypeRange);
        if (OriginalType.empty())
            return;
        
        std::string MutatedType = OriginalType;
        size_t typenamePos = MutatedType.find("typename ");
        if (typenamePos != std::string::npos) {
            MutatedType.erase(typenamePos, 7);
        }
        
        size_t yPos = MutatedType.find("::Y");
        if (yPos != std::string::npos) {
            MutatedType.replace(yPos, 3, "::v");
        }
        
        Rewrite.ReplaceText(TypeRange, MutatedType);
    }
}

void MutatorFrontendAction_375::MutatorASTConsumer_375::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("aliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}