//header file
#pragma once
#include "Mutator_base.h"

/**
 * use_incomplete_struct_in_constexpr_5
 */ 
class MutatorFrontendAction_5 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(5)

private:
    class MutatorASTConsumer_5 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_5(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/use_incomplete_struct_in_constexpr_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::RecordDecl>("IncompleteStruct")) {
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;

        if (SD->isCompleteDefinition())
            return;

        std::string structName = SD->getNameAsString();
        std::string constexprDecl = "constexpr " + structName + " myVar = {0};\n";

        SourceLocation insertLoc = SD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertTextAfter(insertLoc, "/*mut5*/" + constexprDecl);
    }
}

void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(unless(isDefinition())).bind("IncompleteStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}