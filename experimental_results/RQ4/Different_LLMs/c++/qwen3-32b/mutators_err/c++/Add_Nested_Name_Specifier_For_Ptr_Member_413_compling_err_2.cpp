//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Nested_Name_Specifier_For_Ptr_Member_413
 */ 
class MutatorFrontendAction_413 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(413)

private:
    class MutatorASTConsumer_413 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_413(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Nested_Name_Specifier_For_Ptr_Member_413.h"

// ========================================================================================================
#define MUT413_OUTPUT 1

void MutatorFrontendAction_413::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Unary = Result.Nodes.getNodeAs<clang::UnaryOperator>("unary")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(Unary->getLocStart()))
            return;

        if (Unary->getOpcode() != clang::UO_AddrOf)
            return;

        auto *ME = dyn_cast<clang::MemberExpr>(Unary->getSubExpr());
        if (!ME || !ME->getQualifier())
            return;

        clang::NestedNameSpecifier *NNS = ME->getQualifier();
        clang::NestedNameSpecifierLoc NNSLoc = ME->getQualifierLoc();
        clang::TypeSourceInfo *TInfo = NNSLoc.getTypeSourceInfo();
        if (!TInfo)
            return;

        clang::QualType T = TInfo->getType();
        clang::CXXRecordDecl *RD = T->getAsCXXRecordDecl();
        if (!RD)
            return;

        for (const auto &Base : RD->bases()) {
            clang::CXXBaseSpecifier *BaseSpec = &Base;
            clang::QualType BaseType = BaseSpec->getType();
            clang::CXXRecordDecl *BaseDecl = BaseType->getAsCXXRecordDecl();
            if (BaseDecl) {
                std::string BaseName = BaseDecl->getNameAsString();
                clang::SourceLocation InsertLoc = NNSLoc.getRange().getBegin();
                Rewrite.InsertText(InsertLoc, BaseName + "::", true, true);
                return;
            }
        }
    }
}

void MutatorFrontendAction_413::MutatorASTConsumer_413::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = unaryOperator(
        hasOperatorName("&"),
        hasUnaryOperand(
            memberExpr(
                hasNestedNameSpecifierLoc()
            )
        ),
        hasAncestor(callExpr())
    ).bind("unary");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}