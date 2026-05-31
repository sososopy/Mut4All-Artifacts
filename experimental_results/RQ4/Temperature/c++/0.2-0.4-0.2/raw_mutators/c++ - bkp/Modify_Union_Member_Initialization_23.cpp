//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_union_member_initialization_23
 */ 
class MutatorFrontendAction_23 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(23)

private:
    class MutatorASTConsumer_23 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_23(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_union_member_initialization_23.h"

// ========================================================================================================
#define MUT23_OUTPUT 1

void MutatorFrontendAction_23::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("UnionDecl")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                UD->getLocation()))
            return;

        for (auto *Field : UD->fields()) {
            if (Field->hasInClassInitializer()) {
                auto fieldRange = Field->getSourceRange();
                auto fieldText = stringutils::rangetoStr(*(Result.SourceManager), fieldRange);
                auto initPos = fieldText.find('=');
                if (initPos != std::string::npos) {
                    fieldText = fieldText.substr(0, initPos) + ";";
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(fieldRange), fieldText);
                }
            }
        }
    }
}

void MutatorFrontendAction_23::MutatorASTConsumer_23::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isUnion()).bind("UnionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}