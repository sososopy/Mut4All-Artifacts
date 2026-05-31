//header file
#pragma once
#include "Mutator_base.h"

/**
 * insert_incomplete_static_member_49
 */ 
class MutatorFrontendAction_49 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(49)

private:
    class MutatorASTConsumer_49 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_49(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_incomplete_static_member_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AnonStruct = Result.Nodes.getNodeAs<clang::RecordDecl>("AnonStruct")) {
        if (!AnonStruct || !Result.Context->getSourceManager().isWrittenInMainFile(
                AnonStruct->getLocation()))
            return;

        if (!AnonStruct->isAnonymousStructOrUnion())
            return;

        auto structRange = AnonStruct->getSourceRange();
        std::string structText = stringutils::rangetoStr(*(Result.SourceManager), structRange);

        std::string incompleteTypeDecl = "\nstatic ::IncompleteType;\n";
        if (structText.rfind('}') != std::string::npos)
            structText.insert(structText.rfind('}'), incompleteTypeDecl);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(structRange), structText);
    }
}

void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(isAnonymousStructOrUnion()).bind("AnonStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}