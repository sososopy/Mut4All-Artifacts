//header file
#pragma once
#include "Mutator_base.h"

/**
 * overly_large_bitfield_size_562
 */ 
class MutatorFrontendAction_562 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(562)

private:
    class MutatorASTConsumer_562 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_562(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/overly_large_bitfield_size_562.h"

// ========================================================================================================
#define MUT562_OUTPUT 1

void MutatorFrontendAction_562::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;

        for (auto *field : SD->fields()) {
            if (field->isBitField()) {
                auto fieldSourceRange = field->getSourceRange();
                auto fieldText = stringutils::rangetoStr(*(Result.SourceManager), fieldSourceRange);
                size_t colonPos = fieldText.find(':');
                if (colonPos != std::string::npos) {
                    std::string newBitfield = fieldText.substr(0, colonPos + 1) + " 1 + (unsigned long long)0xFFFFFFFFFFFFFFFF";
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(fieldSourceRange), newBitfield);
                }
            }
        }
    }
}

void MutatorFrontendAction_562::MutatorASTConsumer_562::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}