//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Function_Ptr_Type_With_Auto_258
 */ 
class MutatorFrontendAction_258 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(258)

private:
    class MutatorASTConsumer_258 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_258(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Member_Function_Ptr_Type_With_Auto_258.h"

// ========================================================================================================
#define MUT258_OUTPUT 1

void MutatorFrontendAction_258::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CastExpr = Result.Nodes.getNodeAs<clang::CXXStaticCastExpr>("CastExpr")) {
        if (!CastExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CastExpr->getBeginLoc()))
            return;

        const Type *DestType = CastExpr->getType().getTypePtr();
        if (auto *MPType = dyn_cast<clang::MemberPointerType>(DestType)) {
            if (auto *FuncType = MPType->getPointeeType()->getAs<clang::FunctionType>()) {
                clang::TypeSourceInfo *TSI = CastExpr->getTypeSourceInfo();
                if (!TSI) return;

                clang::SourceLocation Start = TSI->getTypeLoc().getBeginLoc();
                clang::SourceLocation End = TSI->getTypeLoc().getEndLoc();
                if (Start.isInvalid() || End.isInvalid()) return;

                std::string DestTypeText = clang::Lexer::getSourceText(
                    clang::CharSourceRange::getTokenRange(clang::SourceRange(Start, End)),
                    Result.Context->getSourceManager(),
                    Result.Context->getLangOpts()
                );

                size_t firstSpace = DestTypeText.find(' ');
                if (firstSpace != std::string::npos) {
                    std::string newDestTypeText = "auto" + DestTypeText.substr(firstSpace);
                    Rewrite.ReplaceText(SourceRange(Start, End), newDestTypeText);
                }
            }
        }
    }
}

void MutatorFrontendAction_258::MutatorASTConsumer_258::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = stmt(
        hasDescendant(cxxStaticCastExpr(
            hasType(memberPointerType(pointee(functionType())))
        ).bind("CastExpr"))
    );
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}