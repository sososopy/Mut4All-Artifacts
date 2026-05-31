//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Type_With_Const_153
 */ 
class MutatorFrontendAction_153 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(153)

private:
    class MutatorASTConsumer_153 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_153(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Type_With_Const_153.h"

// ========================================================================================================
#define MUT153_OUTPUT 1

void MutatorFrontendAction_153::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("ftd")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;

        clang::FunctionDecl *FD = FTD->getTemplatedDecl();
        if (!FD) return;

        for (auto *Param : FD->parameters()) {
            clang::QualType QT = Param->getType();
            if (const auto *TTP = QT->getAs<clang::TemplateTypeParmType>()) {
                if (!QT.hasQualifiers() || !QT.getQualifiers().hasConst()) {
                    clang::TypeSourceInfo *TSI = Param->getTypeSourceInfo();
                    if (!TSI) continue;
                    clang::SourceLocation Start = TSI->getTypeLoc().getBeginLoc();
                    clang::SourceLocation End = TSI->getTypeLoc().getEndLoc();
                    if (Start.isInvalid() || End.isInvalid()) continue;

                    std::string originalText = Rewrite.getRewrittenText(clang::SourceRange(Start, End));
                    if (originalText.empty()) continue;

                    Rewrite.ReplaceText(clang::SourceRange(Start, End), "const " + originalText);
                }
            }
        }
    }
}

void MutatorFrontendAction_153::MutatorASTConsumer_153::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("ftd");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}